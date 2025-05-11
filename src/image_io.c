#include "image_io.h"
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t* load_png(const char* filename, int* width, int* height) {
    unsigned w, h;
    unsigned char* image = NULL;
    unsigned error = lodepng_decode24_file(&image, &w, &h, filename);
    if (error) {
        fprintf(stderr, "Decoder error %u: %s\n", error, lodepng_error_text(error));
        return NULL;
    }
    *width = w;
    *height = h;
    return image;
}

int save_png(const char* filename, const uint8_t* image, int width, int height) {
    unsigned error = lodepng_encode24_file(filename, image, width, height);
    if (error) {
        fprintf(stderr, "Encoder error %u: %s\n", error, lodepng_error_text(error));
        return -1;
    }
    return 0;
}
