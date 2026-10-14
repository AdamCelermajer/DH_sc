// Checks for glitch::video::CImage::fill (0x5ff9b4): the per-format colour packing and the word loop that
// writes the whole data block. The packing is what the original's inline table in PFDTable rows 5, 8, 12, 13
// and 14 describes, and these checks read the bytes back, so a wrong channel order fails here.
//
// NOT RUN: CImage's data constructors (0x6021f8 .. 0x602938) and initData (0x601988) are still `skipped`, so
// there is no way to build an image over a caller's buffer yet. This file is kept as the checks to enable the
// moment that constructor lands; it is not registered with add_test until then.
#include "glitch-video/image.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {

#if defined(DH2_IMAGE_FILL_TEST_HAS_CTOR)
// 0x5ff9b4 packs with the bytes of SColor: component 0 is the low byte of `color`.
glitch::video::SColor rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
    glitch::video::SColor color;
    color.setComponent(0, r);
    color.setComponent(1, g);
    color.setComponent(2, b);
    color.setComponent(3, a);
    return color;
}

std::vector<std::uint8_t> filled(glitch::video::E_PIXEL_FORMAT format, std::uint32_t size,
                                 const glitch::video::SColor& color) {
    std::vector<std::uint8_t> bytes(size, 0);
    glitch::video::CImage image(format, glitch::core::dimension2d<std::int32_t>{1, 1}, bytes.data(), false, false);
    image.fill(color);
    return bytes;
}

std::uint32_t word(const std::vector<std::uint8_t>& bytes, std::size_t index) {
    std::uint32_t value = 0;
    std::memcpy(&value, bytes.data() + 4 * index, 4);
    return value;
}
#endif

}  // namespace

int main() {
#if !defined(DH2_IMAGE_FILL_TEST_HAS_CTOR)
    std::printf("image_fill: skipped (CImage's data constructor is not reconstructed yet)\n");
    return 0;
#else
    const glitch::video::SColor color = rgb(0x12, 0x34, 0x56, 0xFF);

    // Format 5, R5G6B5: halfword (R >> 3) << 11 | (G >> 2) << 5 | (B >> 3), replicated into both halves.
    {
        const auto bytes = filled(5, 16, color);
        const std::uint32_t half = 2u | (13u << 5) | (10u << 11);
        CHECK(word(bytes, 0) == (half | (half << 16)));
        CHECK(word(bytes, 1) == (half | (half << 16)));
        CHECK(word(bytes, 3) == (half | (half << 16)));
    }

    // Format 8, A1R5G5B5: halfword (A >> 7) << 15 | (R >> 3) << 10 | (G >> 3) << 5 | (B >> 3).
    {
        const auto bytes = filled(8, 16, color);
        const std::uint32_t half = 2u | (2u << 5) | (0x12u >> 3 << 10) | (1u << 15);
        CHECK(word(bytes, 0) == (half | (half << 16)));
    }

    // Format 12: bytes r, g, b, a.
    {
        const auto bytes = filled(12, 8, color);
        CHECK(word(bytes, 0) == 0xFF563412u);
        CHECK(word(bytes, 1) == 0xFF563412u);
    }

    // Format 13: bytes a, r, g, b.
    {
        const auto bytes = filled(13, 8, color);
        CHECK(word(bytes, 0) == 0x3412FF56u);
    }

    // Format 14: bytes g, b, a, r.
    {
        const auto bytes = filled(14, 8, color);
        CHECK(word(bytes, 0) == 0x56FF3412u);
    }

    // The loop writes (m_dataSize >> 2) & 7 words after the whole 8-word blocks: 36 bytes = 8 words,
    // which is one full block, so only bytes 0..31 are written.
    {
        const auto bytes = filled(12, 36, color);
        CHECK(word(bytes, 7) == 0xFF563412u);
        for (std::size_t i = 32; i < bytes.size(); ++i) {
            CHECK(bytes[i] == 0);
        }
    }

    // 44 bytes = 11 words, so 8 in the block and 3 in the tail.
    {
        const auto bytes = filled(12, 44, color);
        CHECK(word(bytes, 10) == 0xFF563412u);
    }

    // A format outside the five leaves the data untouched.
    {
        const auto bytes = filled(2, 8, color);
        CHECK(word(bytes, 0) == 0u);
    }

    std::printf("%s\n", failures ? "image_fill: FAILED" : "image_fill: ok");
    return failures != 0;
#endif
}
