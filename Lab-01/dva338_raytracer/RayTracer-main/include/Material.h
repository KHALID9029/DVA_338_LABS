#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Vec3.h"

class Material {
public:
	Vec3f Ambient;
	Vec3f Diffuse;
	Vec3f Specular;
	float Shininess;
	float reflectivity;
	float fuzziness;
	

public: 
	Material(Vec3f & ambient, Vec3f & diffuse, Vec3f & specular, const float shine, const float reflectivity=0.0f, float fuzz=0.0f)
		: Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shine), reflectivity(reflectivity),
		  fuzziness(fuzz) {
	}

	/*Vec3f getColor() const {
		return Vec3f(R, G, B);
	}*/
};

#endif // !_MATERIAL_H_