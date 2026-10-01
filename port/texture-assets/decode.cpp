#include "texture.hpp"

#include <algorithm>
#include <array>
#include <cstdint>

namespace dh2::textures {
namespace {

// PVRTC1 word layout and reconstruction follow the Khronos Data Format
// Specification, "PVRTC Compressed Texture Image Formats". The cache uses
// the legacy PVR v2 selectors 24 (2bpp) and 25 (4bpp).
struct Word {
    std::uint32_t modulation;
    std::uint32_t color;
};
using Color = std::array<int, 4>; // RGB: 5 bits each, A: 4 bits

std::uint32_t le32(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8)
        | (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}

bool power_of_two(std::uint32_t v) { return v && !(v & (v - 1)); }

std::size_t word_index(std::uint32_t x, std::uint32_t y,
                       std::uint32_t words_x, std::uint32_t words_y) {
    // Reflected Morton order: Y contributes the lower bit in each pair.
    const auto smaller = std::min(words_x, words_y);
    std::size_t index = 0;
    unsigned shift = 0;
    for (std::uint32_t bit = 1; bit < smaller; bit <<= 1, ++shift)
        index |= (std::size_t((y & bit) | ((x & bit) << 1)) << shift);
    index |= (std::size_t((x | y) >> shift) << (2 * shift));
    return index;
}

Word word_at(const TextureView& view, std::uint32_t x, std::uint32_t y,
             std::uint32_t words_x, std::uint32_t words_y) {
    const auto* p = view.payload + 8 * word_index(x, y, words_x, words_y);
    return {le32(p), le32(p + 4)};
}

int extend_3_to_5(int v) { return (v << 2) | (v >> 1); }
int extend_4_to_5(int v) { return (v << 1) | (v >> 3); }

Color endpoint(std::uint32_t word_color, bool second) {
    const auto bits = second ? (word_color >> 16) : (word_color & 0xffffU);
    if (bits & 0x8000U) {
        // The first endpoint spends one low bit on the modulation mode.
        const int red = (bits >> 10) & 31;
        const int green = (bits >> 5) & 31;
        const int blue = second ? int(bits & 31) : extend_4_to_5((bits >> 1) & 15);
        return {red, green, blue, 15};
    }
    const int alpha = int((bits >> 12) & 7) << 1;
    const int red = extend_4_to_5((bits >> 8) & 15);
    const int green = extend_4_to_5((bits >> 4) & 15);
    const int blue = second ? extend_4_to_5(bits & 15)
                            : extend_3_to_5((bits >> 1) & 7);
    return {red, green, blue, alpha};
}

int interpolated_channel(const Color& p, const Color& q, const Color& r,
                         const Color& s, int channel, int dx, int dy,
                         int block_width, bool is_2bpp) {
    const int y0 = 4 - dy;
    const int x0 = block_width - dx;
    const int c = p[channel] * x0 * y0 + q[channel] * dx * y0
        + r[channel] * x0 * dy + s[channel] * dx * dy;
    if (channel == 3)
        return is_2bpp ? (c >> 1) + (c >> 5) : c + (c >> 4);
    return is_2bpp ? (c >> 2) + (c >> 7) : (c >> 1) + (c >> 6);
}

int stored_2bpp_weight(const Word& word, int local_x, int local_y) {
    const int pixel = local_y * 8 + local_x;
    const auto data = word.modulation;
    if (!(word.color & 1U)) return (data & (1U << pixel)) ? 8 : 0;
    // Stored samples occupy a checkerboard. The (0,0) sample and, in
    // horizontal/vertical interpolation mode, (4,2) carry one data bit.
    const int sample = local_y * 4 + local_x / 2;
    int code;
    if (sample == 0) code = (data & 2U) ? 3 : 0;
    else if (sample == 10 && (data & 1U)) code = (data & (1U << 21)) ? 3 : 0;
    else code = int((data >> (sample * 2)) & 3U);
    static constexpr int weights[4] = {0, 3, 5, 8};
    return weights[code];
}

int adjacent_weight(const TextureView& view, int x, int y,
                    std::uint32_t words_x, std::uint32_t words_y,
                    int width, int height) {
    x = (x + width) % width;
    y = (y + height) % height;
    const Word word = word_at(view, std::uint32_t(x / 8), std::uint32_t(y / 4),
                              words_x, words_y);
    return stored_2bpp_weight(word, x % 8, y % 4);
}

struct Modulation { int weight; bool punch; };

Modulation modulation(const TextureView& view, const Word& word,
                      int x, int y, std::uint32_t words_x,
                      std::uint32_t words_y, bool is_2bpp) {
    if (!is_2bpp) {
        const int code = int((word.modulation >> (2 * ((y % 4) * 4 + (x % 4)))) & 3U);
        if (word.color & 1U) {
            static constexpr int weights[4] = {0, 4, 4, 8};
            return {weights[code], code == 2};
        }
        static constexpr int weights[4] = {0, 3, 5, 8};
        return {weights[code], false};
    }
    const int local_x = x % 8, local_y = y % 4;
    if (!(word.color & 1U) || !((local_x ^ local_y) & 1))
        return {stored_2bpp_weight(word, local_x, local_y), false};
    const int left = adjacent_weight(view, x - 1, y, words_x, words_y,
                                     int(view.width), int(view.height));
    const int right = adjacent_weight(view, x + 1, y, words_x, words_y,
                                      int(view.width), int(view.height));
    if (word.modulation & 1U) {
        if (!(word.modulation & (1U << 20))) return {(left + right + 1) / 2, false};
        const int up = adjacent_weight(view, x, y - 1, words_x, words_y,
                                       int(view.width), int(view.height));
        const int down = adjacent_weight(view, x, y + 1, words_x, words_y,
                                         int(view.width), int(view.height));
        return {(up + down + 1) / 2, false};
    }
    const int up = adjacent_weight(view, x, y - 1, words_x, words_y,
                                   int(view.width), int(view.height));
    const int down = adjacent_weight(view, x, y + 1, words_x, words_y,
                                     int(view.width), int(view.height));
    return {(left + right + up + down + 2) / 4, false};
}

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a <= b ? b - a < left_size : a - b < right_size;
}

} // namespace

Error decode_rgba8(void* output, std::size_t output_size,
                   std::size_t row_stride, const void* data, std::size_t size) {
    TextureView view{};
    const auto status = open(&view, data, size);
    if (status != Error::ok) return status;
    if (view.format != Format::pvrtc_2bpp && view.format != Format::pvrtc_4bpp)
        return Error::unsupported_format;
    const bool is_2bpp = view.format == Format::pvrtc_2bpp;
    const std::uint32_t block_width = is_2bpp ? 8U : 4U;
    if (!power_of_two(view.width) || !power_of_two(view.height)
        || view.width < 2 * block_width || view.height < 8)
        return Error::unsupported_format;
    const auto words_x = view.width / block_width, words_y = view.height / 4;
    if (std::uint64_t(words_x) * words_y * 8 != view.payload_size)
        return Error::invalid_payload;
    const auto line_bytes = std::size_t(view.width) * 4;
    if (!output || row_stride < line_bytes
        || row_stride > (SIZE_MAX - line_bytes) / (view.height - 1)
        || output_size < row_stride * (view.height - 1) + line_bytes
        || overlaps(output, output_size, data, size))
        return Error::invalid_output;

    auto* out = static_cast<std::uint8_t*>(output);
    for (std::uint32_t y = 0; y < view.height; ++y) {
        for (std::uint32_t x = 0; x < view.width; ++x) {
            const auto word = word_at(view, x / block_width, y / 4, words_x, words_y);
            const auto sample_x = (x + view.width - block_width / 2) % view.width;
            const auto sample_y = (y + view.height - 2) % view.height;
            const auto sx = sample_x / block_width, sy = sample_y / 4;
            const auto next_x = (sx + 1) % words_x, next_y = (sy + 1) % words_y;
            const auto p = word_at(view, sx, sy, words_x, words_y);
            const auto q = word_at(view, next_x, sy, words_x, words_y);
            const auto r = word_at(view, sx, next_y, words_x, words_y);
            const auto s = word_at(view, next_x, next_y, words_x, words_y);
            const auto pa = endpoint(p.color, false), qa = endpoint(q.color, false);
            const auto ra = endpoint(r.color, false), sa = endpoint(s.color, false);
            const auto pb = endpoint(p.color, true), qb = endpoint(q.color, true);
            const auto rb = endpoint(r.color, true), sb = endpoint(s.color, true);
            const auto mod = modulation(view, word, int(x), int(y), words_x,
                                        words_y, is_2bpp);
            const int dx = int(sample_x % block_width), dy = int(sample_y % 4);
            auto* pixel = out + std::size_t(y) * row_stride + std::size_t(x) * 4;
            for (int channel = 0; channel < 4; ++channel) {
                const int a = interpolated_channel(pa, qa, ra, sa, channel,
                                                    dx, dy, int(block_width), is_2bpp);
                const int b = interpolated_channel(pb, qb, rb, sb, channel,
                                                    dx, dy, int(block_width), is_2bpp);
                pixel[channel] = std::uint8_t((a * (8 - mod.weight) + b * mod.weight) / 8);
            }
            if (mod.punch) pixel[3] = 0;
        }
    }
    return Error::ok;
}

} // namespace dh2::textures

extern "C" dh2::textures::Error dh2_texture_decode_rgba8(
    void* output, std::size_t output_size, std::size_t row_stride,
    const void* data, std::size_t size) {
    return dh2::textures::decode_rgba8(output, output_size, row_stride, data, size);
}
