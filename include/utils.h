
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

void xor_cipher(uint8_t *data, size_t len, const char *key);
double current_time_ms();

#endif
