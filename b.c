#include "b.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>


ppmimage *ppm_readimage(const char *filename){
	FILE *file = fopen(filename, "rb");
	if (file == NULL) {
		perror("error could not open file");
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	size_t filesize = ftell(file);
	int file_desc = fileno(file);
	rewind(file);

	ppmimage *result = malloc(sizeof *result);

	char type[3]= {0};
	fscanf(file, "%2s", type);
	if(strcmp(type, "P6")!=0){
		perror("not a P6 PPM file");
		fclose(file);
		return NULL;
	}

	fscanf(file, "%u %u", &(result->cols),&(result->rows));
	fscanf(file, "%u", &(result->maxval));

	off_t offset = ftell(file);

	uint8_t *rawdata = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, file_desc, 0);

	if(rawdata == MAP_FAILED) {
		perror("mmap error");
		fclose(file);
		free(result);
		return NULL;
	}

	uint8_t bytes_per_value= (result->maxval > 256 ? 2 : 1);
	const int values_per_pixel = 3;

	uint8_t *pixeldata = rawdata + offset + 1;

	result->pixels = calloc(result->rows, sizeof(pixel*));
	for (int y=0; y < result->rows; y++){
		result->pixels[y] = calloc(result->cols, sizeof(pixel));
		for (int x=0; x < result->cols; x++) {
			uint8_t *pixel_start = pixeldata + (y * result->cols * values_per_pixel * bytes_per_value) + (x * values_per_pixel * bytes_per_value);

			if (bytes_per_value == 1){
				result->pixels[y][x].r = pixel_start[0];
				result->pixels[y][x].g = pixel_start[1];
				result->pixels[y][x].b = pixel_start[2];
			}else{
				result->pixels[y][x].r = (pixel_start[0] << 8) | pixel_start[1];
				result->pixels[y][x].g = (pixel_start[2] << 8) | pixel_start[3];
				result->pixels[y][x].b = (pixel_start[4] << 8) | pixel_start[5];

			}
		}
	}
	return result;
}
void ppm_destroy(ppmimage* image){
	for (int y =0; y < image->rows; y++){
		free(image->pixels[y]);
	}
	free(image);
}

bool ppm_writeimage(const char *filename, ppmimage* image){
	FILE *file = fopen(filename, "wb");
	if (file == NULL) {
		perror("could not open file");
		return false;
	}

	fprintf(file, "P6\n%u %u\n%u\n", image->cols, image->rows, image->maxval);
	uint8_t bytes_per_value = (image->maxval > 256 ? 2 : 1);
	for(int y=0; y < image->rows; y++){
		for (int x=0; x < image->cols; x++){
			fwrite(&(image->pixels[y][x].r), bytes_per_value, 1, file);
			fwrite(&(image->pixels[y][x].g), bytes_per_value, 1, file);
			fwrite(&(image->pixels[y][x].b), bytes_per_value, 1, file);


		}
	}
	return true;
}
