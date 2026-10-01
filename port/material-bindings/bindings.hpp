#pragma once

#include "../engine-resources/resources.hpp"

// Immutable, checked views of the serialized BRES records. These are new port
// interfaces, not recovered studio C++ objects or an ARM32 ABI replacement.
namespace dh2::materials {
enum class Error : std::uint32_t { ok, argument, index, string };

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

struct Material {
    resources::BresView image;
    const char* id;
    const char* name;
    const char* external_effect_file; // Null for an effect in this BRES image.
    const char* effect_url; // Observed values begin with '#'.
    const std::uint8_t* record; // Remaining fields are not interpreted yet.
};
}

extern "C" {
dh2::materials::Error dh2_image_record(dh2::materials::Image*,
                                        const dh2::resources::BresView*, std::int32_t);
dh2::materials::Error dh2_effect_record(dh2::materials::Effect*,
                                         const dh2::resources::BresView*, std::int32_t);
dh2::materials::Error dh2_material_record(dh2::materials::Material*,
                                           const dh2::resources::BresView*, std::int32_t);
// Returns a zero-based index for an effect URL resolved in the same BRES file.
// Returns -1 for an external effect or an unresolved name.
std::int32_t dh2_material_local_effect(const dh2::materials::Material*);
}
