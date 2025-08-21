#include "RayTracer.h"
#include "raytrace_features.h"

using namespace std;

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
	int samples = 10;

	for (int x = 0; x < this->image->getWidth(); x++) {
		for (int y = 0; y < this->image->getHeight(); y++) {

#ifdef ANTI_ALIASING
			Vec3f accumulatedColor(0.0f, 0.0f, 0.0f);

			// Supersampling: shoot multiple rays per pixel
			for (int i = 0; i < samples; i++) {
				// Generate random offsets to jitter the ray origin slightly
				float offsetX = (rand() / float(RAND_MAX)) - 0.5f;
				float offsetY = (rand() / float(RAND_MAX)) - 0.5f;

				// Get the ray direction using jittered offsets
				ray.d = getEyeRayDirection(x + offsetX, y + offsetY);

				// Trace the ray and accumulate the color
				accumulatedColor += traceRay(ray, Ray::depth);
			}

			// Average the accumulated color
			accumulatedColor *= (1.0f / float(samples));
			// Clamp the color to [0, 1]
			accumulatedColor.x = fminf(fmaxf(accumulatedColor.x, 0.0f), 1.0f);
			accumulatedColor.y = fminf(fmaxf(accumulatedColor.y, 0.0f), 1.0f);
			accumulatedColor.z = fminf(fmaxf(accumulatedColor.z, 0.0f), 1.0f);

			// Set the final pixel color
			this->image->setPixel(x, y, accumulatedColor);
#else
			ray.d = RayTracer::getEyeRayDirection(x, y);

			Vec3f  color = traceRay(ray, Ray::depth);
			this->image->setPixel(x, y, color);
#endif
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
		Vec3f refractedColor = Vec3f(0.0f, 0.0f, 0.0f);

		bool inShadow = false;

		for (const auto& light : this->scene->lights) {
			color += CalculateColor(ray, hitRec, light, inShadow);
		}

		
#if defined(REFLECTIONS)
		if (hitRec.material->reflectivity > 0.0f) {
			reflectedColor = calculateReflection(ray, hitRec, depth);
		}
#endif

#ifdef REFRACTIONS
		if (hitRec.material->transparency > 0.0f)
			refractedColor = calculateRefraction(ray, hitRec, depth);
#endif // 


		//Final Color
		float reflectivity = hitRec.material->reflectivity;
		float transparency = hitRec.material->transparency;
		float base = 1.0f - reflectivity - transparency;

		Vec3f finalColor = color * base +
			reflectedColor * reflectivity + refractedColor * transparency;

		//Clamp color to [0,1]
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

#ifdef SOFT_SHADOWS   
	// Soft shadow factor
	float shadowFactor = computeShadowFactor(hitRec, light);
	inShadow = (shadowFactor < 1.0f);
#endif

	shadow = inShadow;
	bool skipDiffuseAndSpecular = false;

#ifdef SOFT_SHADOWS 
	if (inShadow && shadowFactor <= 0.01f) {
#if defined(SHADOWS_BLACK)
		skipDiffuseAndSpecular = true;
		ambient = Vec3f(0.0f, 0.0f, 0.0f);
#elif defined(SHADOWS_AMBIENT)
		skipDiffuseAndSpecular = true;
#endif
	}
#else
	if (inShadow) {
#if defined(SHADOWS_BLACK)
		skipDiffuseAndSpecular = true;
		ambient = Vec3f(0.0f, 0.0f, 0.0f);
#elif defined(SHADOWS_AMBIENT)
		skipDiffuseAndSpecular = true;
#endif
	}
#endif 

	


	if (!skipDiffuseAndSpecular) {

#if defined(DIFFUSE_LIGHTING)
		diffuse = hitRec.material->Diffuse.multCoordwise(light->Diffuse) * fmaxf(0.0f, N.dot(L));
#endif

#if defined(SPECULAR_LIGHTING)
		specular = hitRec.material->Specular.multCoordwise(light->Specular) * powf(fmaxf(0.0f, R.dot(V)), hitRec.material->Shininess);
#endif
	}


	Vec3f color = ambient*0.5+ambient;
#ifdef SOFT_SHADOWS
	color = ambient + (diffuse + specular) * shadowFactor;
#else
	color = ambient + diffuse + specular;
#endif

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



Vec3f RayTracer::calculateRefraction(const Ray& ray, HitRec& hitRec, int depth) {

	Vec3f N = hitRec.n;
	float refractiveIndex = hitRec.material->refractiveIndex;
	float cosine = -max(-1.0f, min(1.0f, ray.d.dot(N))); //cosine of the angle between the incoming ray direction and the normal.
														 // The clamp ensures the value is between -1 and 1 for safety.

	float curRefIndex = 1.0f, matRefIndex = refractiveIndex; //assuming the current medium is air (n=1.0)

	if (cosine < 0.0f) { // ray is exiting the material
		cosine = -cosine;
		swap(curRefIndex, matRefIndex);
		N = -N;
	}

	// Snell's law -- n1sin(theta1) = n2sin(theta2)
	float etaRatio = curRefIndex / matRefIndex;
	float k = 1.0f - etaRatio * etaRatio * (1.0f - cosine * cosine); //discriminat for refraction direction

	if (k < 0.0f) {
		// Total internal reflection--no refraction is possible
		return Vec3f(0.0f, 0.0f, 0.0f);
	}
	else {
		Vec3f refractDir = (ray.d * etaRatio + N * (etaRatio * cosine - sqrtf(k))).normalize(); //Snell's law vector form
		Ray refractedRay(hitRec.p - N * Ray::rayEps, refractDir);  // Offset slightly into the surface
		return traceRay(refractedRay, depth - 1);
	}
}


float RayTracer::computeShadowFactor(const HitRec& hitRec, const Light* light) {

	int hits = 0;
	int total = light->samples;
	Vec3f N = hitRec.n;

	for (int i = 0; i < total; i++) {
		Vec3f jitteredPos = light->position + randomInUnitSphere() * light->radius; //Randomize the light position inside a sphere to simulate area light.
		Vec3f L = (jitteredPos - hitRec.p).normalize();

		Vec3f shadowOrigin = hitRec.p + N * Ray::rayEps;
		Ray shadowRay(shadowOrigin, L);
		shadowRay.tClip = (jitteredPos - shadowOrigin).len();

		HitRec shadowHitRec;
		searchClosestHit(shadowRay, shadowHitRec);

		if (shadowHitRec.anyHit && shadowHitRec.tHit < shadowRay.tClip) {
			hits++;
		}
	}

	return 1.0f - (float)hits / total;
}