#ifndef EMBED_H
#define EMBED_H

#include <stdint.h>
#include <stddef.h>

void embed_message(uint8_t *image_data, size_t image_size, const char *message);

#endif
