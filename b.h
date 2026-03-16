#ifndef B_H
#define B_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint16_t r,g,b;
} pixel;

typedef struct {
	unsigned int rows;
	unsigned int cols;
	unsigned int maxval;
	pixel **pixels;
} ppmimage;

ppmimage *ppm_readimage(const char *filename);
void ppm_destroy(ppmimage* image);
bool ppm_writeimage(const char *filename, ppmimage* image);

#endif
