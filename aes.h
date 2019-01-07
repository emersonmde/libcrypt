#ifndef LIBCRYPTO_AES_H
#define LIBCRYPTO_AES_H

#include <stdint.h>
#include <stdio.h>

#define AES_BLOCK_SIZE 16

typedef struct {
    size_t size;
    uint8_t round_keys[176];
} AES_KEY;

void aes_expand_key(uint8_t *expanded_key, const uint8_t *key);

void aes_set_key(const uint8_t *key_in, AES_KEY *key_out, size_t len);

void aes_encrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key);

void aes_decrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key);

void aes_encrypt_cbc(const uint8_t *in, uint8_t *out, size_t len, const AES_KEY *key, const uint8_t *iv);

void aes_cbc_decrypt(const uint8_t *in, uint8_t *out, const size_t len, const AES_KEY *key, const uint8_t *iv);

#endif