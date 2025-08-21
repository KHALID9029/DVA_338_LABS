#include <stdlib.h>
#include "mesh.h"

Mesh* insertModel(Mesh** list, int nv, float* vArr, int nt, int* tArr, float scale) {
	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
	mesh->nv = nv;
	mesh->nt = nt;
	mesh->vertices = (Vector*)malloc(nv * sizeof(Vector));
	mesh->vnorms = (Vector*)malloc(nv * sizeof(Vector));
	mesh->triangles = (Triangle*)malloc(nt * sizeof(Triangle));

	//Initial transformations
	mesh->translation = { 0.0f, 0.0f, 0.0f };
	mesh->rotation = { 0.0f, 0.0f, 0.0f };
	mesh->scale = { 1.0f, 1.0f, 1.0f };

	// Bujha lagbe
	// Initialize material properties
	mesh->ambient = { 0.5f, 0.5f, 0.5f };
	mesh->diffuse = { 0.8f, 0.8f, 0.8f };
	mesh->specular = { 1.0f, 1.0f, 1.0f };
	mesh->shininess = 32.0f;

	// set mesh vertices
	for (int i = 0; i < nv; i++) {
		mesh->vertices[i].x = vArr[i * 3] * scale;
		mesh->vertices[i].y = vArr[i * 3 + 1] * scale;
		mesh->vertices[i].z = vArr[i * 3 + 2] * scale;
	}

	// set mesh triangles
	for (int i = 0; i < nt; i++) {
		mesh->triangles[i].vInds[0] = tArr[i * 3];
		mesh->triangles[i].vInds[1] = tArr[i * 3 + 1];
		mesh->triangles[i].vInds[2] = tArr[i * 3 + 2];
	}

	// Assignment 1: 
	// Calculate and store suitable vertex normals for the mesh here.
	// Replace the code below that simply sets some arbitrary normal values	
	// Hint: You need to fix the implementation of the SurfaceNormal function
	// and use that for normal calculations.
	//Vector ___tmp = {0.80078125f, 0.34765625f, 0.1796875f};

	for (int i = 0; i < nt;i++) {
		Triangle t = mesh->triangles[i];

		Vector x = mesh->vertices[t.vInds[0]];
		Vector y = mesh->vertices[t.vInds[1]];
		Vector z = mesh->vertices[t.vInds[2]];

		Vector n = SurfaceNormal(x, y, z);

		mesh->vnorms[t.vInds[0]] = Add(mesh->vnorms[t.vInds[0]], n);
		mesh->vnorms[t.vInds[1]] = Add(mesh->vnorms[t.vInds[1]], n);
		mesh->vnorms[t.vInds[2]] = Add(mesh->vnorms[t.vInds[2]], n);
	}
	for (int i = 0; i < nv; i++) {
		mesh->vnorms[i] = Normalize(mesh->vnorms[i]);
	}
	mesh->next = *list;
	*list = mesh;

	return mesh;
}
