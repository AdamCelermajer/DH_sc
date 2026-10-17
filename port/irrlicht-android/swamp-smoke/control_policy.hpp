#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

namespace dh2::irrlicht_swamp {

// Decode UTF-8 into Unicode scalar values for Irrlicht's wide GUI strings.
// Invalid or truncated sequences become U+FFFD instead of byte-cast glyphs.
inline std::vector<std::uint32_t> utf8_codepoints(const char* text) {
    std::vector<std::uint32_t> output;
    if (!text) return output;
    const auto* bytes = reinterpret_cast<const unsigned char*>(text);
    const std::size_t length = std::strlen(text);
    output.reserve(length);
    for (std::size_t offset = 0; offset < length;) {
        const unsigned char lead = bytes[offset];
        if (lead < 0x80U) {
            output.push_back(lead);
            ++offset;
            continue;
        }

        std::uint32_t codepoint = 0xfffdU;
        std::size_t width = 1;
        if (lead >= 0xc2U && lead <= 0xdfU && offset + 1 < length &&
            (bytes[offset + 1] & 0xc0U) == 0x80U) {
            width = 2;
            codepoint = (std::uint32_t(lead & 0x1fU) << 6U) |
                        std::uint32_t(bytes[offset + 1] & 0x3fU);
        } else if (lead >= 0xe0U && lead <= 0xefU && offset + 2 < length &&
                   (bytes[offset + 1] & 0xc0U) == 0x80U &&
                   (bytes[offset + 2] & 0xc0U) == 0x80U &&
                   !(lead == 0xe0U && bytes[offset + 1] < 0xa0U) &&
                   !(lead == 0xedU && bytes[offset + 1] >= 0xa0U)) {
            width = 3;
            codepoint = (std::uint32_t(lead & 0x0fU) << 12U) |
                        (std::uint32_t(bytes[offset + 1] & 0x3fU) << 6U) |
                        std::uint32_t(bytes[offset + 2] & 0x3fU);
        } else if (lead >= 0xf0U && lead <= 0xf4U && offset + 3 < length &&
                   (bytes[offset + 1] & 0xc0U) == 0x80U &&
                   (bytes[offset + 2] & 0xc0U) == 0x80U &&
                   (bytes[offset + 3] & 0xc0U) == 0x80U &&
                   !(lead == 0xf0U && bytes[offset + 1] < 0x90U) &&
                   !(lead == 0xf4U && bytes[offset + 1] > 0x8fU)) {
            width = 4;
            codepoint = (std::uint32_t(lead & 0x07U) << 18U) |
                        (std::uint32_t(bytes[offset + 1] & 0x3fU) << 12U) |
                        (std::uint32_t(bytes[offset + 2] & 0x3fU) << 6U) |
                        std::uint32_t(bytes[offset + 3] & 0x3fU);
        }
        output.push_back(codepoint);
        offset += width;
    }
    return output;
}

struct Stick {
    float x;
    float y;
};

// Convert screen-pixel drag relative to its joystick center into normalized
// source-world axes. Screen Y is inverted so dragging upward means source +Y.
inline Stick source_stick_from_screen_delta(float dx, float dy,
                                            float radius_pixels) {
    if (!std::isfinite(dx) || !std::isfinite(dy) ||
        !std::isfinite(radius_pixels) || radius_pixels <= 0.0f)
        return {0.0f, 0.0f};
    float x = dx / radius_pixels;
    float y = -dy / radius_pixels;
    const float magnitude_squared = x * x + y * y;
    if (!std::isfinite(magnitude_squared)) return {0.0f, 0.0f};
    if (magnitude_squared > 1.0f) {
        const float inverse_length = 1.0f / std::sqrt(magnitude_squared);
        x *= inverse_length;
        y *= inverse_length;
    }
    return {x, y};
}

// The diagnostic selects source Walk only while a checked movement request is
// accepted. Release, zero input, or a blocked floor endpoint selects Idle.
inline bool prince_walk_requested(float x, float y, bool floor_step_accepted) {
    return floor_step_accepted && std::isfinite(x) && std::isfinite(y) &&
           (std::fabs(x) > 0.01f || std::fabs(y) > 0.01f);
}

// Bounded 20 ms game-loop pacing. A slow frame contributes at most 100 ms;
// at most five source movement steps run per rendered frame.
class FixedStepAccumulator {
public:
    static constexpr std::uint64_t step_nanoseconds = 20000000U;
    static constexpr std::uint64_t max_frame_nanoseconds = 100000000U;
    static constexpr std::uint32_t max_steps_per_frame = 5U;

    std::uint32_t advance(std::uint64_t elapsed_nanoseconds) {
        elapsed_nanoseconds = std::min(elapsed_nanoseconds,
                                       max_frame_nanoseconds);
        accumulator_nanoseconds_ += elapsed_nanoseconds;
        const std::uint64_t due = accumulator_nanoseconds_ / step_nanoseconds;
        const auto steps = static_cast<std::uint32_t>(
            std::min<std::uint64_t>(due, max_steps_per_frame));
        accumulator_nanoseconds_ -= std::uint64_t(steps) * step_nanoseconds;
        // Never carry a large backlog into a later render frame.
        if (due > max_steps_per_frame) accumulator_nanoseconds_ = 0;
        return steps;
    }

    void reset() { accumulator_nanoseconds_ = 0; }
    std::uint64_t remainder_nanoseconds() const {
        return accumulator_nanoseconds_;
    }

private:
    std::uint64_t accumulator_nanoseconds_ = 0;
};

} // namespace dh2::irrlicht_swamp
