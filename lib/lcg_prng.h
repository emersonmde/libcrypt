#ifndef LIBCRYPTO_LCG_PRNG_H
#define LIBCRYPTO_LCG_PRNG_H

#include <stdint.h>

typedef struct {
    uint64_t state, m, a, c;
} lcg_ctx;

uint64_t lcg_uint64(lcg_ctx *ctx);

void lcg_seed(uint64_t seed, lcg_ctx *ctx);

#endif //LIBCRYPTO_LCG_PRNG_H
