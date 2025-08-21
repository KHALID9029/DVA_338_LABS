#ifndef _MESH_H_
#define _MESH_H_

#include "algebra.h"

typedef struct _Triangle {
	int vInds[3]; //vertex indices
} Triangle;

typedef struct _Mesh { 
	int nv;				
	Vector *vertices;
	Vector *vnorms;
	int nt;				
	Triangle *triangles;
	struct _Mesh *next; 

	// Transformations: Task 1.5
	Vector translation;
	Vector rotation;
	Vector scale;

	// Material Properties: Task 2.3
	Vector ambient;
	Vector diffuse;
	Vector specular;
	float shininess;

	// For task 3: identifying a object to apply animation
	int mesh_id;
		
	unsigned int vbo, ibo, vao, vboTarget; // OpenGL handles for rendering
} Mesh;

Mesh* insertModel(Mesh ** objlist, int nv, float * vArr, int nt, int * tArr, float scale = 1.0);

#endif
