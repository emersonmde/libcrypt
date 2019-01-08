/*
 * A simple implentation of a linear congruential generator
 *
 * The starting constants are from the book "Numerical Recipes" and seem to work
 * fairly well to produce random looking input before repeating. For more information
 * see https://en.wikipedia.org/wiki/Linear_congruential_generator
 */

#include "lcg_prng.h"


void lcg_seed(uint64_t seed, lcg_ctx *ctx) {
    ctx->state = seed;
    ctx->c = 1013904223;
    ctx->a = 1664525;
    ctx->m = ~(uint64_t) 0;
}

uint64_t lcg_uint64(lcg_ctx *ctx) {
    return ctx->state = (ctx->a * ctx->state + ctx->c) & ctx->m;
}