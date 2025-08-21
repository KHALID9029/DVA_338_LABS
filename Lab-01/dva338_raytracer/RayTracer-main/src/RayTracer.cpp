#include "RayTracer.h"
#include "raytrace_features.h"

void RayTracer::searchClosestHit(const Ray & ray, HitRec & hitRec) {
    hitRec.anyHit = false;

	hitRec.tHit = ray.tClip;

	bool hit = false;
	int trackPrimIndex = 0;

    for (int i = 0; i < this->scene->objects.size(); i++) {
        hit = false;
        hit=this->scene->objects[i]->hit(ray, hitRec);

        if (hit) {
			trackPrimIndex = i;
        }
    }

	hitRec.primIndex = trackPrimIndex;

	hitRec.material = this->scene->objects[hitRec.primIndex]->getMaterial();

	this->scene->objects[hitRec.primIndex]->computeSurfaceHitFields(ray,hitRec);
}

Vec3f RayTracer::getEyeRayDirection(int x, int y) {
    //Uses a fix camera looking along the negative z-axis
    static float z = -5.0f;		
    static float sizeX = 4.0f; 
    static float sizeY = 3.0f; 
    static float left = -sizeX * 0.5f;
    static float top = sizeY * 0.5f;
    static float dx =  sizeX / float(this->image->getWidth());  
    static float dy =  sizeY / float(this->image->getHeight());

    return Vec3f(left + x * dx, top - y * dy, z).normalize();
}

void RayTracer::fireRays() {
    Ray ray;
    
    for(int x = 0; x < this->image->getWidth(); x++) {
        for(int y = 0; y < this->image->getHeight(); y++) {
            ray.d = RayTracer::getEyeRayDirection(x, y);

			Vec3f color = traceRay(ray, Ray::depth);
			this->image->setPixel(x, y, color);
        }
    }
}

Vec3f RayTracer::traceRay(const Ray& ray, int depth) {

	if (depth <= 0) {
		return bgColor;
	}

	HitRec hitRec;
	searchClosestHit(ray, hitRec);

	if (hitRec.anyHit) {
		Vec3f color = Vec3f(0.0f, 0.0f, 0.0f);
		Vec3f reflectedColor = Vec3f(0.0f, 0.0f, 0.0f);
		bool inShadow = false;

		for (const auto& light : this->scene->lights) {
			color += CalculateColor(ray, hitRec, light, inShadow);
		}

		/*if (inShadow) {
			return color;
		}*/
		
#if defined(REFLECTIONS)
		if (depth > 0) {
			reflectedColor = calculateReflection(ray, hitRec, depth);
		}
#endif
		Vec3f finalColor = color * (1.0f - hitRec.material->reflectivity) +
			reflectedColor * hitRec.material->reflectivity;

		finalColor.x = fminf(fmaxf(finalColor.x, 0.0f), 1.0f);
		finalColor.y = fminf(fmaxf(finalColor.y, 0.0f), 1.0f);
		finalColor.z = fminf(fmaxf(finalColor.z, 0.0f), 1.0f);
		return finalColor;
	}
	else {
		return bgColor;
	}

}


Vec3f RayTracer::CalculateColor(const Ray & ray, const HitRec & hitRec, const Light* light, bool & shadow) {
	
	Vec3f N = hitRec.n;
	N = N.normalize();
	Vec3f L = (light->position - hitRec.p).normalize();
	Vec3f V = (ray.o - hitRec.p).normalize();
	Vec3f R = (N * (2.0f * N.dot(L)) - L).normalize();

	Vec3f ambient = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f diffuse = Vec3f(0.0f, 0.0f, 0.0f);
	Vec3f specular = Vec3f(0.0f, 0.0f, 0.0f);

#if defined(AMBIENT_LIGHTING)
	//Ambient light
	ambient = hitRec.material->Ambient.multCoordwise(light->Ambient);
#endif


	//For shadow rays
	bool inShadow = false;

#if defined(SHADOWS_BLACK) || defined(SHADOWS_AMBIENT)

	Vec3f shadowOrigin = hitRec.p + N * Ray::rayEps; // Added N* Ray::rayEps to avoid self-shadowing and precision errors
	Ray shadowRay(shadowOrigin, L);
    shadowRay.tClip = (light->position - shadowOrigin).len();
	
	HitRec shadowHitRec;
	searchClosestHit(shadowRay, shadowHitRec);
    if (shadowHitRec.anyHit && shadowHitRec.tHit < shadowRay.tClip) {
        inShadow = true;
    }
#endif

	shadow = inShadow;

	bool skipDiffuseAndSpecular = false;

	if (inShadow) {
#if defined(SHADOWS_BLACK)
		skipDiffuseAndSpecular = true;
		ambient = Vec3f(0.0f, 0.0f, 0.0f);
#elif defined(SHADOWS_AMBIENT)
		skipDiffuseAndSpecular = true;	
#endif
	}



	if (!skipDiffuseAndSpecular) {

#if defined(DIFFUSE_LIGHTING)
		diffuse = hitRec.material->Diffuse.multCoordwise(light->Diffuse) * fmaxf(0.0f, N.dot(L));
#endif

#if defined(SPECULAR_LIGHTING)
		specular = hitRec.material->Specular.multCoordwise(light->Specular) * powf(fmaxf(0.0f, R.dot(V)), hitRec.material->Shininess);
#endif
	}


	Vec3f color = ambient*0.5+ambient;
	color = color + diffuse + specular;

	//Clamp color to [0,1]
	color.x = fminf(fmaxf(color.x, 0.0f), 1.0f);
	color.y = fminf(fmaxf(color.y, 0.0f), 1.0f);
	color.z = fminf(fmaxf(color.z, 0.0f), 1.0f);

	return color;
}


Vec3f RayTracer::calculateReflection(const Ray& ray, const HitRec& hitRec, int depth) {

	if (hitRec.material->reflectivity < 0.0f || hitRec.material->reflectivity>1.0f || depth<=0) {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

	Vec3f N = hitRec.n;
	N = N.normalize();
	Vec3f R = (ray.d - N * (2.0f * N.dot(ray.d))).normalize();

#ifdef FUZZY_NORMALS
	R = randomlyPerturbNormals(R, hitRec.material);
#endif

	Vec3f reflectionOrigin = hitRec.p + N * Ray::rayEps; // Added N* Ray::rayEps to avoid self-shadowing and precision errors
	Ray reflectionRay(reflectionOrigin, R);

	Vec3f reflectedColor = traceRay(reflectionRay, depth - 1);
}


Vec3f RayTracer::randomlyPerturbNormals(Vec3f reflectionDir, const Material* mat) {
	// Applying fuzziness
	float fuzz = mat->fuzziness;
	Vec3f randomVec = Vec3f(0.0f, 0.0f, 0.0f);

	if (fuzz > 0.0f) {
		randomVec = RayTracer::randomInUnitSphere();
	}

	return  (reflectionDir + randomVec * fuzz).normalize();
}