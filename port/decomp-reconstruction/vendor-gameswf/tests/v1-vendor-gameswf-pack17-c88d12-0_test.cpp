// Test for batch v1-vendor-gameswf-pack17-c88d12-0.
//   bash tools/trackb/check_tu.sh --test <this file> \
//     port/decomp-reconstruction/vendor-gameswf/upstream/gameswf-r1714/base/container.cpp \
//     port/decomp-reconstruction/vendor-gameswf/upstream/gameswf-r1714/base/utf8.cpp \
//     port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-gameswf_tu_string-39e5bd-0.cpp
// (the last three give tu_string its out-of-line members, as in the pack5 test: container.h's inline code uses them.)
//
// Covers the bucket-count rule of hash<>::set_raw_capacity that the tu_loadlib table (0x7bb580) uses:
// gameswf::dh2_hash::bucket_count in vendor-gameswf/gameswf_hash_dh2.hpp (inline). The rest of the batch needs the
// engine (malloc_internal, the object header, the counted buffer) and is checked by the integrator's build.
// Values are chosen away from the powers of two, where the single-precision log can round either way (see that header).

#include <cstdio>

#include "vendor-gameswf/gameswf_hash_dh2.hpp"

namespace {

int g_failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                              \
        }                                                              \
    } while (0)

// The rule returns at least 4 buckets for every request, including zero and one.
void test_minimum() {
    CHECK(gameswf::dh2_hash::bucket_count(0) == 4);
    CHECK(gameswf::dh2_hash::bucket_count(1) == 4);
    CHECK(gameswf::dh2_hash::bucket_count(2) == 4);
    CHECK(gameswf::dh2_hash::bucket_count(4) == 4);
}

// Away from the boundaries: 1 << (int)(log2(n - 1) + 1).
void test_values() {
    CHECK(gameswf::dh2_hash::bucket_count(6) == 8);     // log2(5) = 2.32 -> 3 -> 8
    CHECK(gameswf::dh2_hash::bucket_count(10) == 16);   // log2(9) = 3.17 -> 4 -> 16
    CHECK(gameswf::dh2_hash::bucket_count(20) == 32);   // log2(19) = 4.25 -> 5 -> 32
    CHECK(gameswf::dh2_hash::bucket_count(100) == 128); // log2(99) = 6.63 -> 7 -> 128
    CHECK(gameswf::dh2_hash::bucket_count(1000) == 1024);  // log2(999) = 9.96 -> 10 -> 1024
}

// Every result is a power of two, at least 4, and at least the requested size's bucket floor (never fewer buckets than
// requested entries would need at this rule, which is never more than twice the request).
void test_power_of_two() {
    for (int n = 0; n <= 300; ++n) {
        const int b = gameswf::dh2_hash::bucket_count(n);
        CHECK(b >= 4);
        CHECK((b & (b - 1)) == 0);
        if (n > 4) {
            CHECK(b < 2 * n + 4);
        }
    }
}

}  // namespace

int main() {
    test_minimum();
    test_values();
    test_power_of_two();
    if (g_failures == 0) {
        std::printf("pack17 c88d12 bucket_count: all checks passed\n");
        return 0;
    }
    std::printf("pack17 c88d12 bucket_count: %d failure(s)\n", g_failures);
    return 1;
}
