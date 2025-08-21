#ifndef _RAY_H_
#define _RAY_H_

#include "Vec3.h"
#include "Material.h"
typedef struct {
	float tHit;
	int primIndex;
	Vec3f p;
	Vec3f n;
	bool anyHit;
	const Material* material;
} HitRec;

class Ray {
public:
	Vec3f o, d;
	float tClip;	
	static float rayEps;
	static float tMax;
	static int depth;
	HitRec hitRec;
public:
	//Default constructor: Set the start position of the eye rays to the origin
	Ray() : tClip(tMax), o(Vec3f(0.0f, 0.0f, 0.0f)) {}

	Ray(const Vec3f & o, const Vec3f& d, float tclip = tMax) : o(o), d(d), tClip(tMax) {}
	
	void epsMoveStartAlongSurfaceNormal(const Vec3f & n) { o = o + n * rayEps; }
	void epsMoveStartAlongDir() { o = o + d * rayEps; }
	
	static void incRayEps() { rayEps *= 10.0f; }
	static void decRayEps() { rayEps /= 10.0f; }
};
#endif
