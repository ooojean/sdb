#include <stdio.h>
#include <stdlib.h>
#include "b.h"
#include <math.h>
#include <libgen.h>

#define COLOR_WHEEL_SEGMENTS 6

pixel hueToRGB(double hue) {
	pixel color;
	double r, g, b;

	double value = 1.0;

	int segment = floor(hue * COLOR_WHEEL_SEGMENTS);

	double offset = (hue * COLOR_WHEEL_SEGMENTS) - segment;

	switch(segment % COLOR_WHEEL_SEGMENTS){
		//red
		case 0: r=value; g=offset; b=0; break;
		//yellow
		case 1: r=(1-offset); g=value; b=0; break;
		//green
		case 2: r=0; g=value; b=offset; break;
		//cyan
		case 3: r=0; g=(1-offset); b=value; break;
		//blue
		case 4: r=offset; g=0; b=value; break;
		//magenta
		case 5: r=value; g=0; b=(1-offset);break;
	}

	color.r = (int)(r*255.0);
	color.g = (int)(r*255.0);
	color.b = (int)(r*255.0);

	return color;
}

int main(int argc, char **argv){
	if (argc != 2){
		printf("usage: %s <filename>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *filename = argv[1];
	char *output_filename = NULL;
	asprintf(&output_filename, "rainbow_%s", basename(filename));

	printf("input:\t%s\noutput:\t%s\n\n", filename, output_filename);
	ppmimage* image = ppm_readimage(filename);

	for (int x=0; x < image->cols; x++){
		for (int y=0; y< image->rows; y++){
			double hue = ((double)y)/((double)image->rows);
			
			pixel color = hueToRGB(hue);
			const double weight = 0.5;

			image->pixels[y][x].r = (image->pixels[y][x].r*(1-weight)) + (color.r*weight);
			image->pixels[y][x].g = (image->pixels[y][x].g*(1-weight)) + (color.g*weight);
			image->pixels[y][x].b = (image->pixels[y][x].b*(1-weight)) + (color.b*weight);
		}
	}

	ppm_writeimage(output_filename, image);
	ppm_destroy(image);

	return EXIT_SUCCESS;
}
