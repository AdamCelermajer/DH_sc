#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

extern "C" {
#include "../pydata-constants/constants.h"
}

namespace dh2::ui {

inline bool font_palette_text_color_at_index_v1(const std::uint8_t* font_array,
                                                std::size_t font_array_size,
                                                std::uint32_t index,
                                                std::uint32_t& color,
                                                std::string& error) {
    if (!font_array || font_array_size < 4) {
        error = "Invalid FontPalette projection input";
        return false;
    }
    auto read_u32 = [](const std::uint8_t* p) {
        return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
               (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
    };
    const std::uint32_t count = read_u32(font_array);
    if (count != 7 || font_array_size != 4u + std::size_t(count) * 8u || index >= count) {
        error = "FontPalette serialized shape/index is outside the recovered seven-entry layout";
        return false;
    }
    color = read_u32(font_array + 4u + std::size_t(index) * 8u + 4u);
    error.clear();
    return true;
}

// Source-compatible ItemInstance::GetColor projection. `font_array` is the
// serialized FontPalette array (count + glow/text uint32 pairs); `power_colors`
// is the original ItemPowerColor constant group. All inputs are borrowed.
inline bool font_palette_text_color_v1(const std::uint8_t* font_array,
                                       std::size_t font_array_size,
                                       const dh2_pycst_view& power_colors,
                                       std::int32_t power_count,
                                       std::uint32_t& color,
                                       std::string& error) {
    if (!font_array || font_array_size < 4 || power_count < 0) {
        error = "Invalid FontPalette projection input";
        return false;
    }

    const char* key = nullptr;
    switch (power_count) {
    case 0: key = "zero"; break;
    case 1: key = "one"; break;
    case 2: key = "two"; break;
    case 3: key = "three"; break;
    case 4: key = "four"; break;
    default: break;
    }
    std::uint32_t index = 2; // Original GetFontDef fallback for five or more.
    if (key) {
        dh2_pycst_result result{};
        if (dh2_pycst_get(&power_colors, "ItemPowerColor", 14, key,
                          static_cast<std::uint32_t>(std::char_traits<char>::length(key)),
                          &result) != 0 || !result.found || result.value < 0 ||
            result.value >= 7) {
            error = std::string("Missing/invalid original ItemPowerColor.") + key;
            return false;
        }
        index = static_cast<std::uint32_t>(result.value);
    }
    return font_palette_text_color_at_index_v1(font_array, font_array_size, index, color, error);
}

// Exact `%06X` width/minimum-width behavior used by the inventory callbacks.
inline bool format_item_name_v1(const std::string& name,
                                std::uint32_t color,
                                std::string& output) {
    static constexpr char hex[] = "0123456789ABCDEF";
    char digits[8];
    unsigned length = 0;
    do { digits[length++] = hex[color & 15u]; color >>= 4; } while (color);
    while (length < 6) digits[length++] = '0';
    std::string candidate = "<font color='#";
    while (length) candidate.push_back(digits[--length]);
    candidate += "'>";
    candidate += name;
    candidate += "</font>";
    output = std::move(candidate);
    return true;
}

} // namespace dh2::ui
