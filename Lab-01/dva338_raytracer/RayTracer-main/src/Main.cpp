#include "RayTracer.h"
#include "Vec3.h" 
#include "raytrace_features.h"

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
RayTracer* initTracer(int width, int height)
{
	RayTracer* rayTracer = new RayTracer(width, height);
	
	//Red
	rayTracer->addSphere(Vec3f(0.0f, 0.0f, -10.0f), 1.0f,
		new Material(
			Vec3f(0.1f, 0.0f, 0.0f),
			Vec3f(0.6f, 0.0f, 0.0f),
			Vec3f(0.6f, 0.6f, 0.6f),
			5000.0f, //shininess
			0.5f //reflectivity
	));

	//Green
	rayTracer->addSphere(Vec3f(2.0f, -0.2f, -11.0f), 0.8f,
		new Material(
			Vec3f(0.0f, 0.1f, 0.0f),   
			Vec3f(0.0f, 0.6f, 0.0f),  
			Vec3f(0.6f, 0.6f, 0.6f),  
			4000.0f, //shininess
			0.5f, //reflectivity
			0.05f //fuzziness
	));

	//yellow one for ask 2
	rayTracer->addSphere(Vec3f(-20.0f, 20.0f, -10.0f), 7.0f,
		new Material(
			Vec3f(0.6f, 0.6f, 0.0f),
			Vec3f(0.9f, 0.9f, 0.0f),
			Vec3f(0.5f, 0.5f, 0.5f),
			5000.0f, //shininess
			0.5f //reflectivity
	));

#ifdef TRIANGLES

	//Red Triangle
	rayTracer->addTriangle(
		Vec3f(-0.8f, 1.0f, -11.0f),
		Vec3f(-2.5f, 3.4f, -11.0f),
		Vec3f(-0.8f, 3.4f, -11.0f),
		new Material(
			Vec3f(0.2f, 0.0f, 0.0f),
			Vec3f(0.5f, 0.0f, 0.0f),
			Vec3f(0.2f, 0.2f, 0.2f),
			0.0f,
			0.6f
	));

	//Green Triangle
	rayTracer->addTriangle(
		Vec3f(-0.8f, 1.0f, -11.0f),
		Vec3f(0.9f, 3.4f, -11.0f),
		Vec3f(-0.8f, 3.4f, -11.0f),
		new Material(
			Vec3f(0.0f, 0.2f, 0.0f),
			Vec3f(0.0f, 0.5f, 0.0f),
			Vec3f(0.2f, 0.2f, 0.2f),
			0.0f,
			0.6f
	));

#endif // TRIANGLES


	//light
	rayTracer->addLight(Vec3f(-10.0f, 10.0f, 10.0f), 
		Vec3f(1.0f, 1.0f, 1.0f), 
		Vec3f(1.0f, 1.0f, 1.0f),
		Vec3f(1.0f, 1.0f, 1.0f));

	//plane
	/*rayTracer->addPlane(
		Vec3f(0.0f, -1.0f, 0.0f),      
		Vec3f(0.0f, 1.0f, 0.0f),     
		new Material(
			Vec3f(0.02f, 0.02f, 0.1f),
			Vec3f(0.05f, 0.5f, 0.25f),
			Vec3f(0.2f, 0.2f, 0.2f),
			16.0f,
			1.0f
		)
	);*/

	// Ground Sphere 
	const Material* blueGround = new Material(
		Vec3f(0.02f, 0.02f, 0.05f),  // ambient
		Vec3f(0.1f, 0.1f, 0.6f),     // diffuse
		Vec3f(0.0f, 0.0f, 0.0f),     // specular (matte surface)
		10.0f,                       // shininess
		1.0f                         // reflective
	);

	// Big sphere to simulate the ground
	rayTracer->addSphere(Vec3f(0.0f, -1001.0f, -10.0f), 1000.0f, blueGround);
	
	return rayTracer;
}

void runTracer(RayTracer* tracer)
{
	tracer->fireRays();
	tracer->toPPM("DVA338_Raytracer_output.ppm");
	tracer->toBMP("DVA338_Raytracer_output.bmp");
}

void destroyTracer(RayTracer* tracer)
{
	delete tracer;
}

int main(int argc, char **argv) {
	RayTracer* tracer = initTracer(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	runTracer(tracer);
	destroyTracer(tracer);
	return 0;
}
