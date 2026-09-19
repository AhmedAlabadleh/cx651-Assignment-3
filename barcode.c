#include "barcode.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Encoding masks for the digits (7 bits each).
// 1 represents a white bar, 0 represents a black bar.
// Left-hand encodings (L-codes)
static const unsigned char L_CODE[10] = {
    0b1110010, // 0
    0b1100110, // 1
    0b1101100, // 2
    0b1010000, // 3
    0b1011100, // 4
    0b1001110, // 5
    0b1000010, // 6
    0b1000100, // 7
    0b1001000, // 8
    0b1110100  // 9
};

// Right-hand encodings (R-codes) are the bitwise inverse of the L-codes.
static const unsigned char R_CODE[10] = {
    0b0001101, // 0
    0b0011001, // 1
    0b0010011, // 2
    0b0101111, // 3
    0b0100011, // 4
    0b0110001, // 5
    0b0111101, // 6
    0b0111011, // 7
    0b0110111, // 8
    0b0001011  // 9
};

// Special patterns
#define START_CODE  0b010
#define MIDDLE_CODE 0b10101
#define END_CODE    0b010

// Helper function to set a pixel to black or white
void set_pixel(struct image* img, int x, int y, int is_white) {
    // Bounds check to prevent writing out of bounds
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) {
        return;
    }
    
    int index = y * img->width + x;
    if (is_white) {
        // White pixel
        img->pixels[index].r = 255;
        img->pixels[index].g = 255;
        img->pixels[index].b = 255;
    } else {
        // Black pixel
        img->pixels[index].r = 0;
        img->pixels[index].g = 0;
        img->pixels[index].b = 0;
    }
}

// Helper function to fill a vertical strip of pixels for a given pattern.
// The pattern is a 7-bit value (or smaller) where the MSB is the leftmost bar.
void fill_pattern(struct image* img, int start_x, unsigned char pattern, int pattern_width) {
    for (int i = 0; i < pattern_width; i++) {
        // Extract the bit from the pattern. We read from MSB to LSB.
        int bit = (pattern >> (pattern_width - 1 - i)) & 1;
        for (int y = 0; y < img->height; y++) {
            set_pixel(img, start_x + i, y, bit);
        }
    }
}

struct image* barcode(char* data, int width, int height) {
    // 1. Allocate memory for the image struct itself
    struct image* img = (struct image*)malloc(sizeof(struct image));
    if (img == NULL) {
        return NULL;
    }

    // 2. Initialize the members of the image struct
    img->width = width;
    img->height = height;

    // 3. Allocate memory for the pixels
    img->pixels = (struct pixel*)malloc(sizeof(struct pixel) * width * height);
    if (img->pixels == NULL) {
        free(img);
        return NULL;
    }

    // Initialize all pixels to white (default background)
    for (int i = 0; i < width * height; i++) {
        img->pixels[i].r = 255;
        img->pixels[i].g = 255;
        img->pixels[i].b = 255;
    }

    // 4. Generate the Barcode Pattern
    // We assume the width is exactly 113 as per the assignment specification.
    // If the width is different, the barcode may be cut off or have extra space.
    int current_x = 0;

    // Quiet Zone (Left) - 9 white bars
    current_x += 9;

    // Start Pattern - 3 bars
    fill_pattern(img, current_x, START_CODE, 3);
    current_x += 3;

    // Left Digits (6 digits) - 7 bars each
    for (int i = 0; i < 6; i++) {
        int digit = data[i] - '0'; // Convert char to int
        fill_pattern(img, current_x, L_CODE[digit], 7);
        current_x += 7;
    }

    // Middle Pattern - 5 bars
    fill_pattern(img, current_x, MIDDLE_CODE, 5);
    current_x += 5;

    // Right Digits (6 digits) - 7 bars each
    for (int i = 6; i < 12; i++) {
        int digit = data[i] - '0';
        fill_pattern(img, current_x, R_CODE[digit], 7);
        current_x += 7;
    }

    // End Pattern - 3 bars
    fill_pattern(img, current_x, END_CODE, 3);
    current_x += 3;

    // Quiet Zone (Right) - 9 white bars
    current_x += 9;

    // The total width should be 113. If current_x is less than width,
    // the remaining pixels stay white due to the initialization loop.

    return img;
}
