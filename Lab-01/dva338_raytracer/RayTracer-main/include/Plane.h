#ifndef _PLANE_H_
#define _PLANE_H_

#include "Object.h"
#include "Vec3.h"
#include "Material.h"
#include "Ray.h"

class Plane : public Object {

public:
	Vec3f point;
	Vec3f normal;
	const Material* material;

	Plane(const Vec3f& p, const Vec3f& n, const Material* mat) 
		: point(p), normal(n), material(mat) {
		normal=normal.normalize();
	}

	bool hit(const Ray& ray, HitRec& rec) const;

	const Material* getMaterial() const;

	void  computeSurfaceHitFields(const Ray& r, HitRec& rec) const;

};
#endif // !_PLANE_H_