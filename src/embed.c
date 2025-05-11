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

void embed_file(uint8_t *image_data, size_t image_size, const uint8_t *file_data, size_t file_size) {
    size_t total_bits = (file_size + sizeof(size_t)) * 8;
    if (total_bits > image_size) {
        fprintf(stderr, "Error: file is too large to embed.\n");
        exit(1);
    }

    size_t bit_index = 0;

    // Embed file size (header)
    for (int i = sizeof(size_t) - 1; i >= 0; --i) {
        uint8_t byte = (file_size >> (i * 8)) & 0xFF;
        for (int b = 7; b >= 0; --b) {
            uint8_t bit = (byte >> b) & 1;
            image_data[bit_index] &= 0xFE;
            image_data[bit_index] |= bit;
            bit_index++;
        }
    }

    // Embed file data
    for (size_t i = 0; i < file_size; ++i) {
        uint8_t current_byte = file_data[i];
        for (int b = 7; b >= 0; --b) {
            uint8_t bit = (current_byte >> b) & 1;
            image_data[bit_index] &= 0xFE;
            image_data[bit_index] |= bit;
            bit_index++;
        }
    }
}


