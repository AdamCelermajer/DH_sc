#include "technique_selector.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <limits>

namespace dh2::scene_materials {
namespace {
constexpr std::string_view current_suffix = "/CurrentTechnique";
constexpr std::size_t max_selector_bytes = 127;

bool borrowed(const resources::BresView& image,
              const std::uint8_t* pointer, std::size_t length) {
    if (!image.bytes || !pointer) return false;
    const auto base = reinterpret_cast<std::uintptr_t>(image.bytes);
    const auto address = reinterpret_cast<std::uintptr_t>(pointer);
    return address >= base && address - base <= image.size &&
           length <= image.size - (address - base);
}

bool span(const resources::BresView& image, std::uint32_t offset,
          std::size_t length) {
    return image.bytes && offset <= image.size &&
           length <= image.size - offset;
}

std::uint32_t word(const std::uint8_t* bytes) {
    return std::uint32_t(bytes[0]) |
           (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) |
           (std::uint32_t(bytes[3]) << 24);
}

bool source_string(const resources::BresView& image, std::uint32_t offset,
                   std::size_t max_bytes, std::string& output) {
    output.clear();
    if (!offset || !span(image, offset, 1)) return false;
    const auto remaining = image.size - offset;
    const auto bound = std::min(remaining, max_bytes + 1);
    const auto* begin = image.bytes + offset;
    const auto* end = static_cast<const std::uint8_t*>(
        std::memchr(begin, 0, bound));
    if (!end || end == begin ||
        static_cast<std::size_t>(end - begin) > max_bytes) return false;
    output.assign(reinterpret_cast<const char*>(begin),
                  static_cast<std::size_t>(end - begin));
    return true;
}

bool valid_ascii_name(std::string_view text) {
    return !text.empty() && std::all_of(text.begin(), text.end(), [](char c) {
        const auto u = static_cast<unsigned char>(c);
        return u >= 0x20 && u <= 0x7e;
    });
}

void fail(std::string& error, const char* message) { error = message; }
}

TechniqueSelectorError decode_current_technique(
    const materials::Parameter& parameter, CurrentTechnique& output,
    std::string& error) {
    output = {};
    error.clear();
    if (!parameter.image.bytes || !parameter.id) {
        fail(error, "missing material parameter or BRES view");
        return TechniqueSelectorError::argument;
    }
    const std::string_view id(parameter.id);
    if (id.size() <= current_suffix.size() ||
        id.substr(id.size() - current_suffix.size()) != current_suffix) {
        fail(error, "material parameter ID is not a CurrentTechnique field");
        return TechniqueSelectorError::kind;
    }
    if (parameter.type_code != 20) {
        fail(error, "CurrentTechnique parameter is not type 20");
        return TechniqueSelectorError::kind;
    }
    if (parameter.value_count != 1) {
        fail(error, "CurrentTechnique value count is not one");
        return TechniqueSelectorError::count;
    }
    if (!borrowed(parameter.image, parameter.raw_value, 8)) {
        fail(error, "CurrentTechnique payload is truncated or outside BRES");
        return TechniqueSelectorError::range;
    }

    const auto suffix_start = id.size() - current_suffix.size();
    const auto profile_marker = id.rfind("-profile_", suffix_start);
    if (profile_marker == std::string_view::npos ||
        profile_marker + 9 >= suffix_start) {
        fail(error, "CurrentTechnique ID has no profile component");
        return TechniqueSelectorError::profile;
    }
    const auto profile = id.substr(profile_marker + 9,
                                   suffix_start - profile_marker - 9);
    if (profile.size() > 32 || !std::all_of(profile.begin(), profile.end(),
            [](char c) {
                const auto u = static_cast<unsigned char>(c);
                return std::isalnum(u) != 0 || c == '_' || c == '-';
            })) {
        fail(error, "CurrentTechnique profile name is invalid");
        return TechniqueSelectorError::profile;
    }

    const auto selector_offset = word(parameter.raw_value + 4);
    std::string selector;
    if (!source_string(parameter.image, selector_offset, max_selector_bytes,
                       selector) || !valid_ascii_name(selector)) {
        fail(error, "CurrentTechnique selector string is missing or invalid");
        return TechniqueSelectorError::string;
    }
    output.parameter_id.assign(id);
    output.profile.assign(profile);
    output.name = std::move(selector);
    output.raw_tag = word(parameter.raw_value);
    return TechniqueSelectorError::ok;
}

TechniqueSelectorError material_current_techniques(
    const materials::Material& material, std::vector<CurrentTechnique>& output,
    std::string& error) {
    output.clear();
    error.clear();
    if (!material.image.bytes || !material.record || !material.id) {
        fail(error, "missing material or BRES view");
        return TechniqueSelectorError::argument;
    }
    for (std::uint32_t index = 0; index < material.parameter_count; ++index) {
        materials::Parameter parameter{};
        const auto parsed = dh2_material_parameter(
            &parameter, &material, static_cast<std::int32_t>(index));
        if (parsed != materials::Error::ok) {
            fail(error, "material parameter failed checked BRES decoding");
            output.clear();
            return parsed == materials::Error::index
                ? TechniqueSelectorError::range
                : TechniqueSelectorError::range;
        }
        const std::string_view id(parameter.id ? parameter.id : "");
        if (id.size() <= current_suffix.size() ||
            id.substr(id.size() - current_suffix.size()) != current_suffix)
            continue;
        CurrentTechnique decoded;
        const auto result = decode_current_technique(parameter, decoded, error);
        if (result != TechniqueSelectorError::ok) {
            output.clear();
            return result;
        }
        if (std::any_of(output.begin(), output.end(), [&](const auto& previous) {
                return previous.profile == decoded.profile;
            })) {
            fail(error, "material repeats a CurrentTechnique profile");
            output.clear();
            return TechniqueSelectorError::profile;
        }
        output.push_back(std::move(decoded));
    }
    return TechniqueSelectorError::ok;
}

TechniqueSelectorError effect_technique_name(
    const materials::EffectGroup& group, std::uint32_t index,
    std::string& output, std::string& error) {
    output.clear();
    error.clear();
    if (!group.image.bytes) {
        fail(error, "missing effect group or BRES view");
        return TechniqueSelectorError::argument;
    }
    if (index >= group.named_count) {
        fail(error, "effect named-record index is outside the group");
        return TechniqueSelectorError::range;
    }
    if (!group.named_records || group.named_count > 255 ||
        !borrowed(group.image, group.named_records,
                  std::size_t(group.named_count) * 12)) {
        fail(error, "effect named-record table is not a bounded byte ordinal table");
        return group.named_count > 255 ? TechniqueSelectorError::limit
                                       : TechniqueSelectorError::range;
    }
    const auto* record = group.named_records + std::size_t(index) * 12;
    const auto name_offset = word(record);
    if (!source_string(group.image, name_offset, max_selector_bytes, output) ||
        !valid_ascii_name(output)) {
        output.clear();
        fail(error, "effect technique name is missing or invalid");
        return TechniqueSelectorError::string;
    }
    return TechniqueSelectorError::ok;
}

TechniqueSelectorError renderer_technique_ordinal(
    std::string_view selector, const std::vector<std::string>& ordered_names,
    std::uint8_t& ordinal, std::string& error) {
    ordinal = 0xff;
    error.clear();
    if (selector.empty()) {
        fail(error, "empty renderer technique selector");
        return TechniqueSelectorError::argument;
    }
    if (ordered_names.size() > 255) {
        fail(error, "renderer technique count exceeds byte ordinal range");
        return TechniqueSelectorError::limit;
    }
    for (std::size_t i = 0; i < ordered_names.size(); ++i) {
        if (ordered_names[i] == selector) {
            ordinal = static_cast<std::uint8_t>(i);
            return TechniqueSelectorError::ok;
        }
    }
    fail(error, "renderer technique selector is absent");
    return TechniqueSelectorError::not_found;
}

TechniqueSelectorError effect_technique_ordinal(
    const materials::EffectGroup& group, std::string_view selector,
    std::uint8_t& ordinal, std::string& error) {
    ordinal = 0xff;
    error.clear();
    if (selector.empty()) {
        fail(error, "empty effect technique selector");
        return TechniqueSelectorError::argument;
    }
    if (group.named_count > 255) {
        fail(error, "effect technique count exceeds byte ordinal range");
        return TechniqueSelectorError::limit;
    }
    for (std::uint32_t i = 0; i < group.named_count; ++i) {
        std::string name;
        const auto result = effect_technique_name(group, i, name, error);
        if (result != TechniqueSelectorError::ok) return result;
        if (name == selector) {
            ordinal = static_cast<std::uint8_t>(i);
            return TechniqueSelectorError::ok;
        }
    }
    fail(error, "selector is absent from serialized effect group");
    return TechniqueSelectorError::not_found;
}
}
