#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Vec3.h"
#include "Object.h"

class Light {
public:
	Vec3f position;
	Vec3f Ambient;
	Vec3f Diffuse;
	Vec3f Specular;

	// --- New Soft Shadow Support ---
	float radius;      // Radius for soft shadow (area light)
	int samples;       // Number of shadow rays for soft shadow sampling


	Light(const Vec3f& pos, const Vec3f& amb, const Vec3f& diff, const Vec3f& spec,
		float rad = 0.0f, int samp = 1)
		: position(pos), Ambient(amb), Diffuse(diff), Specular(spec), radius(rad), samples(samp) {
	}

	/*Vec3f getColor() const {
		return Color;
	}*/
};

#endif // !_LIGHT_H_