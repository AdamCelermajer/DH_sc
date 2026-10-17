#include "texture.hpp"

#include <algorithm>
#include <cstring>

namespace dh2::textures {
namespace {

constexpr std::uint32_t max_dimension = 16384;

std::uint16_t le16(const std::uint8_t* p) {
    return std::uint16_t(p[0]) | (std::uint16_t(p[1]) << 8);
}
std::uint32_t le32(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8)
        | (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}
std::uint32_t be32(const std::uint8_t* p) {
    return (std::uint32_t(p[0]) << 24) | (std::uint32_t(p[1]) << 16)
        | (std::uint32_t(p[2]) << 8) | std::uint32_t(p[3]);
}
bool dimensions(std::uint32_t width, std::uint32_t height) {
    return width > 0 && height > 0 && width <= max_dimension && height <= max_dimension;
}

Error pvr(TextureView* out, const std::uint8_t* bytes, std::size_t size,
          std::size_t base, Kind kind) {
    if (size - base < 52) return Error::short_input;
    const auto* h = bytes + base;
    if (le32(h) != 52 || std::memcmp(h + 44, "PVR!", 4) != 0)
        return Error::invalid_header;
    const auto height = le32(h + 4), width = le32(h + 8);
    if (!dimensions(width, height)) return Error::invalid_extent;
    constexpr std::size_t header_size = 52;
    const std::size_t data_offset = base + header_size;
    const auto data_length = le32(h + 20);
    if (data_length > size - data_offset) return Error::invalid_payload;
    if (data_length < size - data_offset) return Error::trailing_bytes;

    TextureView view{};
    view.bytes = bytes;
    view.size = size;
    view.payload = bytes + data_offset;
    view.payload_size = data_length;
    view.payload_offset = data_offset;
    view.width = width;
    view.height = height;
    view.flags = le32(h + 16);
    view.bits_per_pixel = le32(h + 24);
    view.mipmaps = le32(h + 12);
    view.surfaces = le32(h + 48);
    view.alpha_mask = le32(h + 40);
    view.kind = kind;
    *out = view;

    // The low byte is the legacy PVR pixel-type selector. Preserve all other
    // flag bits verbatim: their engine-specific interpretation is not yet known.
    const auto type = view.flags & 0xffU;
    if (type != 24 && type != 25) return Error::unsupported_format;
    if (view.mipmaps != 0 || view.surfaces != 1) return Error::unsupported_format;
    const auto expected_bpp = type == 24 ? 2U : 4U;
    if (view.bits_per_pixel != expected_bpp) return Error::invalid_header;
    const std::uint64_t expected = type == 24
        ? std::uint64_t(std::max(width, 16U)) * std::max(height, 8U) / 4U
        : std::uint64_t(std::max(width, 8U)) * std::max(height, 8U) / 2U;
    if (expected != data_length) return Error::invalid_payload;
    out->format = type == 24 ? Format::pvrtc_2bpp : Format::pvrtc_4bpp;
    return Error::ok;
}

Error tga(TextureView* out, const std::uint8_t* bytes, std::size_t size) {
    if (size < 18) return Error::short_input;
    const auto width = le16(bytes + 12), height = le16(bytes + 14);
    if (!dimensions(width, height)) return Error::invalid_extent;
    TextureView view{};
    view.bytes = bytes;
    view.size = size;
    view.width = width;
    view.height = height;
    view.bits_per_pixel = bytes[16];
    view.tga_descriptor = bytes[17];
    view.kind = Kind::tga;
    *out = view;
    if (bytes[1] != 0 || bytes[2] != 2 || le16(bytes + 3) != 0
        || le16(bytes + 5) != 0 || bytes[7] != 0 || bytes[16] != 32)
        return Error::unsupported_format;
    const std::size_t offset = 18 + bytes[0];
    const std::uint64_t pixel_bytes = std::uint64_t(width) * height * 4U;
    if (offset > size || pixel_bytes > size - offset) return Error::invalid_payload;
    const std::size_t end = offset + static_cast<std::size_t>(pixel_bytes);
    const std::size_t extra = size - end;
    if (extra != 0) {
        static constexpr char signature[] = "TRUEVISION-XFILE.\0";
        if (extra != 26 || std::memcmp(bytes + end + 8, signature, 18) != 0)
            return Error::trailing_bytes;
    }
    out->payload = bytes + offset;
    out->payload_size = static_cast<std::size_t>(pixel_bytes);
    out->payload_offset = offset;
    out->format = Format::bgra8;
    return Error::ok;
}

std::uint32_t png_crc(const std::uint8_t* bytes, std::size_t size) {
    std::uint32_t crc = 0xffffffffU;
    for (std::size_t i = 0; i < size; ++i) {
        crc ^= bytes[i];
        for (int bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ ((crc & 1U) ? 0xedb88320U : 0U);
    }
    return crc ^ 0xffffffffU;
}

Error png(TextureView* out, const std::uint8_t* bytes, std::size_t size) {
    if (size < 8 + 12 + 13 + 12) return Error::short_input;
    std::size_t offset = 8;
    bool first = true, idat = false, end = false;
    std::uint32_t width = 0, height = 0;
    while (offset < size) {
        if (size - offset < 12) return Error::short_input;
        const auto length = be32(bytes + offset);
        if (length > size - offset - 12) return Error::invalid_payload;
        const auto* type = bytes + offset + 4;
        const auto* payload = type + 4;
        if (png_crc(type, std::size_t(length) + 4) != be32(payload + length))
            return Error::invalid_payload;
        if (first) {
            if (std::memcmp(type, "IHDR", 4) != 0 || length != 13)
                return Error::invalid_header;
            width = be32(payload);
            height = be32(payload + 4);
            if (!dimensions(width, height)) return Error::invalid_extent;
            const auto depth = payload[8], color = payload[9];
            const bool valid_color_depth = (color == 0 && (depth == 1 || depth == 2 || depth == 4 || depth == 8 || depth == 16))
                || (color == 2 && (depth == 8 || depth == 16))
                || (color == 3 && (depth == 1 || depth == 2 || depth == 4 || depth == 8))
                || (color == 4 && (depth == 8 || depth == 16))
                || (color == 6 && (depth == 8 || depth == 16));
            if (!valid_color_depth || payload[10] != 0 || payload[11] != 0 || payload[12] > 1)
                return Error::invalid_header;
            first = false;
        } else if (std::memcmp(type, "IHDR", 4) == 0) {
            return Error::invalid_header;
        }
        if (std::memcmp(type, "IDAT", 4) == 0) idat = true;
        if (std::memcmp(type, "IEND", 4) == 0) {
            if (length != 0 || !idat) return Error::invalid_header;
            end = true;
            offset += 12;
            break;
        }
        offset += std::size_t(length) + 12;
    }
    if (!end) return Error::invalid_header;
    if (offset != size) return Error::trailing_bytes;
    out->bytes = bytes;
    out->size = size;
    out->payload = bytes;
    out->payload_size = size;
    out->payload_offset = 0;
    out->width = width;
    out->height = height;
    out->kind = Kind::png;
    out->format = Format::encoded_png;
    return Error::ok;
}

} // namespace

Error open(TextureView* out, const void* data, std::size_t size) {
    if (!out) return Error::null_input;
    *out = {};
    if (!data) return Error::null_input;
    const auto* bytes = static_cast<const std::uint8_t*>(data);
    if (size >= 8 && std::memcmp(bytes, "BTEXpvr\0", 8) == 0)
        return pvr(out, bytes, size, 8, Kind::btex_pvr_v2);
    if (size >= 52 && le32(bytes) == 52 && std::memcmp(bytes + 44, "PVR!", 4) == 0)
        return pvr(out, bytes, size, 0, Kind::pvr_v2);
    static constexpr std::uint8_t png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (size >= 8 && std::memcmp(bytes, png_signature, 8) == 0)
        return png(out, bytes, size);
    if (size >= 18 && bytes[1] <= 1 && (bytes[2] == 1 || bytes[2] == 2
        || bytes[2] == 3 || bytes[2] == 9 || bytes[2] == 10 || bytes[2] == 11))
        return tga(out, bytes, size);
    return size < 8 ? Error::short_input : Error::unrecognized;
}

} // namespace dh2::textures

extern "C" dh2::textures::Error dh2_texture_open(
    dh2::textures::TextureView* out, const void* data, std::size_t size) {
    return dh2::textures::open(out, data, size);
}
