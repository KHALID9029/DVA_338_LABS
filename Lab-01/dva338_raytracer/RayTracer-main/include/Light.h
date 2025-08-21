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

	Light(const Vec3f& pos, const Vec3f& ambient, const Vec3f& diffuse, Vec3f& specular)
		: position(pos), Ambient(ambient), Diffuse(diffuse), Specular(specular) {
	}

	/*Vec3f getColor() const {
		return Color;
	}*/
};

#endif // !_LIGHT_H_