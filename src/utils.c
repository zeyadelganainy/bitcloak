
#include "utils.h"
#include <string.h>

void xor_cipher(uint8_t *data, size_t len, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < len; ++i) {
        data[i] ^= key[i % key_len];
    }
}
