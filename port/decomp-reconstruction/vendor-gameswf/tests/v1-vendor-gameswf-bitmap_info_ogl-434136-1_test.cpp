// Test for batch v1-vendor-gameswf-bitmap_info_ogl-434136-1 (gameswf::bitmap_info_ogl, part 2 of 2).
//   bash tools/trackb/check_tu.sh --test <this file>
// The class constructors and layout() need the glitch texture manager, which is not in the tree, so this test
// covers only the inline helpers in bitmap_info_ogl.hpp: the three pixel loops and the filter-bit update.
// The header has no out-of-line bodies, so no source is linked.

#include <cstdint>
#include <cstdio>

#include "vendor-gameswf/bitmap_info_ogl_pixels.hpp"

namespace {

int g_failures = 0;

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                                        \
        }                                                                        \
    } while (0)

namespace detail = gameswf::bitmap_ogl_detail;

void test_filter_table() {
    // s_filter_map read from the binary at 0x90c044: {0, 1, 2, 3, 4, 5}.
    CHECK(detail::kFilterMapCount == 6);
    for (int i = 0; i < detail::kFilterMapCount; ++i) {
        CHECK(detail::kFilterMap[i] == i);
    }
}

void test_alpha_expansion() {
    const std::uint8_t src[2] = {0x10, 0x80};
    std::uint8_t dst[8] = {};
    detail::expand_alpha8_to_argb(dst, src, 2);
    const std::uint8_t expected[8] = {0x10, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff};
    for (int i = 0; i < 8; ++i) {
        CHECK(dst[i] == expected[i]);
    }

    // A count of zero or less writes nothing.
    std::uint8_t untouched[4] = {1, 2, 3, 4};
    detail::expand_alpha8_to_argb(untouched, src, 0);
    CHECK(untouched[0] == 1 && untouched[3] == 4);
}

void test_rgba_expansion() {
    // RGBA in memory: R G B A. The binary writes A R G B.
    const std::uint8_t src[8] = {1, 2, 3, 4, 0x50, 0x60, 0x70, 0x80};
    std::uint8_t dst[8] = {};
    detail::expand_rgba_to_argb(dst, src, 2);
    const std::uint8_t expected[8] = {4, 1, 2, 3, 0x80, 0x50, 0x60, 0x70};
    for (int i = 0; i < 8; ++i) {
        CHECK(dst[i] == expected[i]);
    }
}

void test_rgb_expansion() {
    // RGB in memory: R G B. The binary writes 0xff then R G B.
    const std::uint8_t src[6] = {1, 2, 3, 0x40, 0x50, 0x60};
    std::uint8_t dst[8] = {};
    detail::expand_rgb_to_argb(dst, src, 2);
    const std::uint8_t expected[8] = {0xff, 1, 2, 3, 0xff, 0x40, 0x50, 0x60};
    for (int i = 0; i < 8; ++i) {
        CHECK(dst[i] == expected[i]);
    }
}

void test_filter_bits_defaults() {
    // Defaults: min index 1, mag index 1, one mip. Both words start at zero.
    std::uint32_t flags = 0;
    std::uint16_t dirty = 0;
    detail::apply_filter_bits(flags, dirty, 1, 1, 1);
    CHECK(flags == 0x9000u);  // min in bits 12..14 = 1, mag in bits 15..17 = 1
    CHECK(dirty == 0x0c);     // 0x4 min changed, 0x8 mag changed
}

void test_filter_bits_min_test_mips() {
    // Min index 2 with one mip: the min step needs mips > 1 or value <= 1, so it does not apply.
    std::uint32_t flags = 0;
    std::uint16_t dirty = 0;
    detail::apply_filter_bits(flags, dirty, 1, 2, 1);
    CHECK((flags & 0x7000u) == 0u);
    CHECK(dirty == 0x08);     // only the mag step

    // Same index with three mips: the min step applies.
    flags = 0;
    dirty = 0;
    detail::apply_filter_bits(flags, dirty, 3, 2, 1);
    CHECK(flags == 0xa000u);  // min 2 in bits 12..14, mag 1 in bits 15..17
    CHECK(dirty == 0x0c);
}

void test_filter_bits_already_set() {
    // Flags already hold min 1 and mag 1: nothing changes, no dirty bits are set.
    std::uint32_t flags = 0x9000u;
    std::uint16_t dirty = 0;
    detail::apply_filter_bits(flags, dirty, 1, 1, 1);
    CHECK(flags == 0x9000u);
    CHECK(dirty == 0);
}

void test_filter_bits_keep_other_fields() {
    // Bits outside the two filter fields are kept, and the dirty bits already set are kept.
    std::uint32_t flags = 0x1u | (0x3u << 3);
    std::uint16_t dirty = 0x10;
    detail::apply_filter_bits(flags, dirty, 1, 1, 1);
    CHECK((flags & 0x7u) == 0x1u);
    CHECK((flags & (0x3u << 3)) == (0x3u << 3));
    CHECK(dirty == 0x1c);
}

}  // namespace

int main() {
    test_filter_table();
    test_alpha_expansion();
    test_rgba_expansion();
    test_rgb_expansion();
    test_filter_bits_defaults();
    test_filter_bits_min_test_mips();
    test_filter_bits_already_set();
    test_filter_bits_keep_other_fields();
    if (g_failures != 0) {
        std::fprintf(stderr, "%d check(s) failed\n", g_failures);
        return 1;
    }
    std::printf("bitmap_info_ogl helper tests passed\n");
    return 0;
}
