// Helpers from the Ogg Vorbis decoder that the original embeds (stb_vorbis-style
// names). They are file-local in the original; here they have external linkage.
#pragma once

// 0x876364 bit_reverse: reverses the 32 bits of n.
unsigned int bit_reverse(unsigned int n);

// 0x8763e0 ilog: number of significant bits of n; 0 when n has its top bit set.
int ilog(unsigned int n);

// 0x8764d8 uint32_compare / 0x8764f8 point_compare: qsort-style three-way compare.
// Returns 0xffffffff when a < b, 1 when a > b, 0 when equal.
unsigned int uint32_compare(const void* a, const void* b);
unsigned int point_compare(const void* a, const void* b);

// 0x876518 imdct_step3_iter0_loop. Partial: the listing reduces to one float
// subtraction whose result is discarded, so this is a no-op; the transform loop
// is not visible in the pseudocode.
void imdct_step3_iter0_loop(int n, float* e, int iOffset, int kOffset, float* a);

// 0x8767cc imdct_step3_inner_r_loop. Partial, same situation as the iter0 loop.
void imdct_step3_inner_r_loop(int n, float* e, int iOffset, int kOffset, float a, int stride);

// 0x876b34 iter_54. Partial: one float subtraction whose result is discarded.
void iter_54(float* z);

// 0x8784f4 dct_iv_slow. Partial: the listing only copies n floats into a stack
// buffer before the tail call; the DCT itself is not visible.
void dct_iv_slow(float* data, int n);
