#include "texture_loader.hpp"
#include "../engine-textures/textures.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <new>
#include <utility>

namespace dh::foundation {
namespace {
constexpr std::size_t max_file_bytes = 128u * 1024u * 1024u;

std::uint32_t le32(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
           (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}

// The reusable decoder deliberately rejects all mipmapped PVR containers.
// A validated 2D container can safely supply only its first (largest) mip.
bool open_base_mip(const void* bytes, std::size_t size,
                   dh2::textures::View& view) {
    using namespace dh2::textures;
    Description desc{};
    if (!dh2_pvr_describe(bytes, size, &desc) || desc.kind != 0 || !desc.mipmapped)
        return false;
    const auto* data = static_cast<const std::uint8_t*>(bytes);
    const std::size_t offset = size >= 8 && !std::memcmp(data, "BTEXpvr\0", 8) ? 8 : 0;
    const auto* header = data + offset;
    const auto flags = le32(header + 16);
    const auto type = flags & 255;
    if ((type != 24 && type != 25) || desc.width > 4096 || desc.height > 4096 ||
        (desc.width & (desc.width - 1)) || (desc.height & (desc.height - 1)))
        return false;
    const bool two_bpp = type == 24;
    if (le32(header + 24) != (two_bpp ? 2u : 4u)) return false;
    const auto base_size = std::size_t(std::max(desc.width, two_bpp ? 16u : 8u)) *
                           std::max(desc.height, 8u) * (two_bpp ? 2u : 4u) / 8;
    if (base_size > size - offset - 52) return false;
    view = {data + offset + 52, base_size, desc.width, desc.height,
            two_bpp ? Format::pvrtc2 : Format::pvrtc4,
            (flags & 0x8000) != 0, 1, 0};
    return true;
}
} // namespace

bool decode_texture(const void* bytes, std::size_t size,
                    TextureImage& image, std::string& error) {
    if (!bytes || !size || size > max_file_bytes) {
        error = "Texture input is empty or exceeds the 128 MiB input limit";
        return false;
    }
    dh2::textures::View view{};
    auto status = dh2_texture_open(bytes, size, &view);
    if (status == dh2::textures::Error::unsupported && open_base_mip(bytes, size, view))
        status = dh2::textures::Error::ok;
    if (status != dh2::textures::Error::ok) {
        error = std::string("Cannot open texture: ") + dh2_texture_error(status);
        return false;
    }
    try {
        TextureImage decoded;
        decoded.width = view.width;
        decoded.height = view.height;
        decoded.rgba.resize(std::size_t(view.width) * view.height * 4);
        status = dh2_texture_decode(&view, decoded.rgba.data(), decoded.rgba.size());
        if (status != dh2::textures::Error::ok) {
            error = std::string("Cannot decode texture: ") + dh2_texture_error(status);
            return false;
        }
        image = std::move(decoded);
        error.clear();
        return true;
    } catch (const std::bad_alloc&) {
        error = "Cannot allocate decoded texture pixels";
        return false;
    }
}

bool load_texture(const std::filesystem::path& path,
                  TextureImage& image, std::string& error) {
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    if (!stream) {
        error = "Cannot open texture file: " + path.u8string();
        return false;
    }
    const auto end = stream.tellg();
    if (end <= 0 || end > static_cast<std::streamoff>(max_file_bytes)) {
        error = "Texture file is empty, unreadable, or exceeds 128 MiB: " + path.u8string();
        return false;
    }
    try {
        std::vector<std::uint8_t> bytes(static_cast<std::size_t>(end));
        stream.seekg(0, std::ios::beg);
        if (!stream.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()))) {
            error = "Cannot read complete texture file: " + path.u8string();
            return false;
        }
        if (!decode_texture(bytes.data(), bytes.size(), image, error)) {
            error += ": " + path.u8string();
            return false;
        }
        return true;
    } catch (const std::bad_alloc&) {
        error = "Cannot allocate texture file buffer: " + path.u8string();
        return false;
    }
}

} // namespace dh::foundation
