#pragma once
#include <cstddef>
#include <cstdint>

namespace dh2::textures {
enum class Error : std::uint32_t { ok, argument, header, dimensions, format, truncated, capacity, unsupported };
enum class Format : std::uint32_t { pvrtc2, pvrtc4, tga_bgr24, tga_bgra32 };
// The first eight fields mirror the observed STextureDesc word offsets.
// They are values, never original ARM32 pointers or a C++ object layout.
struct Description {
    std::uint32_t kind, engine_format, base_mip, reserved;
    std::uint32_t width, height, surfaces, mipmapped;
};
struct View {
    const std::uint8_t* payload;
    std::size_t payload_size;
    std::uint32_t width, height;
    Format format;
    std::uint32_t alpha, top_origin, right_origin;
};
}
extern "C" {
// Reconstructed PVR description semantics on validated PVR v2 / BTEX input.
// Additional input bounds checks intentionally reject unsafe original cases.
bool dh2_pvr_describe(const void*, std::size_t, dh2::textures::Description*);
bool dh2_pvrtc_decompress(const void*, std::size_t, bool, unsigned, unsigned, std::uint8_t*, std::size_t);
dh2::textures::Error dh2_texture_open(const void*, std::size_t, dh2::textures::View*);
dh2::textures::Error dh2_texture_decode(const dh2::textures::View*, std::uint8_t*, std::size_t);
const char* dh2_texture_error(dh2::textures::Error);
}
