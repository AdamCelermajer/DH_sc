#include "floor_types.hpp"

namespace dh2::floor_types {
namespace {

bool equal(Span left, Span right) {
    if (left.size != right.size) return false;
    for (std::size_t i = 0; i < left.size; ++i) {
        if (left.data[i] != right.data[i]) return false;
    }
    return true;
}

bool contains(Span haystack, const char* needle, std::size_t needle_size) {
    if (!haystack.data || needle_size == 0 || haystack.size < needle_size) return false;
    for (std::size_t i = 0; i <= haystack.size - needle_size; ++i) {
        std::size_t j = 0;
        while (j < needle_size && haystack.data[i + j] == needle[j]) ++j;
        if (j == needle_size) return true;
    }
    return false;
}

Span unquote_percent22(Span value) {
    constexpr char quote[] = "%22";
    constexpr std::size_t quote_size = sizeof(quote) - 1U;
    if (!value.data || value.size < quote_size * 2U) return value;

    std::size_t opening = value.size;
    for (std::size_t i = 0; i <= value.size - quote_size; ++i) {
        if (equal({value.data + i, quote_size}, {quote, quote_size})) {
            opening = i;
            break;
        }
    }
    if (opening == value.size) return value;
    const std::size_t content = opening + quote_size;
    for (std::size_t i = content; i <= value.size - quote_size; ++i) {
        if (equal({value.data + i, quote_size}, {quote, quote_size})) {
            return {value.data + content, i - content};
        }
    }
    return value;
}

bool whitespace(char value) {
    return value == ' ' || value == '\t' || value == '\v' ||
           value == '\f' || value == '\r';
}

} // namespace

Error find_property(const char* bytes, std::size_t available_bytes,
                    Span key, Property* out) {
    if (out) {
        out->found = false;
        out->value = {nullptr, 0};
    }
    if (!bytes || !out || !key.data || key.size == 0) return Error::argument;
    if (available_bytes > kMaxUserPropertiesBytes) return Error::too_large;

    std::size_t text_size = 0;
    while (text_size < available_bytes && bytes[text_size] != '\0') ++text_size;
    if (text_size == available_bytes) return Error::unterminated;

    std::size_t line_start = 0;
    while (line_start < text_size) {
        std::size_t line_end = line_start;
        while (line_end < text_size && bytes[line_end] != '\n') ++line_end;

        std::size_t separator = line_start;
        while (separator < line_end && bytes[separator] != '=') ++separator;
        std::size_t key_start = line_start;
        std::size_t key_end = separator;
        while (key_start < key_end && whitespace(bytes[key_start])) ++key_start;
        while (key_end > key_start && whitespace(bytes[key_end - 1U])) --key_end;
        const Span line_key{bytes + key_start, key_end - key_start};
        if (equal(line_key, key)) {
            out->found = true;
            const std::size_t value_start =
                separator < line_end ? separator + 1U : line_end;
            out->value = unquote_percent22(
                {bytes + value_start, line_end - value_start});
        }
        line_start = line_end < text_size ? line_end + 1U : text_size;
    }
    return Error::ok;
}

std::uint32_t floor_type_mask(bool property_present, Span property_value) {
    const Span source = property_present ? property_value : Span{nullptr, 0};
    if (!source.data) return 0;

    std::uint32_t mask = 0;
    if (contains(source, "void", 4U)) mask |= kFloorTypeVoid;
    if (contains(source, "wall", 4U)) mask |= kFloorTypeWall;
    if (contains(source, "hole", 4U)) mask |= kFloorPathHole;
    if (contains(source, "water", 5U)) mask |= kFloorPathWater;
    return mask;
}

bool can_path_on(std::uint32_t floor_mask, std::uint32_t object_path_mask) {
    return floor_mask == 0U || (floor_mask & object_path_mask) == floor_mask;
}

} // namespace dh2::floor_types
