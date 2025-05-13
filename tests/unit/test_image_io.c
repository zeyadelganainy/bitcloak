#include <assert.h>
#include "image_io.h"

void test_load_and_save_png() {
    int width = 100, height = 100;
    uint8_t pixels[100 * 100 * 3] = {255}; // White image

    save_png("test_output.png", pixels, width, height);
    int loaded_width, loaded_height;
    uint8_t *loaded_pixels = load_png("test_output.png", &loaded_width, &loaded_height);

    assert(loaded_pixels != NULL);
    assert(loaded_width == width);
    assert(loaded_height == height);

    free(loaded_pixels);
    remove("test_output.png"); // Clean up
}

int main() {
    test_load_and_save_png();
    printf("All image_io tests passed!\n");
    return 0;
}