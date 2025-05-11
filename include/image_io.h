// image_io.h
#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <stdint.h>

//loads bmp file and returns a pointer to the pixel data and width and height of the image
uint8_t* load_bmp(const char *filename, int *width, int *height);
//saves the modified data back to a bmp file
void save_bmp(const char *filename, const uint8_t *data, int width, int height);

#endif
