#include "barcode.h"
#include <stdio.h>
#include <stdlib.h>

struct image* barcode(char* data, int width, int height) {
    // 1. Allocate memory for the image struct itself
    struct image* img = (struct image*)malloc(sizeof(struct image));
    
    // Check if malloc failed
    if (img == NULL) {
        return NULL;
    }

    // 2. Initialize the members of the image struct
    img->width = width;
    img->height = height;

    // 3. Allocate memory for the pixels
    // Assuming the 'pixels' member is a pointer to an array of pixel structs.
    // If 'pixels' is a pointer to a flat array of bytes, you may need to adjust the size calculation.
    img->pixels = (struct pixel*)malloc(sizeof(struct pixel) * width * height);

    // Check if pixel allocation failed
    if (img->pixels == NULL) {
        free(img); // Clean up the previously allocated struct
        return NULL;
    }

    // TODO: If your struct image has other members (like a header or magic number),
    // initialize them here.

    // 4. Return the pointer to the blank image
    return img;
}
