#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Vec3.h"

class Image { 
private:
	int width, height;
	Vec3f * pixels;

public:
	Image(const int w, const int h);
	~Image(void);
	int getWidth(void) const { return width; }
	int getHeight(void) const { return height; }
	void setPixel(const int x, const int y, const Vec3f & c) { pixels[y * width + x] = c; }
	Vec3f getPixel(const int x, const int y) { return pixels[y * width + x]; }
	Vec3f * getPixelBufferPtr(void) { return pixels; }
	void toPPM(const char* path);
	void toBMP(const char* path);
};

#endif
