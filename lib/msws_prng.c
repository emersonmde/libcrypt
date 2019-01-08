/*
 * A very simple Middle Square Weyl Sequence PRNG.
 *
 * See https://mswsrng.wixsite.com/rand for the full implementation. This includes the seed method to generate
 * suitable seeds that work best for the Weyl sequence.
 */

#include "msws_prng.h"


void msws_seed(msws_ctx *ctx, uint64_t seed) {
    ctx->s = (seed | 1) ^ 0xdbc8915fabd37257;
}

uint64_t msws_uint64(msws_ctx *ctx) {
    ctx->x *= ctx->x;
    ctx->x += (ctx->w += ctx->s);
    return ctx->x = (ctx->x >> 32) | (ctx->x << 32);
}