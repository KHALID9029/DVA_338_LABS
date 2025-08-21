
#include "Sphere.h"


bool Sphere::hit(const Ray & r, HitRec & rec) const {	
	
	//Calculation for finding t
	Vec3f oc = r.o - c;
	float A = r.d.dot(r.d);
	float B = 2.0f * oc.dot(r.d);
	float C = oc.dot(oc) - (this->r * this->r);

	float D = pow(B,2) - (4.0f * A * C);

	if (D < 0) {
		return false;
	}

	float sqrtD = sqrtf(D);
	float t1 = (-B - sqrtD) / (2.0f * A);
	float t2 = (-B + sqrtD) / (2.0f * A);

	float tHit = rec.tHit;

	// If both zero, hitting behind the ray
	if (t1 < 0 && t2 < 0) {
		return false;
	}

	// If both positive, hitting in front of the ray, we take the closest
	if (t1 > 0 && t2 > 0) {
		tHit = fminf(t1, t2);
	}

	// If one is negative and the other positive, we take the positive one
	if (t1 < 0 || t2 < 0) {
		tHit = fmaxf(t1, t2);
	}

	// If tHit is greater than the clipping distance or the closest hit so far, we ignore it
	if (tHit > r.tClip || (rec.anyHit && tHit > rec.tHit)) {
		return false;
	}

	rec.tHit = tHit;
	rec.anyHit = true;

	//rec.material = Sphere::getMaterial(); --Not very efficient so assign the material only after the best hit point is fixed

	Sphere::computeSurfaceHitFields(r, rec);

	return true;
}


void Sphere::computeSurfaceHitFields(const Ray & r, HitRec & rec) const {
	rec.p = r.o + r.d * rec.tHit;
	rec.n = (rec.p - c).normalize();
}


const Material* Sphere::getMaterial() const {
	return material;
}