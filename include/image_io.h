#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <stdint.h>

uint8_t* load_png(const char* filename, int* width, int* height);
int save_png(const char* filename, const uint8_t* image, int width, int height);

#endif
