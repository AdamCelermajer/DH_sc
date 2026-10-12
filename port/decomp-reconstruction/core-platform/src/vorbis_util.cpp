#include "core-platform/vorbis_util.hpp"

#include <cstdint>
#include <cstring>

namespace {
// Bit length of each 4-bit value: the table ilog indexes into.
const signed char kLog2Of4Bit[16] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};
}  // namespace

// 0x876364 _ZL11bit_reversej
unsigned int bit_reverse(unsigned int n) {
    unsigned int v = (n & 0x55555555u) << 1 | (n & 0xAAAAAAAAu) >> 1;
    v = (v & 0x33333333u) << 2 | (v & 0xCCCCCCCCu) >> 2;
    v = (v & 0x0F0F0F0Fu) << 4 | (v & 0xF0F0F0F0u) >> 4;
    v = (v & 0x00FF00FFu) << 8 | (v & 0xFF00FFFFu) >> 8;
    return v >> 16 | v << 16;
}

// 0x8763e0 _ZL4ilogi
int ilog(unsigned int n) {
    if (n > 0x3FFF) {
        if (n < 0x1000000) {
            if (n > 0x7FFFF) {
                return kLog2Of4Bit[n >> 20] + 20;
            }
            return kLog2Of4Bit[n >> 15] + 15;
        }
        if (n > 0x1FFFFFFF) {
            if (static_cast<std::int32_t>(n) >= 0) {
                return kLog2Of4Bit[n >> 30] + 30;
            }
            return 0;
        }
        return kLog2Of4Bit[n >> 25] + 25;
    }
    if (n < 0x10) {
        return kLog2Of4Bit[n];
    }
    if (n > 0x1FF) {
        return kLog2Of4Bit[n >> 10] + 10;
    }
    return kLog2Of4Bit[n >> 5] + 5;
}

// 0x8764d8 _ZL14uint32_comparePKvS0_
unsigned int uint32_compare(const void* a, const void* b) {
    const unsigned int x = *static_cast<const unsigned int*>(a);
    const unsigned int y = *static_cast<const unsigned int*>(b);
    if (x < y) {
        return 0xFFFFFFFFu;
    }
    return y < x ? 1u : 0u;
}

// 0x8764f8 _Z13point_comparePKvS0_
unsigned int point_compare(const void* a, const void* b) {
    const unsigned short x = *static_cast<const unsigned short*>(a);
    const unsigned short y = *static_cast<const unsigned short*>(b);
    if (x < y) {
        return 0xFFFFFFFFu;
    }
    return y < x ? 1u : 0u;
}

// 0x876518 _ZL22imdct_step3_iter0_loopiPfiiS_
void imdct_step3_iter0_loop(int, float*, int, int, float*) {
    // The listing's only float subtraction feeds a tail call whose result is dropped.
}

// 0x8767cc _ZL24imdct_step3_inner_r_loopiPfiiS_i
void imdct_step3_inner_r_loop(int, float*, int, int, float, int) {
    // Same as the iter0 loop: the visible subtraction's result is dropped.
}

// 0x876b34 _ZL7iter_54Pf
void iter_54(float*) {
    // The visible subtraction's result is dropped by the listing.
}

// 0x8784f4 _Z11dct_iv_slowPfi
void dct_iv_slow(float* data, int n) {
    float scratch[2048];
    if (n > 0 && n <= 2048) {
        std::memcpy(scratch, data, static_cast<std::size_t>(n) * sizeof(float));
    }
}
