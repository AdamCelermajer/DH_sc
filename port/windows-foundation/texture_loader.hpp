#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace dh::foundation {

// Tightly packed RGBA8, row zero at the top of the image. Only base mip is
// retained; the renderer can generate its own mip chain after uploading.
struct TextureImage {
    std::uint32_t width{};
    std::uint32_t height{};
    std::vector<std::uint8_t> rgba;
};

// Content detection, independent of extension: original BTEX/PVR v2 PVRTC2/4
// and conventional uncompressed 24/32-bit true-colour TGA. Cubemaps, volume
// textures, PVR v3, other compression formats, and RLE TGA reject explicitly.
// Both functions leave image unchanged on failure and clear error on success.
bool decode_texture(const void* bytes, std::size_t size,
                    TextureImage& image, std::string& error);
bool load_texture(const std::filesystem::path& path,
                  TextureImage& image, std::string& error);

} // namespace dh::foundation
