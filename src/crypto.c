
#include "crypto.h"
#include "aes.h"
#include <stdlib.h>
#include <string.h>

#define AES_BLOCK_SIZE 16
#define AES256 1
#define CBC 1

static void derive_key(const char *passphrase, uint8_t *key) {
    size_t len = strlen(passphrase);
    memset(key, 0, 32);
    if (len > 32) len = 32;
    memcpy(key, passphrase, len);
}

static void pkcs7_pad(uint8_t *data, size_t data_len, size_t padded_len) {
    uint8_t pad_value = padded_len - data_len;
    for (size_t i = data_len; i < padded_len; ++i) {
        data[i] = pad_value;
    }
}

static size_t pkcs7_unpad(uint8_t *data, size_t data_len) {
    if (data_len == 0) return 0;
    uint8_t pad_value = data[data_len - 1];
    if (pad_value > AES_BLOCK_SIZE || pad_value == 0) return data_len;
    for (size_t i = data_len - pad_value; i < data_len; ++i) {
        if (data[i] != pad_value) return data_len;
    }
    return data_len - pad_value;
}

int aes_encrypt(const uint8_t *input, size_t input_len, const char *passphrase, uint8_t **output, size_t *output_len) {
    uint8_t key[32];
    uint8_t iv[16] = {0};

    derive_key(passphrase, key);

    size_t padded_len = ((input_len / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;
    uint8_t *buffer = malloc(padded_len);
    if (!buffer) return -1;

    memcpy(buffer, input, input_len);
    pkcs7_pad(buffer, input_len, padded_len);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, buffer, padded_len);

    *output = buffer;
    *output_len = padded_len;
    return 0;
}

int aes_decrypt(const uint8_t *input, size_t input_len, const char *passphrase, uint8_t **output, size_t *output_len) {
    if (input_len % AES_BLOCK_SIZE != 0) return -1;

    uint8_t key[32];
    uint8_t iv[16] = {0};

    derive_key(passphrase, key);

    uint8_t *buffer = malloc(input_len);
    if (!buffer) return -1;

    memcpy(buffer, input, input_len);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, buffer, input_len);

    size_t unpadded_len = pkcs7_unpad(buffer, input_len);
    *output = buffer;
    *output_len = unpadded_len;
    return 0;
}
