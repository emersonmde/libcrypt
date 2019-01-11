/*
 *
 * Round Functions:
 *      theta
 *      rho
 *      pi
 *      chi
 *      iota
 */

#include <math.h>
#include "keccak.h"

// Returns a with the zth bit set to b
static inline uint64_t set_bit(const uint64_t a, const uint8_t z, const uint8_t b) {
    return (a & (~(1ULL << z))) | (b << z);
}

// Returns the zth bit of a
static inline uint8_t get_bit(const uint64_t a, const uint8_t z) {
    return (uint8_t)((a >> z) & 1U);
}

// TODO: Change to use get/set bit
void theta(uint64_t a[5][5], uint64_t b[5][5], KECCAK_CTX *ctx) {
    uint8_t x, y;
    uint64_t z;

    size_t w = ctx->w;

    // Build a lookup table of column XOR sums
    // For all pairs (x, z) such that 0 ≤ x < 5 and 0 ≤ z < w, let
    // C[x,z]=a[x,0,z] ⊕ a[x,1,z] ⊕ a[x,2,z] ⊕ a[x,3,z] ⊕ a[x,4,z]
    uint64_t c[5];
    for (x = 0; x < 5; x++) {
        for (z = 0; z < w; z++) {
            c[x] = (c[x] & (~(1 << z))) | (((a[x][0] & (1 << z)) ^ (a[x][1] & (1 << z)) ^ (a[x][2] & (1 << z)) ^ (a[x][3] & (1 << z)) ^ (a[x][4] & (1 << z))) << z);
        }
    }

    // Build a lookup table of the xor sum of two column xor sums
    // For all pairs (x, z) such that 0 ≤ x < 5 and 0 ≤ z < w let
    // D[x, z]=C[(x-1) mod 5, z] ⊕ C[(x+1) mod 5, (z – 1) mod w]
    uint64_t d[5];
    for (x = 0; x < 5; x++) {
        for (z = 0; z < w; z++) {
            d[x] = (d[x] & (~(1 << z))) | (((c[x - 1 % 5] & (1 << z)) ^ ((c[x + 1] & (1 << (z - 1 % 5))))) << z);
        }
    }

    // Assign the correct bit in the output by using the d lookup table
    // For all triples (x, y, z) such that 0 ≤ x < 5, 0 ≤ y < 5, and 0 ≤ z < w, let
    // b[x,y,z] = a[x,y,z] ⊕ D[x,z]
    for (x = 0; x < 5; x++) {
        for (y = 0; y < 5; y++) {
            for (z = 0; z < w; z++) {
                b[x][y] = (a[x][y] & (~(1 << z))) | ((d[x] & (1 << z)) << z);
            }
        }
    }
}

// Rotate each bit in lane
// For all z such that 0≤z<w, let A′ [0,0,z] = A[0,0,z]
// Let(x,y)=(1,0)
// For t from 0 to 23:
//      for all z such that 0≤z<w, let A′[x, y, z] = A[x, y, (z–(t+1)(t+2)/2) mod w]
//      let (x, y) = (y, (2x+3y) mod 5)
// TODO: Change to use get/set bit
void rho(uint64_t a[5][5], uint64_t b[5][5], KECCAK_CTX *ctx) {
    uint8_t x, y, t, temp;
    uint64_t z, zi;

    size_t w = ctx->w;

    b[0][0] = a[0][0];
    x = 1;
    y = 0;
    for (t = 0; t < 24; t++) {
        for (z = 0; z < w; z++) {
            zi = (z - (t + 1) * (t + 2) / 2) % w;
            b[x][y] = (a[x][y] & (~(1 << z))) | (((a[x][y] >> zi) & 1) << z);
            temp = y;
            y = (((uint8_t)2 * x) + ((uint8_t)3 * y)) % (uint8_t)5;
            x = temp;
        }
    }

}

// For all triples (x, y, z) such that 0 ≤ x < 5, 0 ≤ y < 5, and 0 ≤ z < w, let
//      b[x, y, z]= a[(x + 3y) mod 5, x, z]
void pi(uint64_t a[5][5], uint64_t b[5][5], KECCAK_CTX *ctx) {
    uint8_t x, y;
    uint8_t z;

    size_t w = ctx->w;

    for (x = 0; x < 5; x++) {
        for (y = 0; y < 5; y++) {
            for (z = 0; z < w; z++) {
                b[x][y] = set_bit(b[x][y], z, get_bit(a[(x + (3 * y)) % 5][x], z));
            }
        }
    }
}

// For all triples (x, y, z) such that 0 ≤ x < 5, 0 ≤ y < 5, and 0 ≤ z < w, let
//      b[x,y,z] = a[x,y,z] ⊕ ((a[(x+1) mod 5, y, z] ⊕ 1) ⋅ a[(x+2) mod 5, y, z])
void chi(uint64_t a[5][5], uint64_t b[5][5], KECCAK_CTX *ctx) {
    uint8_t x, y, z, a1, a2, a3;

    size_t w = ctx->w;

    for (x = 0; x < 5; x++) {
        for (y = 0; y < 5; y++) {
            for (z = 0; z < w; z++) {
                a1 = get_bit(a[x][y], z);
                a2 = get_bit(a[(x + 1) % 5][y], z) ^ (uint8_t)1;
                a3 = get_bit(a[(x + 2) % 5][y], z);
                b[x][y] = set_bit(b[x][y], z, a1 ^ (a2 & a3));
            }
        }
    }
}

// Generate the round constant using a linear feedback shift register
uint8_t round_constant(uint8_t t) {
    if (t % 255 == 0)
        return (uint8_t)1;

    uint64_t r = 10000000U;
    for (int i = 1; i <= t % 255; i++) {
        r = 0 | r;
        r = set_bit(r, 0, get_bit(r, 0) ^ get_bit(r, 8));
        r = set_bit(r, 4, get_bit(r, 4) ^ get_bit(r, 8));
        r = set_bit(r, 5, get_bit(r, 5) ^ get_bit(r, 8));
        r = set_bit(r, 6, get_bit(r, 6) ^ get_bit(r, 8));
    }
    return get_bit(r, 0);
}

// For all triples (x, y, z) such that 0≤x<5, 0≤y<5, and 0≤z<w, let A′[x, y, z] = A[x, y, z]
// LetRC=0ULL
// For j from 0 to l, let RC[2j –1]=rc(j+7ir)
// For all z such that 0≤z<w, let A′[0,0,z]=A′[0,0,z] ⊕ RC[z]
// Return A′
void iota(uint64_t a[5][5], uint64_t b[5][5], uint8_t ir, KECCAK_CTX *ctx) {
    uint8_t x, y, z, j;
    uint64_t rc;

    size_t w = ctx->w;

    for (x = 0; x < 5; x++) {
        for (y = 0; y < 5; y++) {
            for (z = 0; z < w; z++) {
                b[x][y] = set_bit(b[x][y], z, get_bit(a[x][y], z));
            }
        }
    }

    rc = 0;
    for (j = 0; j <= ctx->l; j++) {
        rc = set_bit(rc, ((uint8_t)2 << j) - (uint8_t)1, round_constant(j + (uint8_t)7 * ir));
    }

    for (z = 0; z < w; z++) {
        b[0][0] = set_bit(b[0][0], z, get_bit(b[0][0], z) ^ get_bit(rc, z));
    }
}

void rnd(uint64_t a[5][5], uint8_t ir, KECCAK_CTX *ctx) {
    uint8_t x, y, z;
    size_t w = ctx->w;
    uint64_t b[5][5];

    for (x = 0; x < 5; x++) {
        for (y = 0; y < 5; y++) {
            b[x][y] = 0;
        }
    }

    theta(a, b, ctx);
    rho(b, a, ctx);
    pi(a, b, ctx);
    chi(b, a, ctx);
    iota(a, b, ir, ctx);

    for (x = 0; x < 5; x++) {
        for (y = 0; y < 5; y++) {
            a[x][y] = b[x][y];
        }
    }
}

void keccakf(const uint64_t *in, uint64_t *out) {
    uint8_t x, y, i;
    uint64_t a[5][5];
    KECCAK_CTX ctx;

    ctx.b = 1600;
    ctx.nr = 24;
    ctx.w = 64;
    ctx.l = 6;

    i = 0;
    for (x = 0; x < 5; x++) {
        for (y = 0; y <5; y++) {
            a[x][y] = in[i++];
        }
    }

    for (i = (uint8_t)(12 + 2 * ctx.l - ctx.nr); i <= 12 + 2 * ctx.l - 1; i++) {
        rnd(a, i, &ctx);
    }

    i = 0;
    for (x = 0; x < 5; x++) {
        for (y = 0; y <5; y++) {
            out[i++] = a[x][y];
        }
    }
}
