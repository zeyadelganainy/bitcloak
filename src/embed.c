#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void embed_message(uint8_t *image_data, size_t image_size, const char *message) {
    size_t msg_len = strlen(message);
    size_t total_bits = (msg_len + 1) * 8; // +1 for null terminator

    if (total_bits > image_size) {
        fprintf(stderr, "Error: message is too large to embed in image.\n");
        exit(1);
    }

    size_t bit_index = 0;

    for (size_t i = 0; i <= msg_len; ++i) {
        uint8_t ch = (i < msg_len) ? message[i] : '\0'; // null terminator

        for (int b = 7; b >= 0; --b) {
            uint8_t bit = (ch >> b) & 1;          // extract bit
            image_data[bit_index] &= 0xFE;        // clear LSB
            image_data[bit_index] |= bit;         // set LSB to message bit
            bit_index++;
        }
    }
}
