#pragma once

#include "character_template_factory.hpp"

#include <cstdint>
#include <string>

namespace dh2::world { struct Object; }

namespace dh2::world::crypt_template_character_projection_v1 {

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    not_character,
    missing_source_identity,
    template_resolution_failed,
};

// A generated MGP Character's source identity and runtime property route.
// The editor primitive label is retained separately and is never used as a
// fallback for char_template_pydata/char_template.
struct Descriptor {
    std::uint32_t module_index{};
    std::uint32_t source_record{};
    std::string source_path;
    std::string object_name;
    std::string object_type;
    character::template_factory::Source property_source;
    character::template_factory::Resolution property_resolution;
    // False when the caller only has MGP metadata and has not loaded the
    // Character template catalog needed to resolve authored alternatives.
    bool property_resolution_attempted = false;
    float world_position[3]{};
    float local_rotation_degrees[3]{};
    float local_scale[3]{};
};

// Projects one imported MGP Character through the existing source-backed
// template resolver. With the default selection, multi-slot templates return
// selection_required and retain their authored ordered slots, including
// duplicates. This function does not draw RNG or allocate a runtime actor.
Status project(const Object& object,
               const character::template_factory::Catalog& catalog,
               std::int32_t selected_alternative,
               Descriptor* output,
               std::string& error);

// Retains exact MGP identity, template attributes and transform without
// resolving the property route. Use when the importer has no source Catalog;
// property_resolution remains explicitly unattempted.
Status retain_source(const Object& object, Descriptor* output,
                    std::string& error);

const char* status_name(Status status) noexcept;

}  // namespace dh2::world::crypt_template_character_projection_v1
