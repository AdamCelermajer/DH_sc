// Checks for glitch::video::CImage::fill (0x5ff9b4): the per-format colour packing and the word loop that
// writes the data block.
//
// The packings were read off the binary's switch and then confirmed by building an image over a caller's buffer
// and printing the bytes back. Expectations are written BOTH as explicit bytes at offsets 0..3 and as the
// little-endian word the buffer holds, because the byte at offset 0 is the easy thing to get backwards: for
// format 12 it is SColor component 3, not component 0.
//
// Write loop (confirmed in the listing, not inferred): `m_dataSize >> 5` passes of eight words (the eight STR at
// 0x5ffa3c..0x5ffa58, ADD R3,#0x20 at 0x5ffa5c) followed by `(m_dataSize >> 2) & 7` words (UBFX R3,R12,#2,#3 at
// 0x5ffa68). m_dataSize is a BYTE count for every format here (format 5 at 8x8 measures 128), so the loop is
// simply "fill the block in 8-word strides, then the remainder in words". A block smaller than one word (e.g.
// a 1x1 16-bit image, 2 bytes) therefore writes nothing - that is the binary's shape, not a defect.
#include "glitch-video/image.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {

// SColor components 0..3 are bytes 0..3 of the word: a=0x12, b=0x34, c=0x56, r(alpha)=0xFF.
glitch::video::SColor sampleColor() {
    glitch::video::SColor color;
    color.setComponent(0, 0x12);
    color.setComponent(1, 0x34);
    color.setComponent(2, 0x56);
    color.setComponent(3, 0xFF);
    return color;
}

// Builds an image of the given pixel size over a caller-owned buffer and fills it.
std::vector<std::uint8_t> filled(glitch::video::E_PIXEL_FORMAT format, std::int32_t width, std::int32_t height,
                                 const glitch::video::SColor& color, std::uint32_t extraBytes = 0) {
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(width) * height * 4 + extraBytes, 0);
    glitch::video::CImage image(format, glitch::core::dimension2d<std::int32_t>{width, height}, bytes.data(), false, false);
    image.fill(color);
    return bytes;
}

std::uint32_t word(const std::vector<std::uint8_t>& bytes, std::size_t index) {
    std::uint32_t value = 0;
    std::memcpy(&value, bytes.data() + 4 * index, 4);
    return value;
}

}  // namespace

int main() {
    const glitch::video::SColor color = sampleColor();
    CHECK(color.color == 0xFF563412u);  // component 0 is the LOW byte

    // Format 12 (masks 0x0000ff00 / 0x00ff0000 / 0xff000000 / 0x000000ff): offset 0 gets component 3.
    {
        const auto bytes = filled(12, 4, 4, color);
        CHECK(bytes[0] == 0xFF && bytes[1] == 0x12 && bytes[2] == 0x34 && bytes[3] == 0x56);
        CHECK(word(bytes, 0) == 0x563412FFu);
        CHECK(word(bytes, 15) == 0x563412FFu);  // the whole 64-byte block is written
    }

    // Format 13 (masks 0x00ff0000 / 0x0000ff00 / 0x000000ff / 0xff000000): offset 0 gets component 2.
    {
        const auto bytes = filled(13, 4, 4, color);
        CHECK(bytes[0] == 0x56 && bytes[1] == 0xFF && bytes[2] == 0x12 && bytes[3] == 0x34);
        CHECK(word(bytes, 0) == 0x3412FF56u);
    }

    // Format 14 (masks 0x000000ff / 0x0000ff00 / 0x00ff0000 / 0xff000000): offset 0 gets component 1.
    {
        const auto bytes = filled(14, 4, 4, color);
        CHECK(bytes[0] == 0x34 && bytes[1] == 0x56 && bytes[2] == 0xFF && bytes[3] == 0x12);
        CHECK(word(bytes, 0) == 0x12FF5634u);
    }

    // Format 5, R5G6B5: halfword (a>>3)<<11 | (b>>2)<<5 | (c>>3), replicated into both halves of the word.
    // A 8x8 image is 16 halfwords per row * 8 rows = 128 bytes, so the whole block is written.
    {
        const std::uint32_t half = ((0x12u >> 3) << 11) | ((0x34u >> 2) << 5) | (0x56u >> 3);
        const auto bytes = filled(5, 8, 8, color);
        CHECK(word(bytes, 0) == (half | (half << 16)));
        CHECK(word(bytes, 31) == (half | (half << 16)));
    }

    // Format 8, A1R5G5B5: halfword (r>>7)<<15 | (a>>3)<<10 | (b>>3)<<5 | (c>>3), replicated.
    {
        const std::uint32_t half = ((0xFFu >> 7) << 15) | ((0x12u >> 3) << 10) | ((0x34u >> 3) << 5) | (0x56u >> 3);
        const auto bytes = filled(8, 8, 8, color);
        CHECK(word(bytes, 0) == (half | (half << 16)));
    }

    // A 1x1 16-bit image has a 2-byte block: no whole word pair and a tail of 0, so fill writes nothing.
    {
        const auto bytes = filled(5, 1, 1, color);
        CHECK(word(bytes, 0) == 0u);
    }

    // A format outside the five writes nothing.
    {
        const auto bytes = filled(2, 4, 4, color);
        CHECK(word(bytes, 0) == 0u);
    }

    // The owning constructors (0x602028 and 0x602110) allocate their own block. Format 12 is 4 bytes per
    // pixel, so 4x4 is a 16-byte pitch and a 64-byte block; the reference count starts at 0, not at the
    // IReferenceCounted default of 1, because that is what every original constructor stores.
    {
        glitch::video::CImage owned(12, glitch::core::dimension2d<std::int32_t>{4, 4});
        CHECK(owned.referenceCounter == 0);
        CHECK(owned.m_data != nullptr);
        CHECK(owned.m_pitch == 16);
        CHECK(owned.m_dataSize == 64);
        CHECK(owned.m_ownsData == 1);
        CHECK(owned.m_hasMipmaps == 0);
        CHECK(owned.m_mipData == nullptr);  // no mip chain without the flag
        owned.fill(color);
        CHECK(word(std::vector<std::uint8_t>(owned.m_data, owned.m_data + 64), 0) == 0x563412FFu);
    }
    {
        glitch::video::CImage mip(5, glitch::core::dimension2d<std::int32_t>{4, 4}, true);
        CHECK(mip.referenceCounter == 0);
        CHECK(mip.m_hasMipmaps == 1);
        CHECK(mip.m_mipData != nullptr);       // a chain was built
        CHECK(mip.m_mipData[mip.m_mipLevels] == nullptr);  // the chain is null-terminated
        CHECK(mip.m_pitch == 8);               // 4 pixels * 16 bits / 8
        CHECK(mip.m_dataSize == 32);
    }

    // ~CImage (0x601804) releases the block and the chain only when the image owns them. The first block goes
    // out of scope at the closing brace, so this exercises the destructor; a sanitizer or a leak checker would
    // see a leak here if the destructor did nothing.
    {
        glitch::video::CImage owned(12, glitch::core::dimension2d<std::int32_t>{8, 8});
        CHECK(owned.m_data != nullptr);
        CHECK(owned.m_dataSize == 256);
        owned.fill(color);  // touch it so the block is not optimised away
        CHECK(word(std::vector<std::uint8_t>(owned.m_data, owned.m_data + 4), 0) == 0x563412FFu);
    }

    std::printf("%s\n", failures ? "image_fill: FAILED" : "image_fill: ok");
    return failures != 0;
}
