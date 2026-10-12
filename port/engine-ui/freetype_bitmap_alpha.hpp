#pragma once
#include <cstddef>
#include <cstdint>
namespace dh2::ui {
struct FreetypeBitmap40 {
    const std::uint8_t* bytes;
    std::size_t size;
    std::int32_t width,rows,pitch;
    std::uint32_t pixel_mode,num_grays,row0_offset;
};
struct FreetypeAlpha32 {
    std::uint8_t* bytes;
    std::size_t capacity;
    std::uint32_t width,height,pitch,reserved;
};
static_assert(sizeof(FreetypeBitmap40)==40);
static_assert(sizeof(FreetypeAlpha32)==32);
}
// Safe native FT pixel decoding, not original packed-row byte-copy parity.
// pixel modes:1 MONO,2 GRAY,3 GRAY2,4 GRAY4. Output0..255 alpha, padded.
extern "C" int dh2_freetype_bitmap_alpha(dh2::ui::FreetypeAlpha32*,
    const dh2::ui::FreetypeBitmap40*) noexcept;
