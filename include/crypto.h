
#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <stddef.h>

int aes_encrypt(const uint8_t *input, size_t input_len, const char *passphrase, uint8_t **output, size_t *output_len);
int aes_decrypt(const uint8_t *input, size_t input_len, const char *passphrase, uint8_t **output, size_t *output_len);

#endif
