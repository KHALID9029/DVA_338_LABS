#include "Triangle.h"

bool Triangle::hit(const Ray& ray, HitRec& hitRec) const {
	float denom = n.dot(ray.d);
	if (fabs(denom) < 1e-6) {
		return false; // Ray is parallel to the triangle
	}

	float D = -(n.dot(v0));
	float t = -(n.dot(ray.o) + D) / denom;

	if (t < 1e-14 || t >= ray.tClip) {
		return false; // Intersection is behind the ray or beyond the clipping distance
	}

	Vec3f p = ray.o + ray.d * t;
	Vec3f edge0 = v1 - v0;
	Vec3f edge1 = v2 - v1;
	Vec3f edge2 = v0 - v2;

	Vec3f c0 = edge0.cross(p - v0);
	Vec3f c1 = edge1.cross(p - v1);
	Vec3f c2 = edge2.cross(p - v2);

	// Check if the point is inside the triangle using the normal vector
	if (n.dot(c0) >= 0 && n.dot(c1) >= 0 && n.dot(c2) >= 0) {
		hitRec.tHit = t;
		hitRec.anyHit = true;
		
		return true;
	}
	return false; // Point is outside the triangle
}

const Material* Triangle::getMaterial() const {
	return material;
}

void Triangle::computeSurfaceHitFields(const Ray& r, HitRec& rec) const {
	Vec3f p = r.o + r.d * rec.tHit;
	rec.n = n;
	rec.p = p;
}