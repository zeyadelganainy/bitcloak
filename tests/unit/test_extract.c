#include <assert.h>
#include <string.h>
#include "extract.h"

void test_extract_message() {
    uint8_t pixels[100] = {0};
    const char *message = "Hello, BitCloak!";
    size_t img_size = sizeof(pixels);

    embed_message(pixels, img_size, message);

    char extracted[100] = {0};
    extract_message(pixels, img_size, extracted);
    assert(strcmp(message, extracted) == 0); // Ensure the extracted message matches
}

int main() {
    test_extract_message();
    printf("All extract tests passed!\n");
    return 0;
}