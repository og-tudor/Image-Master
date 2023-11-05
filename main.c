#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

typedef unsigned char uchar;

typedef struct image_t {
	uchar **grayscale;
	uchar **red;
	uchar **green;
	uchar **blue;

} IMAGE_T;

int main(void)
{
	//allocating the struct
	IMAGE_T *img = malloc(1 * sizeof(IMAGE_T));
	char command[12], type[3];
	//check = 1 => grayscale, = 2 => rgb
	int width, height, max_val, chk_type = 0;
	int x1, x2, y1, y2, x_1, x_2, y_1, y_2;
	do {
		scanf(" %s", command);
		if (strcmp(command, "LOAD") == 0) {
			load(&chk_type, &height, &width, &max_val, &x1, &y1, &x2, &y2,
				 type, &img->grayscale, &img->red, &img->green, &img->blue);
		} else if (strcmp(command, "SELECT") == 0) {
			select_coord(&x_1, &y_1, &x_2, &y_2, &x1, &y1, &x2, &y2,
						 &chk_type, &height, &width);
		} else if (strcmp(command, "SAVE") == 0) {
			save(width, height, img->grayscale, img->red, img->green, img->blue,
				 chk_type, type, max_val);
		} else if (strcmp(command, "ROTATE") == 0) {
			int angle;
			scanf("%d", &angle);
			if (chk_type == 0)
				printf("No image loaded\n");
			else
				rotate(&img->grayscale, &img->red, &img->green, &img->blue,
					   &x1, &y1, &x2, &y2, &width, &height, chk_type, angle);
		} else if (strcmp(command, "CROP") == 0) {
			if (chk_type == 0)
				printf("No image loaded\n");
			else
				crop(&img->grayscale, &img->red, &img->green, &img->blue,
					 &x1, &y1, &x2, &y2, &width, &height, chk_type);
		} else if (strcmp(command, "APPLY") == 0) {
			char parameter[30];
			fgets(parameter, 30, stdin);
			if (chk_type == 0)
				printf("No image loaded\n");
			else if (parameter[0] == '\n')
				printf("Invalid command\n");
			else if (chk_type == 1)
				printf("Easy, Charlie Chaplin\n");
			else
				apply(&img->grayscale, &img->red, &img->green, &img->blue,
					  &x1, &y1, &x2, &y2, &width, &height, chk_type, parameter);
		} else if (strcmp(command, "EXIT") == 0) {
			if (chk_type == 1) {
				dealloc_grayscale(height, img[0].grayscale);
				free(img);
				return 0;
			} else if (chk_type == 2) {
				dealloc_rgb(height, img[0].red, img[0].green, img[0].blue);
				free(img);
				return 0;
			}
			//No image selected
			printf("No image loaded\n");
			free(img);
			return 0;
		} else if (chk_type != 0) {
			char collect_garbage[1000];
			fgets(collect_garbage, 1000, stdin);
			printf("Invalid command\n");
		} else if (chk_type == 0) {
			char collect_garbage[1000];
			fgets(collect_garbage, 1000, stdin);
			printf("No image loaded\n");
		}
	} while ((strcmp(command, "EXIT") != 0));
	return 0;
}
