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
    size_t w;
    // ?? - log2(b/25)
    size_t l;
} KECCAK_CTX;

void theta(const uint64_t **a, uint64_t **b, KECCAK_CTX *ctx);
void rho(const uint64_t **a, uint64_t **b, KECCAK_CTX *ctx);
void pi(const uint64_t **a, uint64_t **b, KECCAK_CTX *ctx);
void chi(const uint64_t **a, uint64_t **b, KECCAK_CTX *ctx);
void iota(const uint64_t **a, uint64_t **b, int nr, KECCAK_CTX *ctx);

#endif //LIBCRYPTO_KECCAK_H
