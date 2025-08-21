//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	//If you follow the setup instructions provided for Visual Studio
	//The include path specified already includes "GL", so we can ignore it
	#include <glew.h>
	#include <freeglut.h>
#elif defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
	#include <GL/glew.h>
	#include <GLUT/glut.h>
#else
	#include <GL/glew.h>
	#include <GL/freeglut.h>
#endif
#include "camera.h"	//algebra is included in camera.h
#include "shaders.h"
#include "mesh.h"
#include "light.h"
#include <string>
#include <iostream>
#include <fstream>

#define M_PI 3.14159265358979323846
#define part2
using namespace std;


// Position, Rotation, FOV, nearplane, farplane, isOrthogonal
#ifdef part2
Camera cam = { {0,0,20}, {0,0,0}, 60, 1, 10000, false };
#else
// Camera for task 3
Camera cam = { {5,7,20}, {-13,13,0}, 60, 1, 10000, false };
#endif


// Task 1.3
// Orthogonal parameters
float left_value = -20;
float right_value = 20;
float top = 10;
float bottom = -10;
float nearP = cam.nearPlane;
float farP = cam.farPlane;

int screen_width = 1024;
int screen_height = 768;

Mesh *meshList = NULL; // Global pointer to linked list of triangle meshes

// Shader programs
GLuint basicShader, interlacedShader, phongShader, cartoonShader,
		flatShader, phongMultiLightShader, cartoonMultiLightShader, 
		morphShader, wireframeShader, currentShader;
float morphFactor = 0.0f;
int windowHandle;

// Light
// position, ambient, diffuse, specular
Light light1 = {
	{-20.0f, 10, 10} ,
	{ 0.2f, 0.2f, 0.2f },
	{ 0.9f, 0.9f, 0.7f },
	{ 1.0f, 1.0f, 1.0f }
};

Light light2 = {
	{20.0f, 10.0f, -10.0f} ,
	{0.2f,0.2f,0.2f},
	{0.9f, 0.9f, 0.7f},
	{1.0f,1.0f,1.0f}
};

Light lights[2] = { light1, light2 };

Vector viewPos = cam.position;

// LAB-03
InstanceRenderer cadillacRenderer;

// Global transform matrices
// V is the view transform
// P is the projection transform
// PV = P * V is the combined view-projection transform
Matrix V, P, PV;

// Task-2.1
string loadShaderFile(const string& filename) {
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Could not open file: " << filename << endl;
		return "";
	}

	//check
	return string((istreambuf_iterator<char>(file)),
		istreambuf_iterator<char>());
}


void printShaderErrorInfo(GLuint shaderProgram)
{
	GLint maxLength = 0;
	glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	GLchar * errorLog = (GLchar*)calloc(maxLength, sizeof(GLchar));
	glGetShaderInfoLog(shaderProgram, maxLength, &maxLength, &errorLog[0]);

	// Provide the infolog in whatever manor you deem best.
	printf("%s\n", errorLog);
	printf("\n");
	free(errorLog);
}

GLuint prepareShaderProgram(const string& vsPath, const string fsPath) {

	string vsSource = loadShaderFile(vsPath);
	string fsSource = loadShaderFile(fsPath);

	if (vsSource.empty()) vsSource = vs_n2c_src;
	if (fsSource.empty()) fsSource = fs_ci_src;

	const char* vs_src = vsSource.c_str();
	const char* fs_src = fsSource.c_str();

	GLint success = GL_FALSE;

	GLuint program = glCreateProgram();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_src, NULL); // shader, string count, string, length
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);	
	if (!success) printf("Error in vertex shader!\n");
	else printf("Vertex shader compiled successfully!\n");
	if(success == GL_FALSE)
	{
		printShaderErrorInfo(vs);
		glDeleteShader(vs); // Don't leak the shader.
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_src, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success) printf("Error in fragment shader!\n");
	else printf("Fragment shader compiled successfully!\n");
	if(success == GL_FALSE)
	{
		printShaderErrorInfo(fs);
		glDeleteShader(fs); // Don't leak the shader.
	}

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	GLint isLinked = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (!isLinked) printf("Link error in shader program!\n");
	else printf("Shader program linked successfully!\n");

	return program;
}

void initShaders() {
	basicShader = prepareShaderProgram("Basic.vert", "Basic.frag");
	interlacedShader = prepareShaderProgram("Interlaced.vert", "Interlaced.frag");
	phongShader = prepareShaderProgram("Phong.vert", "Phong.frag");
	cartoonShader = prepareShaderProgram("Cartoon.vert", "Cartoon.frag");
	flatShader = prepareShaderProgram("FlatShader.vert", "FlatShader.frag");
	phongMultiLightShader = prepareShaderProgram("Phong.vert", "PhongMultiLight.frag");
	morphShader = prepareShaderProgram("Morph.vert", "Morph.frag");
	cartoonMultiLightShader = prepareShaderProgram("Cartoon.vert", "CartoonMultiLight.frag");
	//wireframeShader = prepareShaderProgram("Wirefram.vert", "Wirefram.frag");

	if (!basicShader || !interlacedShader || !phongShader || !cartoonShader || !flatShader || !phongMultiLightShader || !morphShader || !cartoonMultiLightShader) {
		cerr << "Failed to prepare shader program!" << endl;
		exit(EXIT_FAILURE);
	}

	/*if (basicShader) {
		cerr << "Basic shader program created successfully!" << endl;
	}
	if (interlacedShader) {
		cerr << "Interlaced shader program created successfully!" << endl;
	}*/

	currentShader = phongShader;
}

void prepareMesh(Mesh *mesh) {
	int sizeVerts = mesh->nv * 3 * sizeof(float);
	int sizeCols  = mesh->nv * 3 * sizeof(float);
	int sizeTris = mesh->nt * 3 * sizeof(int);

	// VBO -- For storage of vertex data
	// IBO -- For storage of index data -- in what order to access VBO
	// VAO -- Vertex Array Object -- how to interpret data
	
	// For storage of state and other buffer objects needed for vertex specification
	// setting up a vao
	glGenVertexArrays(1, &mesh->vao); //#of obj, where to store id
	glBindVertexArray(mesh->vao);

	// Allocate VBO and load mesh data (vertices and normals)
	glGenBuffers(1, &mesh->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts + sizeCols, NULL, GL_STATIC_DRAW); //target_buffer, size, initial data, hint to GPU
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVerts, (void*)mesh->vertices); // target_buffer, offset, size, data
	glBufferSubData(GL_ARRAY_BUFFER, sizeVerts, sizeCols, (void *)mesh->vnorms);

	// Allocate index buffer and load mesh indices
	glGenBuffers(1, &mesh->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeTris, (void *)mesh->triangles, GL_STATIC_DRAW);

	// Define the format of the vertex data
	GLint vPos = glGetAttribLocation(currentShader, "vPos");
	glEnableVertexAttribArray(vPos);
	glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, NULL); // attribute_index, #of components per vertex, data_type, don't normalize, stride, offset at beginning

	// Define the format of the vertex data 
	GLint vNorm = glGetAttribLocation(currentShader, "vNorm");
	glEnableVertexAttribArray(vNorm);
	glVertexAttribPointer(vNorm, 3, GL_FLOAT, GL_FALSE, 0, (void *)(mesh->nv * 3 *sizeof(float)));


	if (mesh->vboTarget) {
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vboTarget);
		GLint vPosTarget = glGetAttribLocation(currentShader, "vPosTarget");
		if (vPosTarget >= 0) {
			glEnableVertexAttribArray(vPosTarget);
			glVertexAttribPointer(vPosTarget, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		}
	}

	glBindVertexArray(0);

}

void renderMesh(Mesh *mesh) {
	
	// Assignment TODO: Apply the transforms from local mesh coordinates to world coordinates here
	// Combine it with the viewing transform that is passed to the shader below
	
	//Local mesh transform to view coordinates -- Task 1.5
	Matrix S = scaling({ mesh->scale.x, mesh->scale.y, mesh->scale.z });
	Matrix R = rotationAroundZ(mesh->rotation.z);
	R = MatMatMul(rotationAroundY(mesh->rotation.y),R);
	R = MatMatMul(rotationAroundX(mesh->rotation.x),R);

	Matrix T = translation({ mesh->translation.x, mesh->translation.y, mesh->translation.z });
	Matrix W = MatMatMul(T, MatMatMul(R, S));
	Matrix M = MatMatMul(V, W);
	Matrix PVM = MatMatMul(P, M);

	glUseProgram(currentShader); // CPU initiates, GPU executes, anything before this is done on CPU

	// Pass the viewing transform to the shader
	GLint loc_PV = glGetUniformLocation(currentShader, "PV");
	//glUniformMatrix4fv(loc_PV, 1, GL_FALSE, PV.e);
	glUniformMatrix4fv(loc_PV, 1, GL_FALSE, PVM.e); // For task 1.5

	// basic and interlaced shaders does not need those
	if (currentShader == phongShader || currentShader == cartoonShader || currentShader == flatShader
		|| currentShader == phongMultiLightShader || currentShader == morphShader || currentShader == cartoonMultiLightShader) 
	{
		GLint loc_M = glGetUniformLocation(currentShader, "M");
		glUniformMatrix4fv(loc_M, 1, GL_FALSE, M.e);

		//passing light for single light source
		if (currentShader == phongShader || currentShader == cartoonShader || currentShader == flatShader
			|| currentShader == morphShader) 
		{
			//passing light
			GLint loc_lightPos = glGetUniformLocation(currentShader, "lightPos");
			glUniform3f(loc_lightPos, light1.position.x, light1.position.y, light1.position.z);

			GLint loc_lightAmbient = glGetUniformLocation(currentShader, "lightAmbient");
			glUniform3f(loc_lightAmbient, light1.ambient.x, light1.ambient.y, light1.ambient.z);

			GLint loc_lightDiffuse = glGetUniformLocation(currentShader, "lightDiffuse");
			glUniform3f(loc_lightDiffuse, light1.diffuse.x, light1.diffuse.y, light1.diffuse.z);

			GLint loc_lightSpecular = glGetUniformLocation(currentShader, "lightSpecular");
			glUniform3f(loc_lightSpecular, light1.specular.x, light1.specular.y, light1.specular.z);
		}

		//passing cam position
		GLint loc_viewPos = glGetUniformLocation(currentShader, "viewPos");
		glUniform3f(loc_viewPos, cam.position.x, cam.position.y, cam.position.z);

		//passing material properties-- cartoon only needs diffuse
		if (currentShader == phongShader || currentShader == flatShader || currentShader == phongMultiLightShader || currentShader == morphShader) 
		{
			GLint loc_matAmbient = glGetUniformLocation(currentShader, "matAmbient");
			glUniform3f(loc_matAmbient, mesh->ambient.x, mesh->ambient.y, mesh->ambient.z);

			GLint loc_matDiffuse = glGetUniformLocation(currentShader, "matDiffuse");
			glUniform3f(loc_matDiffuse, mesh->diffuse.x, mesh->diffuse.y, mesh->diffuse.z);

			GLint loc_matSpecular = glGetUniformLocation(currentShader, "matSpecular");
			glUniform3f(loc_matSpecular, mesh->specular.x, mesh->specular.y, mesh->specular.z);

			GLint loc_matShininess = glGetUniformLocation(currentShader, "matShininess");
			glUniform1f(loc_matShininess, mesh->shininess);

			// Morph shader additionally needs the morph factor
			if (currentShader == morphShader) {
				GLint loc_morphFactor = glGetUniformLocation(currentShader, "morphFactor");
				if (loc_morphFactor >= 0) {
					glUniform1f(loc_morphFactor, morphFactor);
				}
			}
		}
		else if (currentShader == cartoonShader || currentShader == cartoonMultiLightShader) {
			GLint loc_matDiffuse = glGetUniformLocation(currentShader, "matDiffuse");
			glUniform3f(loc_matDiffuse, mesh->diffuse.x, mesh->diffuse.y, mesh->diffuse.z);

			if (currentShader == cartoonMultiLightShader) {
				for (int i = 0; i < 2; i++) {
					char buf[32];
					sprintf_s(buf, sizeof(buf), "lightPos[%d]", i);
					glUniform3f(glGetUniformLocation(currentShader, buf), lights[i].position.x, lights[i].position.y, lights[i].position.z);
				}
			}
		}

		// Multiple light sources
		if (currentShader == phongMultiLightShader) {
			for (int i = 0; i < 2; i++) {
				char buf[32];
				sprintf_s(buf, sizeof(buf), "lightPos[%d]", i);
				// get location of lightPos[0] and pass 3 float values
				glUniform3f(glGetUniformLocation(currentShader, buf), lights[i].position.x, lights[i].position.y, lights[i].position.z);
				sprintf_s(buf, sizeof(buf), "lightAmbient[%d]", i);
				glUniform3f(glGetUniformLocation(currentShader, buf), lights[i].ambient.x, lights[i].ambient.y, lights[i].ambient.z);
				sprintf_s(buf, sizeof(buf), "lightDiffuse[%d]", i);
				glUniform3f(glGetUniformLocation(currentShader, buf), lights[i].diffuse.x, lights[i].diffuse.y, lights[i].diffuse.z);
				sprintf_s(buf, sizeof(buf), "lightSpecular[%d]", i);
				glUniform3f(glGetUniformLocation(currentShader, buf), lights[i].specular.x, lights[i].specular.y, lights[i].specular.z);
			}
		}
	}

	// Select current resources 
	glBindVertexArray(mesh->vao);

	// Draw all triangles
	glDrawElements(GL_TRIANGLES, mesh->nt * 3, GL_UNSIGNED_INT, NULL); 
	glBindVertexArray(0);
}



void display(void) {
	Mesh *mesh;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	Assignment TODO: The implementation of the View- and ProjectionMatrix functions in camera.cpp 
	returns hardcoded matrices. Your assignment is to fix them.
	*/
	V = ViewMatrix(cam);

	if (cam.isOrthogonal) {
		P = OrthogonalProjectionMatrix(left_value, right_value, top, bottom, nearP, farP);
	}
	else {
		P = PerspectiveProjectionMatrix(cam, screen_width, screen_height);
	}
	
	
	// This finds the combined view-projection matrix
	PV = MatMatMul(P, V);

	// Select the shader program to be used during rendering 
	//glUseProgram(currentShader);

	// Render all meshes in the scene
	mesh = meshList;
		
	while (mesh != NULL) {
		renderMesh(mesh);
		mesh = mesh->next;
	}

	glFlush();
}

void changeSize(int w, int h) {
	screen_width = w;
	screen_height = h;
	glViewport(0, 0, screen_width, screen_height);

}
void cleanUp(void);

void keypress(unsigned char key, int x, int y) {

	float step = 0.2f;
	float angleStep = 20.0f * (float) M_PI / 180.0f;

	switch(key) {

	// Camera movement
	case 'x': cam.position.x -= step; break;
	case 'X': cam.position.x += step; break;
	case 'y': cam.position.y -= step; break;
	case 'Y': cam.position.y += step; break;
	case 'z': cam.position.z -= step; break;
	case 'Z': cam.position.z += step; break;

	// Camera rotation
	case 'i': cam.rotation.x -= angleStep; break;
	case 'I': cam.rotation.x += angleStep; break;
	case 'j': cam.rotation.y -= angleStep; break;
	case 'J': cam.rotation.y += angleStep; break;
	case 'k': cam.rotation.z -= angleStep; break;
	case 'K': cam.rotation.z += angleStep; break;

	case 'p':
		cam.isOrthogonal = !cam.isOrthogonal;
		break;

	// change the shader programme
	case '1': currentShader = basicShader; break;
	case '2': currentShader = interlacedShader; break;
	case '3': currentShader = phongShader; break;
	case '4': currentShader = cartoonShader; break;
	case '5': currentShader = phongMultiLightShader; break;
	case '6': currentShader = cartoonMultiLightShader; break;
	case '7': currentShader = flatShader; break;
	case '8': currentShader = morphShader; break;

	#ifdef __APPLE__
		// MAC OS GLUT does not implement LeaveMainLoop().
		cleanUp();
		exit(0);
	#else
		glutLeaveMainLoop();
	#endif
		break;
	}
	glutPostRedisplay();
}


void loadObjects() {

	if (!cadillacRenderer.loadMasterObject("../Cadillac/Cadillac.obj")) {
		cerr << "Failed to load Cadillac model\n";
		exit(EXIT_FAILURE);
	}
}


void init(void) {

	glEnable(GL_DEPTH_TEST);
	// Compile and link the given shader program (vertex shader and fragment shader)
	initShaders();

	loadObjects();

	// Setup OpenGL buffers for rendering of the meshes
	Mesh * mesh = meshList;
	while (mesh != NULL) {
		prepareMesh(mesh);
		mesh = mesh->next;
	}	
}

void cleanUp(void) {	
	printf("Running cleanUp function... ");
	// Free openGL resources
	// ...
	// Free meshes
	// ...
	Mesh* mesh = meshList;
	while (mesh) {
		Mesh* next = mesh->next;
		free(mesh->vertices);
		free(mesh->vnorms);
		free(mesh->triangles);
		free(mesh);
		mesh = next;
	}

	glDeleteProgram(basicShader);
	glDeleteProgram(interlacedShader);
	glDeleteProgram(phongShader);
	glDeleteProgram(cartoonShader);

	glutDestroyWindow(windowHandle);
	
	printf("Done!\n\n");
}


//Task 3
void timerCallback(int value) {
	cout << "Tick-Tock : " << value << endl;

	// To keep it periodic
	glutTimerFunc(1000, timerCallback, value + 1);
}

void animation(int value) {
	Mesh* current = meshList;

	while (current) {
		if (current->mesh_id == 2) {
			current->rotation.y += 2.0f;
			break;
		}
		current = current->next;
	}

	morphFactor = (sin(glutGet(GLUT_ELAPSED_TIME) * 0.001) + 1.0) / 2.0;
	glutPostRedisplay();
	glutTimerFunc(1000 / 50, animation, value + 1); // 50 FPS
}

// Include data for some triangle meshes (hard coded in struct variables)
#include "./models/mesh_bunny.h"
#include "./models/mesh_cow.h"
#include "./models/mesh_cube.h"
#include "./models/mesh_frog.h"
#include "./models/mesh_knot.h"
#include "./models/mesh_sphere.h"
#include "./models/mesh_teapot.h"
#include "./models/mesh_triceratops.h"


int main(int argc, char **argv) {
	
	// Setup freeGLUT
	unsigned int _glut_mode = GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH;
	glutInitDisplayMode(_glut_mode);
	glutInit(&argc, argv);
	glutInitWindowSize(screen_width, screen_height);
	windowHandle = glutCreateWindow("DVA338 Programming Assignments");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keypress);
	//This is a freeglut define value. In MacOS GLUT, these functions do not exist.
	#ifdef GLUT_CORE_PROFILE 
  	glutInitContextVersion(4, 1);
  	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	#endif
	
	glewExperimental = GL_TRUE;
	// Uses GLEW as OpenGL Loading Library to get access to modern core features as well as extensions
	GLenum err = glewInit(); 
	if (GLEW_OK != err) { fprintf(stdout, "Error: %s\n", glewGetErrorString(err)); return 1; }

	// Output OpenGL version info
	fprintf(stdout, "GLEW version: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "OpenGL version: %s\n", (const char *)glGetString(GL_VERSION));
	fprintf(stdout, "OpenGL vendor: %s\n\n", glGetString(GL_VENDOR));

	
	
	init();

	// Timer
	glutTimerFunc(1000, timerCallback, 0);
	glutTimerFunc(1000 / 50, animation, 0); // 50 FPS

	glutMainLoop();

	cleanUp();	
	return 0;
}
