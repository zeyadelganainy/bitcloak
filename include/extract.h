#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdint.h>
#include <stddef.h>

char* extract_message(const uint8_t *image_data, size_t image_size);
uint8_t* extract_file(const uint8_t *image_data, size_t image_size, size_t *out_file_size, char *out_filename);


#endif
