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

	//yellow one for task 2
	rayTracer->addSphere(Vec3f(-20.0f, 20.0f, -10.0f), 7.0f,
		new Material(
			Vec3f(0.6f, 0.6f, 0.0f),
			Vec3f(0.9f, 0.9f, 0.0f),
			Vec3f(0.5f, 0.5f, 0.5f),
			5000.0f, //shininess
			0.5f //reflectivity
	));

#ifdef REFRACTIONS
	// Glass-like sphere in front of red sphere -- for refraction test
	rayTracer->addSphere(Vec3f(-0.5f, -0.05f, -5.0f), 0.5f, new Material(
		Vec3f(0.0f, 0.0f, 0.0f),    // ambient (glass doesn't emit light)
		Vec3f(0.1f, 0.1f, 0.1f),    // diffuse (low)
		Vec3f(0.9f, 0.9f, 0.9f),    // specular (highlights)
		10000.0f,                   // shininess
		0.05f,                      // reflectivity (glass has some)
		0.0f,                       // fuzziness (none - clean reflection)
		1.0f,                       // transparency (fully transparent)
		1.52f                       // refractive index for glass
	));
#endif // 


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


	// Main white light from the top-left (slightly soft shadow)
	rayTracer->addLight(
		Vec3f(-10.0f, 5.0f, 10.0f),
		Vec3f(1.0f, 1.0f, 1.0f),
		Vec3f(1.0f, 1.0f, 1.0f),
		Vec3f(1.0f, 1.0f, 1.0f),
		1.0f, 100
	);

	// Warm orange light from the front-right (soft shadow)
	rayTracer->addLight(
		Vec3f(3.0f, 5.0f, 10.0f),
		Vec3f(1.0f, 1.0f, 1.0f),
		Vec3f(1.0f, 1.0f, 1.0f),
		Vec3f(1.0f, 1.0f, 1.0f),
		0.2f, 10
	);

	//// Cool bluish overhead fill light (very soft)
	//rayTracer->addLight(
	//	Vec3f(0.0f, 15.0f, 0.0f),
	//	Vec3f(0.02f, 0.02f, 0.05f),
	//	Vec3f(0.6f, 0.6f, 1.0f),
	//	Vec3f(0.6f, 0.6f, 1.0f),
	//	0.6f, 24
	//);

	//// Back light (cool tone, medium softness)
	//rayTracer->addLight(
	//	Vec3f(0.0f, 4.0f, -10.0f),
	//	Vec3f(0.05f, 0.05f, 0.07f),
	//	Vec3f(0.5f, 0.6f, 0.7f),
	//	Vec3f(0.5f, 0.6f, 0.7f),
	//	0.3f, 12
	//);

	//// Stylized color light from below (cyberpunk bounce)
	//rayTracer->addLight(
	//	Vec3f(0.0f, -3.0f, 3.0f),
	//	Vec3f(0.02f, 0.01f, 0.06f),
	//	Vec3f(0.3f, 0.1f, 0.8f),
	//	Vec3f(0.3f, 0.1f, 0.8f),
	//	0.25f, 10
	//);

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
