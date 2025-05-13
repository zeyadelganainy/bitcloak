#include <assert.h>
#include <string.h>
#include "embed.h"

void test_embed_message() {
    uint8_t pixels[100] = {0};
    const char *message = "Hello, BitCloak!";
    size_t img_size = sizeof(pixels);

    embed_message(pixels, img_size, message);
    // Ensure embedding succeeds (if needed, verify behavior differently)

    char extracted[100] = {0};
    extract_message(pixels, img_size, extracted);
    assert(strcmp(message, extracted) == 0); // Ensure the message matches
}

int main() {
    test_embed_message();
    printf("All embed tests passed!\n");
    return 0;
}