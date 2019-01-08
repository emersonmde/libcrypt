/*
 * A very simple Middle Square Weyl Sequence PRNG.
 *
 * See https://mswsrng.wixsite.com/rand for the full implementation. This includes the seed method to generate
 * suitable seeds that work best for the Weyl sequence.
 */

#ifndef LIBCRYPTO_MSWS_PRNG_H
#define LIBCRYPTO_MSWS_PRNG_H

#include <stdint.h>

typedef struct {
    uint64_t x, w, s;
} msws_ctx;

void msws_prng_seed(msws_ctx *ctx, uint64_t s);

uint64_t msws_prng_next(msws_ctx *ctx);

#endif //LIBCRYPTO_MSWS_PRNG_H
