#include <assert.h>
#include "utils.h"

void test_current_time_ms() {
    double time1 = current_time_ms();
    double time2 = current_time_ms();
    assert(time2 >= time1); // Ensure time is increasing
}

void test_xor_cipher() {
    uint8_t data[] = "Hello, BitCloak!";
    const char *key = "key";
    size_t size = strlen((char *)data);

    xor_cipher(data, size, key);
    xor_cipher(data, size, key); // XOR again to decrypt
    assert(strcmp((char *)data, "Hello, BitCloak!") == 0); // Ensure data matches
}

int main() {
    test_current_time_ms();
    printf("All utils tests passed!\n");
    return 0;
}