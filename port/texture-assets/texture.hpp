#pragma once

#include <cstddef>
#include <cstdint>

// Immutable views of texture files found in the supplied Dungeon Hunter 2 cache.
// This is a new, checked ABI. It is not the original engine's STextureDesc ABI.
namespace dh2::textures {

enum class Error : std::uint32_t {
    ok = 0,
    null_input,
    short_input,
    unrecognized,
    invalid_header,
    invalid_extent,
    invalid_payload,
    trailing_bytes,
    unsupported_format,
};

enum class Kind : std::uint32_t {
    unknown = 0,
    btex_pvr_v2,
    pvr_v2,
    tga,
    png,
};

enum class Format : std::uint32_t {
    unknown = 0,
    pvrtc_2bpp,
    pvrtc_4bpp,
    bgra8,
    encoded_png,
};

struct TextureView {
    const std::uint8_t* bytes;
    std::size_t size;
    const std::uint8_t* payload;
    std::size_t payload_size;
    std::size_t payload_offset;
    std::uint32_t width, height;
    std::uint32_t flags, bits_per_pixel, mipmaps, surfaces, alpha_mask;
    std::uint32_t tga_descriptor;
    Kind kind;
    Format format;
};

// On success, all pointers borrow `data` and remain valid only while it does.
// No input bytes are changed and no heap allocation occurs. PNG is structurally
// checked, but its pixels are intentionally left encoded for a separate decoder.
// On unsupported_format, the recognized container metadata is still returned.
Error open(TextureView* out, const void* data, std::size_t size);

} // namespace dh2::textures

extern "C" dh2::textures::Error dh2_texture_open(
    dh2::textures::TextureView* out, const void* data, std::size_t size);
