#include "barcode.h"
#include "loader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv){
    // TODO: parse the arguments in argv. 
    // You can expect argv[1] to be the digits to encode
    // You can expect argv[2] to be the integer width
    // You can expect argv[3] to be the integer height
    // You can expect argv[4] to be the output filepath.

    if(argc != 5) {
        printf("Incorrect number of arguments. Expected: ./build/image_calc <barcode_value> <width> <height> <output_image_path>\n");
        return -1;
    }

    // Parse the arguments based on the expected format
    char* barcode_value = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    char* output_filepath = argv[4]; // Renamed to match assignment instructions

    // Call barcode to generate the image
    // As defined in barcode.h: struct image* barcode(char* data, int width, int height);
    struct image* img = barcode(barcode_value, width, height);

    // Check if image generation was successful
    if (img == NULL) {
        printf("Error: Failed to generate barcode image.\n");
        return -1;
    }

    // Save result to file
    // As defined in loader.h: int saveimage(char* filename, struct image* image);
    int save_result = saveimage(output_filepath, img);

    if (save_result != 0) {
        printf("Error: Failed to save image to %s\n", output_filepath);
        // Free memory before exiting on error
        free(img->pixels);
        free(img);
        return -1;
    }

    // Free the allocated memory for 'img' to prevent leaks.
    // This is important for the valgrind test in test.sh.
    free(img->pixels);
    free(img);

    return 0;
}
