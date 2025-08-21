#include "camera.h"
#include "math.h"

// Task-1.3
Matrix PerspectiveProjectionMatrix(Camera cam, int screenWidth, int screenHeight)
{

	// Assignment 1: Calculate the projection transform yourself 	
	// The matrix P should be calculated from camera parameters
	// Therefore, you need to replace this hard-coded transform.
	Matrix P = {0.0f};
	/*P.e[0] = 1.299038f; P.e[4] = 0.000000f; P.e[ 8] =  0.000000f; P.e[12] =  0.0f;
	P.e[1] = 0.000000f; P.e[5] = 1.732051f; P.e[ 9] =  0.000000f; P.e[13] =  0.0f;
	P.e[2] = 0.000000f; P.e[6] = 0.000000f; P.e[10] = -1.000200f; P.e[14] = -2.000200f;
	P.e[3] = 0.000000f; P.e[7] = 0.000000f; P.e[11] = -1.000000f; P.e[15] =  0.0f;*/
	float aspect = (float)screenWidth / (float)screenHeight;

	float fovRad = deg2rad(cam.fov);

	float contanFov = 1.0f / tan(fovRad / 2.0f);

	float far = cam.farPlane;
	float near = cam.nearPlane;

	P.e[0] = contanFov / aspect;
	P.e[5] = contanFov;
	P.e[10] = -(far + near) / (far - near);
	P.e[11] = -1.0f;
	P.e[14] = -(2.0f * far * near) / (far - near);
	P.e[1] = P.e[2] = P.e[3] = P.e[4] = P.e[6] = P.e[7] = P.e[8] = P.e[9] = 0.0f;
	P.e[12] = P.e[13] = 0.0f;
	P.e[15] = 0.0f;

	return P;
}

// Task-1.3
Matrix OrthogonalProjectionMatrix(float left, float right, float top, float bottom, float near, float far)
{
	Matrix P = { 0.0f };

	P.e[0] = 2.0f / (right - left);
	P.e[5] = 2.0f / (top - bottom);
	P.e[10] = 2.0f / (near - far);
	P.e[12] = -(right + left) / (right - left);
	P.e[13] = -(top + bottom) / (top - bottom);
	P.e[14] = -(far + near) / (far - near);
	P.e[15] = 1;

	return P;
}


//Interface function for view matrix
// Task-1.2
Matrix ViewMatrix(Camera cam)
{
	// Assignment 1: Calculate the transform to view coordinates yourself 	
	// The matrix V should be calculated from camera parameters
	// Therefore, you need to replace this hard-coded transform. 
	Matrix V;
	/*V.e[0] = 1.0f; V.e[4] = 0.0f; V.e[ 8] = 0.0f; V.e[12] =   0.0f;
	V.e[1] = 0.0f; V.e[5] = 1.0f; V.e[ 9] = 0.0f; V.e[13] =   0.0f;
	V.e[2] = 0.0f; V.e[6] = 0.0f; V.e[10] = 1.0f; V.e[14] = -cam.position.z;
	V.e[3] = 0.0f; V.e[7] = 0.0f; V.e[11] = 0.0f; V.e[15] =   1.0f;*/

	Matrix T = translation({ -cam.position.x, -cam.position.y, -cam.position.z });
	Matrix Rx = rotationAroundX(-cam.rotation.x);
	Matrix Ry = rotationAroundY(-cam.rotation.y);
	Matrix Rz = rotationAroundZ(-cam.rotation.z);

	V = MatMatMul(Rz, MatMatMul(Ry, MatMatMul(Rx, T)));

	return V;
}