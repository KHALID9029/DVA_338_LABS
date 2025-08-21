#include "Image.h"
#include <stdio.h>
#include <string>
#include "libbmp.h"
Image::Image(const int w, const int h) : width(w), height(h) {
    pixels = new Vec3f[width * height];
}
Image::~Image(void) {
    delete [] pixels;
}

typedef unsigned char col_int;
void Image::toPPM(const char* path) {
    // https://nicolashollmann.de/blog/writing-portable-anymap/
    FILE *outfile = fopen(path, "wb");
    if(NULL == outfile) 
    {
        fprintf(stderr,"Could not open the image file '%s' for saving.\n", path);
        return;
    }
    fprintf(outfile, "P6");
    fprintf(outfile, "\n%d %d\n255\n", width, height);
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            const Vec3f pixel = getPixel(x, y) * 255;
            col_int colors[3] = {
                (col_int)(pixel.r), 
                (col_int)(pixel.g), 
                (col_int)(pixel.b) 
                };
            fwrite(colors, 3, 1, outfile);
        }
    }

    fclose(outfile);
    printf("PPM Image created at %s\n", path);
}

void Image::toBMP(const char* path){
    BmpImg img(this->getWidth(), this->getHeight());
    
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            const Vec3f pixel = getPixel(x, y) * 255;
            img.set_pixel(x,y, (col_int)pixel.r, (col_int)pixel.g, (col_int)pixel.b);
        }
    }

    BmpError err = img.write(path);
    switch (err)
    {
    case BmpError::BMP_OK:
        printf("BMP Image created at %s\n", path);
        break;
    
    default:
        fprintf(stderr, 
        "An error occurred when trying to create BMP\n");
        fprintf(stderr, 
        "Error code: %d\n", err);
        break;
    }
}