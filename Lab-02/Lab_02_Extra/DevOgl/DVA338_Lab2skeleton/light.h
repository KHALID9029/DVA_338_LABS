#ifndef _Light_H_
#define _Light_H_
#include "algebra.h"


typedef struct _Light {
	Vector position;
	Vector ambient;
	Vector diffuse;
	Vector specular;
} Light;
#endif