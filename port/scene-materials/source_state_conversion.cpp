#include "source_state_conversion.hpp"

namespace dh2::scene_materials::source_state {
namespace {

std::uint32_t read_le32(const std::uint8_t* bytes) noexcept {
    return std::uint32_t(bytes[0]) |
           (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) |
           (std::uint32_t(bytes[3]) << 24);
}

void write_le32(std::uint8_t* bytes, std::uint32_t value) noexcept {
    bytes[0] = static_cast<std::uint8_t>(value);
    bytes[1] = static_cast<std::uint8_t>(value >> 8);
    bytes[2] = static_cast<std::uint8_t>(value >> 16);
    bytes[3] = static_cast<std::uint8_t>(value >> 24);
}

} // namespace

Error convert_video_state(const std::uint8_t* source, std::size_t source_size,
                          std::uint8_t* output, std::size_t output_size) noexcept {
    if (!source || !output) return Error::argument;
    if (source_size != kVideoStateBytes) return Error::input_size;
    if (output_size != kRenderPassStateBytes) return Error::output_size;

    const auto packed = read_le32(source + 0x08);
    const auto flags = read_le32(source + 0x0c);
    const auto output_packed =
        std::uint32_t(source[0]) |
        (std::uint32_t(source[2]) << 8) |
        (std::uint32_t(source[3]) << 16) |
        (((packed >> 12) & 0x7u) << 24) |
        (((flags >> 12) & 0x7u) << 27) |
        (packed & 0xc0000000u);

    std::uint32_t output_flags = 0;
    // Source RenderState flags at +0x0c are compacted into renderpass flags.
    // This bit-for-bit mapping follows the conditional set/clear instructions
    // in the original copy constructor, including the lone bit from +0x10.
    constexpr unsigned source_flag_bits[] = {
        19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30,
    };
    for (unsigned i = 0; i < sizeof(source_flag_bits) / sizeof(source_flag_bits[0]); ++i) {
        if (flags & (1u << source_flag_bits[i]))
            output_flags |= 1u << (16u + i);
    }
    if (read_le32(source + 0x10) & 1u) output_flags |= 1u << 27;
    output_flags |= ((packed >> 18) & 0x7u);
    output_flags |= ((packed >> 18) & 0x38u); // bits 21..23 -> output bits 3..5
    output_flags |= ((packed >> 18) & 0x1c0u); // bits 24..26 -> output bits 6..8
    output_flags |= ((packed >> 18) & 0xe00u); // bits 27..29 -> output bits 9..11
    output_flags |= ((flags >> 3) & 0x3000u); // source bits 15..16 -> output 12..13
    output_flags |= ((flags >> 3) & 0xc000u); // source bits 17..18 -> output 14..15

    // The ARM routine copies four flag bytes and five scalar words unchanged.
    for (std::size_t i = 0; i < 4; ++i) output[8 + i] = source[0x14 + i];
    write_le32(output + 0, output_packed);
    write_le32(output + 4, output_flags);
    write_le32(output + 12, read_le32(source + 0x28));
    write_le32(output + 16, read_le32(source + 0x2c));
    write_le32(output + 20, read_le32(source + 0x30));
    write_le32(output + 24, read_le32(source + 0x34));
    write_le32(output + 28, read_le32(source + 0x38));
    return Error::ok;
}

const char* error_name(Error error) noexcept {
    switch (error) {
    case Error::ok: return "ok";
    case Error::argument: return "argument";
    case Error::input_size: return "input_size";
    case Error::output_size: return "output_size";
    }
    return "unknown";
}

} // namespace dh2::scene_materials::source_state
