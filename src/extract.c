
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "extract.h"

char* extract_message(const uint8_t *image_data, size_t image_size) {
    size_t bit_index = 0;
    size_t buffer_size = image_size / 8;
    char *message = malloc(buffer_size);
    if (!message) {
        perror("Failed to allocate memory for extracted message");
        exit(1);
    }

    for (size_t i = 0; i < buffer_size; ++i) {
        char ch = 0;
        for (int b = 7; b >= 0; --b) {
            uint8_t bit = image_data[bit_index++] & 1;
            ch |= (bit << b);
        }

        message[i] = ch;
        if (ch == '\0') break;
    }

    return message;
}

uint8_t* extract_file(const uint8_t *image_data, size_t image_size, size_t *out_file_size, char *out_filename) {
    size_t bit_index = 0;

    // Read filename length
    uint8_t name_len = 0;
    for (int b = 7; b >= 0; --b) {
        uint8_t bit = image_data[bit_index++] & 1;
        name_len |= (bit << b);
    }

    // Read filename
    for (uint8_t i = 0; i < name_len; ++i) {
        char ch = 0;
        for (int b = 7; b >= 0; --b) {
            uint8_t bit = image_data[bit_index++] & 1;
            ch |= (bit << b);
        }
        out_filename[i] = ch;
    }
    out_filename[name_len] = '\0';

    // Read file size
    size_t file_size = 0;
    for (int i = 0; i < sizeof(size_t); ++i) {
        uint8_t byte = 0;
        for (int b = 7; b >= 0; --b) {
            uint8_t bit = image_data[bit_index++] & 1;
            byte |= (bit << b);
        }
        file_size = (file_size << 8) | byte;
    }

    *out_file_size = file_size;

    uint8_t *buffer = malloc(file_size);
    if (!buffer) {
        perror("malloc");
        exit(1);
    }

    for (size_t i = 0; i < file_size; ++i) {
        uint8_t byte = 0;
        for (int b = 7; b >= 0; --b) {
            uint8_t bit = image_data[bit_index++] & 1;
            byte |= (bit << b);
        }
        buffer[i] = byte;
    }

    return buffer;
}
