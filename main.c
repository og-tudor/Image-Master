#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

typedef unsigned char uchar;

typedef struct image_t
{
    uchar **grayscale;
    uchar **red;
    uchar **green;
    uchar **blue;

}Image_t;

int main(void)
{
    Image_t *img = malloc(1 * sizeof(Image_t));
    char command[10], type[3];
    int width, height, max_val, chk_type = 0, chk_format, x1, x2, y1, y2;
	do {
        scanf(" %s", command);
        // LOAD
        if (strcmp(command, "LOAD") == 0) {
            char input_file[30];
            scanf(" %s", input_file);
            FILE *input = fopen(input_file, "rb");
            if (input == NULL)
                fprintf(stderr, "Failed to load %s", input_file);
            else {
                if(chk_type == 1) {
                    dealloc_grayscale(height, img[0].grayscale);
                }
                else if(chk_type == 2) {
                    dealloc_rgb(height, img[0].red, img[0].green, img[0].blue);
                }
                fscanf(input, "%s", type);
                fscanf(input, "%d", &width);
                x1 = 0;
                x2 = width;
                fscanf(input, "%d", &height);
                y1 = 0;
                y2 = height;
                fscanf(input, "%d", &max_val);
                // Grayscale
                if(strcmp(type, "P2") == 0 || strcmp(type, "P5") == 0) {
                    chk_type = 1;
                    img->grayscale = alloc_matrix(width, height);
                    chk_format = read_grayscale(width, height, img[0].grayscale, input, type);
                // RGB
                } else if (strcmp(type, "P3") == 0 || strcmp(type, "P6") == 0) {
                    chk_type = 2;
                    img->red = alloc_matrix(width, height);
                    img->green = alloc_matrix(width, height);
                    img->blue = alloc_matrix(width, height);
                    chk_format = read_rgb(width, height, img[0].red, img[0].green, img[0].blue, input, type);
                }
            }
            printf("\n %s type \t %d width \t %d height \t %d max_value\t %d chk_type\n", type, width, height, max_val, chk_type);
            printf("Loaded %s\n", input_file);
            fclose(input);
        }

        else if (strcmp(command, "PRINT") == 0) {
            if(chk_type == 1)
                print_grayscale(img->grayscale, width, height);
            else if (chk_type == 2)
                print_rgb(img->red, img->green, img->blue, width, height);
        }

        else if (strcmp(command, "SELECT") == 0) {
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            if(chk_type == 0) {
                x1 = 0;
                x2 = width;
                y1 = 0;
                y2 = height;
                printf("No image loaded");
            }
            else if((x2-x1) > width || (y2-y1) > height) {
                printf("Invalid set of coordinates\n");
                x1 = 0;
                x2 = width;
                y1 = 0;
                y2 = height;
            }
            else
                printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
        }

        else if (strcmp(command, "SAVE") == 0) {
            save(width, height, img[0].grayscale, img[0].red, img[0].green, img[0].blue, chk_type, type, max_val);
        }

        else if (strcmp(command, "ROTATE") == 0) {
            if(chk_type == 0)
                printf("No image loaded");
            else
                rotate(img->grayscale, img->red, img->green, img->blue, x1, y1, x2, y2, width, height, chk_type);
        }

        else if (strcmp(command, "CROP") == 0) {
            if(chk_type == 0)
                printf("No image loaded");
            else
                crop(&img->grayscale, img->red, img->green, img->blue, &x1, &y1, &x2, &y2, &width, &height, chk_type);
        }

        else if (strcmp(command, "EXIT") == 0) {
            if(chk_type == 1) {
                dealloc_grayscale(height, img[0].grayscale);
                free(img);
            }
            else if(chk_type == 2) {
                dealloc_rgb(height, img[0].red, img[0].green, img[0].blue);
                free(img);
            }
            else
                printf("No image loaded");
        }
        else
            printf("Invalid command\n");
	} while (strcmp(command, "EXIT") != 0);
    return 0;
}
