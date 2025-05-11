#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdint.h>
#include <stddef.h>

char* extract_message(const uint8_t *image_data, size_t image_size);

#endif
