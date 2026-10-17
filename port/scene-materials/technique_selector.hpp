#pragma once

#include "../material-bindings/bindings.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

// Bounded readers for COLLADA/BRES CurrentTechnique selectors. These are new
// checked views over serialized fields, not the original engine material ABI.
namespace dh2::scene_materials {
enum class TechniqueSelectorError : std::uint32_t {
    ok,
    argument,
    kind,
    count,
    range,
    string,
    profile,
    limit,
    not_found,
};

struct CurrentTechnique {
    std::string parameter_id;
    std::string profile;
    std::string name;
    // The first serialized word in the type-20 value. Its meaning is not
    // established by this reader and is deliberately retained without a name.
    std::uint32_t raw_tag = 0;
};

// Decode one already-validated material parameter. Only a parameter whose ID
// ends with /CurrentTechnique and whose type is 20 is accepted. BRES offsets
// remain four-byte source offsets; no serialized pointer is dereferenced
// before a range check.
TechniqueSelectorError decode_current_technique(
    const materials::Parameter& parameter,
    CurrentTechnique& output,
    std::string& error);

// Return every profile-specific CurrentTechnique parameter in a material.
// On malformed matching parameters or duplicate profile IDs, output is empty.
TechniqueSelectorError material_current_techniques(
    const materials::Material& material,
    std::vector<CurrentTechnique>& output,
    std::string& error);

// Read one selector name from a checked effect group's 12-byte named-record
// table. Other named-record words remain opaque.
TechniqueSelectorError effect_technique_name(
    const materials::EffectGroup& group,
    std::uint32_t index,
    std::string& output,
    std::string& error);

// Reproduce CMaterialRenderer::getTechniqueID's bounded lookup over an
// ordered renderer-technique name list: first exact match wins, the ordinal
// is one byte, and 0xff means absent. A 255-entry list has valid ordinals
// 0..254; a 256th entry cannot be represented distinctly from the sentinel.
TechniqueSelectorError renderer_technique_ordinal(
    std::string_view selector,
    const std::vector<std::string>& ordered_names,
    std::uint8_t& ordinal,
    std::string& error);

// Convenience check against the serialized named table. The returned ordinal
// is explicitly the table ordinal, not a claim that the runtime's compiled
// renderer retained every source entry in the same order.
TechniqueSelectorError effect_technique_ordinal(
    const materials::EffectGroup& group,
    std::string_view selector,
    std::uint8_t& ordinal,
    std::string& error);
}
