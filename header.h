#ifndef header
#define header
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned char uchar;

uchar **alloc_matrix(int width, int height)
{
	//Allocating every line
	uchar **a;
	a = malloc(height * sizeof(uchar *));
	if (!a) {
		printf("Malloc failed\n");
		return NULL;
	}
	//Allocating every column
	for (int i = 0; i < height; i++) {
		a[i] = malloc(width * sizeof(int));
		if (!a[i]) {
			printf("Malloc failed\n");
			return NULL;
		}
	}
	return a;
}

//Freeing the grayscale
void dealloc_grayscale(int height, uchar **a)
{
	for (int i = 0; i < height; i++)
		free(a[i]);
	free(a);
}

//Freeing the rgb matrix
void dealloc_rgb(int height, uchar **r, uchar **g, uchar **b)
{
	for (int i = 0; i < height; i++) {
		free(r[i]);
		free(g[i]);
		free(b[i]);
	}
	free(r);
	free(g);
	free(b);
}

void read_grayscale(int width, int height, uchar **a, FILE *input, char type[3])
{
	//Text output
	if (strcmp(type, "P2") == 0) {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				fscanf(input, "%hhd", &a[i][j]);
	}
	//Binary output
	if (strcmp(type, "P5") == 0) {
		uchar toxic_trash;
		fread(&toxic_trash, sizeof(uchar), 1, input);
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				fread(&a[i][j], sizeof(uchar), 1, input);
	}
}

void read_rgb(int width, int height, uchar **r, uchar **g, uchar **b,
			  FILE *input, char type[3])
{
	//Text ouput
	if (strcmp(type, "P3") == 0) {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				fscanf(input, "%hhd", &r[i][j]); // red
				fscanf(input, "%hhd", &g[i][j]); // green
				fscanf(input, "%hhd", &b[i][j]); // blue
			}
	}
	//Binary output
	if (strcmp(type, "P6") == 0) {
		uchar toxic_trash;
		fread(&toxic_trash, sizeof(uchar), 1, input);
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				fread(&r[i][j], sizeof(uchar), 1, input); // red
				fread(&g[i][j], sizeof(uchar), 1, input); // green
				fread(&b[i][j], sizeof(uchar), 1, input); // blue
			}
	}
}

void load(int *chk_type, int *height, int *width, int *max_val,
		  int *x1, int *y1, int *x2, int *y2, char type[3],
		  uchar ***a, uchar ***r, uchar ***g, uchar ***b)
{
	char input_file[30];
	scanf(" %s", input_file);
	//Reading all the files in binary
	FILE *input = fopen(input_file, "rb");
	//Case if image doesn't exist
	if (!input) {
		fprintf(stdout, "Failed to load %s\n", input_file);
		//Freeing the previous loaded matrix
		if (*chk_type == 1) {
			dealloc_grayscale(*height, *a);
			*chk_type = 0;
		} else if (*chk_type == 2) {
			dealloc_rgb(*height, *r, *g, *b);
			*chk_type = 0;
		}
	//Case where file exists
	} else {
		//Freeing the previous loaded matrix
		if (*chk_type == 1)
			dealloc_grayscale(*height, *a);
		else if (*chk_type == 2)
			dealloc_rgb(*height, *r, *g, *b);
		//Reading the header and assigning the whole selection
		fscanf(input, "%s", type);
		fscanf(input, "%d", width);
		*x1 = 0;
		*x2 = *width;
		fscanf(input, "%d", height);
		*y1 = 0;
		*y2 = *height;
		fscanf(input, "%d", max_val);
		// Grayscale
		if (strcmp(type, "P2") == 0 || strcmp(type, "P5") == 0) {
			*chk_type = 1;
			*a = alloc_matrix(*width, *height);
			read_grayscale(*width, *height, *a, input, type);
		// RGB
		} else if (strcmp(type, "P3") == 0 || strcmp(type, "P6") == 0) {
			*chk_type = 2;
			*r = alloc_matrix(*width, *height);
			*g = alloc_matrix(*width, *height);
			*b = alloc_matrix(*width, *height);
			read_rgb(*width, *height, *r, *g, *b, input, type);
		}
		printf("Loaded %s\n", input_file);
		fclose(input);
	}
}

void select_coord(int *x_1, int *y_1, int *x_2, int *y_2, int *x1, int *y1,
				  int *x2, int *y2, int *chk_type, int *height, int *width)
{
	int aux;
	*x_1 = *x1; *y_1 = *y1; *x_2 = *x2; *y_2 = *y2;
	//Case where an image is loaded
	if (*chk_type != 0) {
		char all[4];
		scanf("%s", all);
		//Select whole matrix
		if  (strstr(all, "ALL")) {
			*x1 = 0; *x2 = *width; *y1 = 0; *y2 = *height;
			printf("Selected ALL\n");
		//Select only a part of the image
		} else {
			*x1 = atoi(all);
			scanf("%d %d %d", y1, x2, y2);
			//Interchanging the coordinates if they are in the wrong order
			if (*x2 < *x1) {
				aux = *x1;
				*x1 = *x2;
				*x2 = aux;
				}
			if (*y2 < *y1) {
				aux = *y1;
				*y1 = *y2;
				*y2 = aux;
			}
			//Checking if the selection is valid
			if ((*x2 - *x1) > *width || (*y2 - *y1) > *height ||
				(*x2 == *x1) || (*y2 == *y1) || (*x2 > *width) ||
				(*y2 > *height) || (*x1 < 0) || (*x2 < 0)) {
				printf("Invalid set of coordinates\n");
				*x1 = *x_1; *y1 = *y_1; *x2 = *x_2; *y2 = *y_2;
			//Successful selection
			} else {
				printf("Selected %d %d %d %d\n", *x1, *y1, *x2, *y2);
			}
		}
	}
}

void save(int width, int height, uchar **a, uchar **b, uchar **c, uchar **d,
		  int chk_type, char type[3], int max_val)
{
	/* For each type i first write the header then the values
	   Values are always written in ascii */
	// Check if an image is loaded
	if (chk_type == 0) {
		//printf("No image loaded\n");
		return;
	}
	// format = ascii / no ascii :(
	FILE *exit;
	char exit_file[40];
	char *format;
	fgets(exit_file, 40, stdin);
	format = strstr(exit_file, "ascii");
	char *file_name;
	file_name = strtok(exit_file, "\n ");
	// Checking ascii
	if (format) {
		exit = fopen(file_name, "wt");
		// GRAYSCALE
		if (chk_type == 1) {
			fprintf(exit, "P2\n");
			fprintf(exit, "%d %d\n", width, height);
			fprintf(exit, "%d\n", max_val);
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++)
					fprintf(exit, "%d ", a[i][j]);
			fprintf(exit, "\n");
			}
		}
		// RGB
		if (chk_type == 2) {
			fprintf(exit, "P3\n");
			fprintf(exit, "%d %d\n", width, height);
			fprintf(exit, "%d\n", max_val);
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					fprintf(exit, "%d ", b[i][j]);
					fprintf(exit, "%d ", c[i][j]);
					fprintf(exit, "%d ", d[i][j]);
				}
			fprintf(exit, "\n");
			}
		}
		printf("Saved %s\n", file_name);
	//BINAR
	} else {
		exit = fopen(file_name, "wb");
		//GRAYSCALE
		if (chk_type == 1) {
			fprintf(exit, "P5\n");
			fprintf(exit, "%d %d\n", width, height);
			fprintf(exit, "%d\n", max_val);
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++)
					fwrite(&a[i][j], sizeof(uchar), 1, exit);
			}
		}
		//RGB
		if (chk_type == 2) {
			fprintf(exit, "P6\n");
			fprintf(exit, "%d %d\n", width, height);
			fprintf(exit, "%d\n", max_val);
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					fwrite(&b[i][j], sizeof(uchar), 1, exit);
					fwrite(&c[i][j], sizeof(uchar), 1, exit);
					fwrite(&d[i][j], sizeof(uchar), 1, exit);
				}
			}
		}
		// Successful save
		printf("Saved %s\n", file_name);
	}
	fclose(exit);
}

uchar **rotate_img(uchar ***a, uchar ***r, uchar ***g, uchar ***b, int *width,
				   int *height, int *x2, int *y2, int n, int chk_type)
{
	while (n) {
		//grayscale
		if (chk_type == 1) {
			uchar **aux = alloc_matrix(*height, *width);
			for (int i = 0; i < *height; i++) {
				for (int j = 0; j < *width; j++)
					aux[j][*height - i - 1] = (*a)[i][j];
			}
			dealloc_grayscale(*height, *a);
			*a = aux;
		} else {
			uchar **aux1 = alloc_matrix(*height, *width);
			uchar **aux2 = alloc_matrix(*height, *width);
			uchar **aux3 = alloc_matrix(*height, *width);
			for (int i = 0; i < *height; i++) {
				for (int j = 0; j < *width; j++)
					aux1[j][*height - i - 1] = (*r)[i][j];
			}
			dealloc_grayscale(*height, *r);
			*r = aux1;
			for (int i = 0; i < *height; i++) {
				for (int j = 0; j < *width; j++)
					aux2[j][*height - i - 1] = (*g)[i][j];
			}
			dealloc_grayscale(*height, *g);
			*g = aux2;
			for (int i = 0; i < *height; i++) {
				for (int j = 0; j < *width; j++)
					aux3[j][*height - i - 1] = (*b)[i][j];
			}
			dealloc_grayscale(*height, *b);
			*b = aux3;
		}
		int auxi = *width;
		*width = *height;
		*height = auxi;
		*x2 = *width;
		*y2 = *height;
		n--;
	}
}

uchar **rotate_square(uchar ***a, uchar **aux, int x1,
					  int y1, int x2, int y2, int n)
{
	while (n) {
		for (int i = 0; i < y2 - y1; i++) {
			for (int j = 0; j < x2 - x1; j++)
				aux[j][(x2 - x1) - i - 1] = (*a)[i + y1][j + x1];
		}

		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++)
				(*a)[i][j] = aux[i - y1][j - x1];
		}
		n--;
	}
}

void rotate(uchar ***a, uchar ***r, uchar ***g, uchar ***b, int *x1, int *y1,
			int *x2, int *y2, int *width, int *height, int chk_type, int angle)
{
	/*The algorithm for rotate moves lines from top to bottom to
	  columns from right to left performing a rotation by 90`
	*/
	// Checking if the selection s square
	if (*x1 != 0 && *x2 != *width && *y1 != 0 &&
		*y2 != *height && (*x2 - *x1) != (*y2 - *y1)) {
		printf("The selection must be square\n");
		int trash;
		scanf("%d", &trash);
		return;
	}
	// Remembering the input angle
	int true_angle = angle;
	angle = angle % 360;
	if (angle < 0)
		angle = angle + 360;
	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		int trash;
		scanf("%d", &trash);
		return;
	}
	// Angle/90 is the numbers of 90` rotations
	// Rotate whole matrix
	if (*x1 == 0 && *x2 == *width && *y1 == 0 && *y2 == *height) {
		rotate_img(a, r, g, b, width, height, x2, y2, angle / 90, chk_type);

	// Rotate square selection
	} else {
		// Grayscale case
		if (chk_type == 1) {
			uchar **aux = alloc_matrix((*y2 - *y1), (*x2 - *x1));
			rotate_square(a, aux, *x1, *y1, *x2, *y2, angle / 90);
			dealloc_grayscale((*y2 - *y1), aux);
		// RGB case
		} else {
			uchar **aux1 = alloc_matrix((*y2 - *y1), (*x2 - *x1));
			uchar **aux2 = alloc_matrix((*y2 - *y1), (*x2 - *x1));
			uchar **aux3 = alloc_matrix((*y2 - *y1), (*x2 - *x1));
			rotate_square(r, aux1, *x1, *y1, *x2, *y2, angle / 90);
			rotate_square(g, aux2, *x1, *y1, *x2, *y2, angle / 90);
			rotate_square(b, aux3, *x1, *y1, *x2, *y2, angle / 90);
			dealloc_grayscale((*y2 - *y1), aux1);
			dealloc_grayscale((*y2 - *y1), aux2);
			dealloc_grayscale((*y2 - *y1), aux3);
		}
	}
	if (true_angle != 360 && true_angle != -360)
		true_angle = true_angle % 360;
	printf("Rotated %d\n", true_angle);
}

void crop_matrix(uchar **a, uchar **aux, int x1, int y1, int x2, int y2)
{
	for (int i = y1; i < y2; i++)
		for (int j = x1; j < x2; j++)
			aux[i - y1][j - x1] = a[i][j];
}

void crop(uchar ***a, uchar ***r, uchar ***g, uchar ***b, int *x1, int *y1,
		  int *x2, int *y2, int *width, int *height, int chk_type)
{
	// Grayscale case
	if (chk_type == 1) {
		uchar **aux = alloc_matrix((*x2 - *x1), (*y2 - *y1));
		crop_matrix(*a, aux, *x1, *y1, *x2, *y2);
		dealloc_grayscale(*height, *a);
		*a = aux;
		*height = *y2 - *y1;
		*width = *x2 - *x1;
		*x1 = 0;
		*x2 = *width;
		*y1 = 0;
		*y2 = *height;
		printf("Image cropped\n");
	// RGB case
	} else {
		uchar **aux1 = alloc_matrix((*x2 - *x1), (*y2 - *y1));
		uchar **aux2 = alloc_matrix((*x2 - *x1), (*y2 - *y1));
		uchar **aux3 = alloc_matrix((*x2 - *x1), (*y2 - *y1));
		crop_matrix(*r, aux1, *x1, *y1, *x2, *y2);
		crop_matrix(*g, aux2, *x1, *y1, *x2, *y2);
		crop_matrix(*b, aux3, *x1, *y1, *x2, *y2);
		dealloc_grayscale(*height, *r);
		dealloc_grayscale(*height, *g);
		dealloc_grayscale(*height, *b);
		*r = aux1;
		*g = aux2;
		*b = aux3;
		*height = *y2 - *y1;
		*width = *x2 - *x1;
		*x1 = 0;
		*x2 = *width;
		*y1 = 0;
		*y2 = *height;
		printf("Image cropped\n");
	}
}

void clamp(double *value)
{
	if (*value > 255)
		*value = 255;
	else if (*value < 0)
		*value = 0;
}

int apply_gaussian_blur(uchar **aux, int i, int j)
{
	double value = 0;

	value = value + aux[i - 1][j - 1] * 0.0625;
	value = value + aux[i - 1][j] * (0.125);
	value = value + aux[i - 1][j + 1] * (0.0625);

	value = value + aux[i][j - 1] * (0.125);
	value = value + aux[i][j] * (0.25);
	value = value + aux[i][j + 1] * (0.125);

	value = value + aux[i + 1][j - 1] * (0.0625);
	value = value + aux[i + 1][j] * (0.125);
	value = value + aux[i + 1][j + 1] * (0.0625);

	clamp(&value);
	int true_value = floor(value);
	return value;
}

int apply_blur(uchar **aux, int i, int j)
{
	double value = 0;

	value = value + aux[i - 1][j - 1] * (1.0 * 1 / 9);
	value = value + aux[i - 1][j] * (1.0 * 1 / 9);
	value = value + aux[i - 1][j + 1] * (1.0 * 1 / 9);

	value = value + aux[i][j - 1] * (1.0 * 1 / 9);
	value = value + aux[i][j] * (1.0 * 1 / 9);
	value = value + aux[i][j + 1] * (1.0 * 1 / 9);

	value = value + aux[i + 1][j - 1] * (1.0 * 1 / 9);
	value = value + aux[i + 1][j] * (1.0 * 1 / 9);
	value = value + aux[i + 1][j + 1] * (1.0 * 1 / 9);

	clamp(&value);
	int true_value = floor(value);
	return value;
}

int apply_edge(uchar **aux, int i, int j)
{
	double value = 0;

	value = value + aux[i - 1][j - 1] * (-1);
	value = value + aux[i - 1][j] * (-1);
	value = value + aux[i - 1][j + 1] * (-1);

	value = value + aux[i][j - 1] * (-1);
	value = value + aux[i][j] * (8);
	value = value + aux[i][j + 1] * (-1);

	value = value + aux[i + 1][j - 1] * (-1);
	value = value + aux[i + 1][j] * (-1);
	value = value + aux[i + 1][j + 1] * (-1);

	clamp(&value);
	int true_value = floor(value);
	return value;
}

int apply_sharpen(uchar **aux, int i, int j)
{
	double value = 0;

	value = value + aux[i - 1][j - 1] * 0;
	value = value + aux[i - 1][j] * (-1);
	value = value + aux[i - 1][j + 1] * (0);

	value = value + aux[i][j - 1] * (-1);
	value = value + aux[i][j] * (5);
	value = value + aux[i][j + 1] * (-1);

	value = value + aux[i + 1][j - 1] * (0);
	value = value + aux[i + 1][j] * (-1);
	value = value + aux[i + 1][j + 1] * (0);

	clamp(&value);
	int true_value = floor(value);
	return value;
}

void apply(uchar ***a, uchar ***r, uchar ***g, uchar ***b,
		   int *x1, int *y1, int *x2, int *y2, int *width, int *height,
		   int chk_type, char parameter[30])
{
	/* chk[1-4] (uso genius) are used for checking
	   if the selection is on the edges of the image */
	int chk1 = 0;
	int chk2 = 0;
	int chk3 = 0;
	int chk4 = 0;
	parameter[strlen(parameter) - 1] = '\0';
	parameter++;
	uchar **aux1 = alloc_matrix(*width, *height);
	uchar **aux2 = alloc_matrix(*width, *height);
	uchar **aux3 = alloc_matrix(*width, *height);
	if (*y1 == 0)
		chk1 = 1;
	if (*x1 == 0)
		chk2 = 1;
	if (*x2 == *width)
		chk3 = 1;
	if (*y2 == *height)
		chk4 = 1;
	for (int i = 0; i < *height; i++)
		for (int j = 0; j < *width; j++)
			aux1[i][j] = (*r)[i][j];
	for (int i = 0; i < *height; i++)
		for (int j = 0; j < *width; j++)
			aux2[i][j] = (*g)[i][j];
	for (int i = 0; i < *height; i++)
		for (int j = 0; j < *width; j++)
			aux3[i][j] = (*b)[i][j];
	if (strcmp(parameter, "EDGE") == 0) {
		for (int i = (chk1 + *y1); i < (*y2 - chk4); i++) {
			for (int j = (chk2 + *x1); j < (*x2 - chk3); j++) {
				(*r)[i][j] = apply_edge(aux1, i, j);
				(*g)[i][j] = apply_edge(aux2, i, j);
				(*b)[i][j] = apply_edge(aux3, i, j);
			}
		}
	} else if (strcmp(parameter, "SHARPEN") == 0) {
		for (int i = (chk1 + *y1); i < (*y2 - chk4); i++) {
			for (int j = (chk2 + *x1); j < (*x2 - chk3); j++) {
				(*r)[i][j] = apply_sharpen(aux1, i, j);
				(*g)[i][j] = apply_sharpen(aux2, i, j);
				(*b)[i][j] = apply_sharpen(aux3, i, j);
			}
		}
		//TODO muta dupa ultimu else astea
	} else if (strcmp(parameter, "GAUSSIAN_BLUR") == 0) {
		for (int i = (chk1 + *y1); i < (*y2 - chk4); i++) {
			for (int j = (chk2 + *x1); j < (*x2 - chk3); j++) {
				(*r)[i][j] = apply_gaussian_blur(aux1, i, j);
				(*g)[i][j] = apply_gaussian_blur(aux2, i, j);
				(*b)[i][j] = apply_gaussian_blur(aux3, i, j);
			}
		}
	} else if (strcmp(parameter, "BLUR") == 0) {
		for (int i = (chk1 + *y1); i < (*y2 - chk4); i++) {
			for (int j = (chk2 + *x1); j < (*x2 - chk3); j++) {
				(*r)[i][j] = apply_blur(aux1, i, j);
				(*g)[i][j] = apply_blur(aux2, i, j);
				(*b)[i][j] = apply_blur(aux3, i, j);
			}
		}
	} else {
		printf("APPLY parameter invalid\n");
		return;
	}
	dealloc_grayscale(*height, aux1);
	dealloc_grayscale(*height, aux2);
	dealloc_grayscale(*height, aux3);
	printf("APPLY %s done\n", parameter);
}

#endif
