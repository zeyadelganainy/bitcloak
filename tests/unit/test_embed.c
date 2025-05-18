#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "embed.h"
#include "extract.h"

// Test embedding a message into an image
void test_embed_message() {
    uint8_t pixels[100] = {0};
    const char *message = "Hello, BitCloak!";
    size_t img_size = sizeof(pixels);

    int result = embed_message(pixels, img_size, message);
    assert(result == 0); // Ensure the function succeeds

    // Extract the message to verify
    char extracted[100] = {0};
    extract_message(pixels, img_size, extracted);
    assert(strcmp(message, extracted) == 0); // Ensure the message matches

    printf("test_embed_message passed!\n");
}

// Test embedding a file into an image
void test_embed_file() {
    uint8_t pixels[1000] = {0};
    const char *filename = "test.txt";
    const uint8_t file_data[] = "This is a test file.";
    size_t file_size = sizeof(file_data) - 1; // Exclude null terminator
    size_t img_size = sizeof(pixels);

    int result = embed_file(pixels, img_size, file_data, file_size, filename);
    assert(result == 0); // Ensure the function succeeds

    // Verify the embedded file
    uint8_t extracted_data[1000] = {0};
    char extracted_filename[256] = {0};
    size_t extracted_size = 0;

    extract_file(pixels, img_size, extracted_data, &extracted_size, extracted_filename);
    assert(strcmp(filename, extracted_filename) == 0); // Ensure filenames match
    assert(extracted_size == file_size);               // Ensure file sizes match
    assert(memcmp(file_data, extracted_data, file_size) == 0); // Ensure file data matches

    printf("test_embed_file passed!\n");
}

// Test embedding a message that is too large for the image
void test_embed_message_too_large() {
    uint8_t pixels[10] = {0}; // Small image
    const char *message = "This message is too large for the image.";

    int result = embed_message(pixels, sizeof(pixels), message);
    assert(result == -1); // Ensure the function returns an error

    printf("test_embed_message_too_large passed!\n");
}

// Test embedding a file that is too large for the image
void test_embed_file_too_large() {
    uint8_t pixels[100] = {0}; // Small image
    const char *filename = "test.txt";
    const uint8_t file_data[200] = {0}; // File larger than the image
    size_t file_size = sizeof(file_data);

    int result = embed_file(pixels, sizeof(pixels), file_data, file_size, filename);
    assert(result == -1); // Ensure the function returns an error

    printf("test_embed_file_too_large passed!\n");
}

int main() {
    test_embed_message();
    test_embed_file();
    test_embed_message_too_large();
    test_embed_file_too_large();

    printf("All embed tests passed!\n");
    return 0;
}