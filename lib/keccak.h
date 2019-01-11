/*
 *
 * Round Functions:
 *      theta
 *      rho
 *      pi
 *      chi
 *      iota
 */

#ifndef LIBCRYPTO_KECCAK_H
#define LIBCRYPTO_KECCAK_H

#include <stdio.h>
#include <stdint.h>

typedef uint64_t **KECCAK_STATE;

typedef struct {
    // Holds state A (usually input)
    KECCAK_STATE A;
    // Holds state A' (usually output)
    KECCAK_STATE TEMP;
    // Width - defined as "The total length of the strings permuted" aka block size
    size_t b;
    // Number of rounds
    int nr;
    // Word length - b/25
    uint8_t w;
    // ?? - log2(b/25)
    uint8_t l;
    // State
    uint64_t **a;
} KECCAK_CTX;

void theta(uint64_t a[5][5], uint64_t b[5][5], KECCAK_CTX *ctx);
void rho(uint64_t a[5][5], uint64_t b[5][5], KECCAK_CTX *ctx);
void pi(uint64_t a[5][5], uint64_t b[5][5], KECCAK_CTX *ctx);
void chi(uint64_t a[5][5], uint64_t b[5][5], KECCAK_CTX *ctx);
void iota(uint64_t a[5][5], uint64_t b[5][5], uint8_t ir, KECCAK_CTX *ctx);

void keccakf(const uint64_t *in, uint64_t *out);

#endif //LIBCRYPTO_KECCAK_H
