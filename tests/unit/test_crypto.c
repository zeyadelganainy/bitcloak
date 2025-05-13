#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "crypto.h"


void test_xor_cipher() {
    uint8_t data[] = "Hello, BitCloak!";
    const char *key = "key";
    size_t size = strlen((char *)data);

    xor_cipher(data, size, key);
    xor_cipher(data, size, key); // XOR again to decrypt
    assert(strcmp((char *)data, "Hello, BitCloak!") == 0); // Ensure data matches
}

void test_aes_encrypt_decrypt() {
    uint8_t data[] = "Hello, BitCloak!";
    uint8_t *encrypted = NULL;
    uint8_t *decrypted = NULL;
    size_t encrypted_size = 0, decrypted_size = 0;

    aes_encrypt(data, strlen((char *)data), "key", &encrypted, &encrypted_size);
    aes_decrypt(encrypted, encrypted_size, "key", &decrypted, &decrypted_size);

    assert(decrypted_size == strlen((char *)data));
    assert(strcmp((char *)decrypted, "Hello, BitCloak!") == 0);

    free(encrypted);
    free(decrypted);
}

int main() {
    test_xor_cipher();
    test_aes_encrypt_decrypt();
    printf("All crypto tests passed!\n");
    return 0;
}