#pragma once

#include "../engine-resources/resources.hpp"

// Immutable, checked views of the serialized BRES records. These are new port
// interfaces, not recovered studio C++ objects or an ARM32 ABI replacement.
namespace dh2::materials {
enum class Error : std::uint32_t { ok, argument, index, string, layout, kind };

struct Image {
    resources::BresView image;
    const char* id;
    const char* name;
    const char* source_path;
    std::uint32_t raw_word_12;
    std::uint32_t raw_word_16;
};

struct Effect {
    resources::BresView image;
    const char* id;
    const char* name;
    const std::uint8_t* record;
};

// Each effect has two observed groups. Their three count/offset pairs contain
// 12-byte named records, 24-byte parameters, and 32-bit image references.
// The groups' runtime selection and named-record payloads are unresolved.
struct EffectGroup {
    resources::BresView image;
    std::uint32_t named_count, parameter_count, image_count;
    const std::uint8_t* named_records;
    const std::uint8_t* parameter_records;
    const std::uint8_t* image_indices;
};

struct Parameter {
    resources::BresView image;
    const char* id;
    const char* semantic;
    std::uint32_t type_code, value_count;
    const std::uint8_t* raw_value; // Type-specific serialized data.
};

// Effect parameters share the 24-byte stride but have numeric words at +4/+8;
// material parameters instead store a semantic string at +4.
struct EffectParameter {
    resources::BresView image;
    const char* id;
    std::uint32_t type_code, raw_word_8, value_count;
    const std::uint8_t* raw_value;
};

struct ImageRef {
    std::int32_t index; // -1 is the serialized 0xffffffff sentinel.
    const char* id;
    const char* name;
    const char* source_path;
};

struct Material {
    resources::BresView image;
    const char* id;
    const char* name;
    const char* external_effect_file; // Null for an effect in this BRES image.
    const char* effect_url; // Observed values begin with '#'.
    const std::uint8_t* record; // Borrowed raw source record.
    std::uint32_t parameter_count;
    const std::uint8_t* parameter_records;
};
}

extern "C" {
dh2::materials::Error dh2_image_record(dh2::materials::Image*,
                                        const dh2::resources::BresView*, std::int32_t);
dh2::materials::Error dh2_effect_record(dh2::materials::Effect*,
                                         const dh2::resources::BresView*, std::int32_t);
dh2::materials::Error dh2_material_record(dh2::materials::Material*,
                                           const dh2::resources::BresView*, std::int32_t);
dh2::materials::Error dh2_effect_group(dh2::materials::EffectGroup*,
                                       const dh2::materials::Effect*, std::int32_t);
dh2::materials::Error dh2_effect_parameter(dh2::materials::EffectParameter*,
                                           const dh2::materials::EffectGroup*, std::int32_t);
dh2::materials::Error dh2_effect_group_image(dh2::materials::ImageRef*,
                                             const dh2::materials::EffectGroup*, std::int32_t);
dh2::materials::Error dh2_material_parameter(dh2::materials::Parameter*,
                                             const dh2::materials::Material*, std::int32_t);
// Type-code 11 material parameters contain a pointer to a signed image index.
dh2::materials::Error dh2_material_sampler_image(dh2::materials::ImageRef*,
                                                 const dh2::materials::Material*, std::int32_t);
// Returns a zero-based index for an effect URL resolved in the same BRES file.
// Returns -1 for an external effect or an unresolved name.
std::int32_t dh2_material_local_effect(const dh2::materials::Material*);
}
