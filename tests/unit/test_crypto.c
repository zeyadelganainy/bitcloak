#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "crypto.h"

// Test derive_key function
void test_derive_key() {
    uint8_t key[32];
    const char *passphrase = "test_passphrase";

    derive_key(passphrase, key);

    // Ensure the key is derived correctly
    assert(memcmp(key, passphrase, strlen(passphrase)) == 0);
    for (size_t i = strlen(passphrase); i < 32; ++i) {
        assert(key[i] == 0); // Ensure the rest of the key is zero-padded
    }

    printf("test_derive_key passed!\n");
}

// Test pkcs7_pad function
void test_pkcs7_pad() {
    uint8_t data[16] = "Hello";
    size_t data_len = 5;
    size_t padded_len = 16;

    pkcs7_pad(data, data_len, padded_len);

    // Ensure the padding is applied correctly
    for (size_t i = data_len; i < padded_len; ++i) {
        assert(data[i] == (padded_len - data_len));
    }

    printf("test_pkcs7_pad passed!\n");
}

// Test pkcs7_unpad function
void test_pkcs7_unpad() {
    uint8_t data[16] = "Hello";
    size_t data_len = 16;
    size_t padded_len = 16;

    pkcs7_pad(data, 5, padded_len);
    size_t unpadded_len = pkcs7_unpad(data, data_len);

    // Ensure the unpadded length is correct
    assert(unpadded_len == 5);

    printf("test_pkcs7_unpad passed!\n");
}

// Test aes_encrypt and aes_decrypt functions
void test_aes_encrypt_decrypt() {
    const char *message = "Hello, BitCloak!";
    const char *passphrase = "test_passphrase";
    uint8_t *encrypted = NULL;
    uint8_t *decrypted = NULL;
    size_t encrypted_size = 0, decrypted_size = 0;

    // Encrypt the message
    int encrypt_result = aes_encrypt((uint8_t *)message, strlen(message), passphrase, &encrypted, &encrypted_size);
    assert(encrypt_result == 0);
    assert(encrypted != NULL);
    assert(encrypted_size > 0);

    // Decrypt the message
    int decrypt_result = aes_decrypt(encrypted, encrypted_size, passphrase, &decrypted, &decrypted_size);
    assert(decrypt_result == 0);
    assert(decrypted != NULL);
    assert(decrypted_size == strlen(message));
    assert(memcmp(decrypted, message, decrypted_size) == 0);

    // Free allocated memory
    free(encrypted);
    free(decrypted);

    printf("test_aes_encrypt_decrypt passed!\n");
}

int main() {
    test_derive_key();
    test_pkcs7_pad();
    test_pkcs7_unpad();
    test_aes_encrypt_decrypt();

    printf("All crypto tests passed!\n");
    return 0;
}