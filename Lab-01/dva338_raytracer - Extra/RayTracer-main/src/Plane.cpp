#include "Plane.h"

bool Plane::hit(const Ray& r, HitRec& rec) const {
	float denom = normal.dot(r.d);

	// We can't divide by zero, so we check if the ray is parallel to the plane
	if (fabs(denom) > 1e-6) {

		//n*p = -D
		float D = -(normal.dot(point));
		//t = -(n*o + D) / (n*d)
		float t = -(normal.dot(r.o) + D) / denom;

		if (t >= 0 && t < rec.tHit && t < r.tClip) {
			rec.tHit = t;
			rec.anyHit = true;
			
			return true;
		}
	}
	return false;
}

const Material* Plane::getMaterial() const {

	return this->material;
}

void  Plane::computeSurfaceHitFields(const Ray& r, HitRec& rec) const {
	rec.p = r.o + r.d * rec.tHit;
	rec.n = normal;
}