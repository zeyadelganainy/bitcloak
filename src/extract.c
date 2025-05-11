#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


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
            uint8_t bit = image_data[bit_index] & 1;
            ch |= (bit << b);
            bit_index++;
        }

        message[i] = ch;
        if (ch == '\0') break;  // message is done
    }

    return message;
}