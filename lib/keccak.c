/*
 *
 * Round Functions:
 *      theta
 *      rho
 *      pi
 *      chi
 *      iota
 */

#include "keccak.h"

//void theta(const uint64_t **a, uint64_t **b, KECCAK_CTX *ctx) {
//    uint8_t x, y;
//    uint64_t z, c1, c2;
//    size_t w = ctx->w;
//
//    uint8_t temp;
//
//
//    for (x = 0; x < 5; x++) {
//        for (y = 0; y < 5; y++) {
//            for (z = 0; z < w; z++) {
//                c1 = (a[x - 1 % 5][0] & (1 << z)) ^ (a[x - 1 % 5][1] & (1 << z)) ^ (a[x - 1 % 5][2] & (1 << z)) ^ (a[x - 1 % 5][3] & (1 << z)) ^ (a[x - 1 % 5][4] & (1 << z));
//                c2 = (a[x + 1 % 5][0] & (1 << (z - 1 % 5))) ^ (a[x + 1 % 5][1] & (1 << (z - 1 % 5))) ^ (a[x + 1 % 5][2] & (1 << (z - 1 % 5))) ^ (a[x + 1 % 5][3] & (1 << (z - 1 % 5))) ^ (a[x + 1 % 5][4] & (1 << (z - 1 % 5)));
//                b[x][y] = (a[x][y] & (~(1 << z))) ^ ((c1 ^ c2) << z);
//            }
//        }
//    }
//}

void theta(const uint64_t **a, uint64_t **b, KECCAK_CTX *ctx) {
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
void rho(const uint64_t **a, uint64_t **b, KECCAK_CTX *ctx) {
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

void pi(const uint64_t **a, uint64_t **b, KECCAK_CTX *ctx) {

}

void chi(const uint64_t **a, uint64_t **b, KECCAK_CTX *ctx) {

}

void iota(const uint64_t **a, uint64_t **b, int nr, KECCAK_CTX *ctx) {

}
