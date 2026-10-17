#include "crypt_template_character_projection_v1.hpp"
#include "../world-data/world.hpp"

#include <cstring>
#include <utility>

namespace dh2::world::crypt_template_character_projection_v1 {

Status project(const Object& object,
               const character::template_factory::Catalog& catalog,
               std::int32_t selected_alternative,
               Descriptor* output,
               std::string& error) {
    error.clear();
    if (!output || selected_alternative <
            character::template_factory::selection_required) {
        if (output) *output = {};
        error = "Crypt template Character projection arguments are invalid";
        return Status::invalid_argument;
    }

    Descriptor projected;
    const auto retained = retain_source(object, &projected, error);
    if (retained != Status::complete) {
        *output = {};
        return retained;
    }

    auto resolution = character::template_factory::resolve(
        projected.property_source, catalog, selected_alternative);
    if (resolution.status != character::template_factory::Status::resolved &&
        resolution.status != character::template_factory::Status::selection_required) {
        *output = {};
        error = std::string("Crypt template Character route rejected: ") +
            character::template_factory::status_name(resolution.status);
        return Status::template_resolution_failed;
    }
    projected.property_resolution = std::move(resolution);
    projected.property_resolution_attempted = true;
    *output = std::move(projected);
    return Status::complete;
}

Status retain_source(const Object& object, Descriptor* output,
                    std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output) {
        error = "Crypt template Character source output is missing";
        return Status::invalid_argument;
    }
    if (object.kind != RecordKind::mgp || !object.gametype ||
        std::strcmp(object.gametype, "Character") != 0) {
        error = "Crypt template Character projection requires an MGP Character";
        return Status::not_character;
    }
    if (!object.source_path || !*object.source_path || !object.name ||
        !*object.name) {
        error = "Crypt template Character source path/name is missing";
        return Status::missing_source_identity;
    }

    character::template_factory::Source source;
    source.object_name = object.name;
    const auto* editor_template = dh2_world_field(&object, "_templateName");
    source.editor_template_name = editor_template ? editor_template : "";
    const auto* data_class = dh2_world_field(&object, "char_template_pydata");
    source.template_data_class = data_class ? data_class : "";
    const auto* template_name = dh2_world_field(&object, "char_template");
    source.template_name = template_name ? template_name : "";
    const auto* explicit_property = dh2_world_field(&object, "charpropsname");
    source.explicit_property_name = explicit_property ? explicit_property : "";

    Descriptor projected;
    projected.module_index = object.module_index;
    projected.source_record = object.source_record;
    projected.source_path = object.source_path;
    projected.object_name = object.name;
    projected.object_type = object.gametype;
    projected.property_source = std::move(source);
    for (unsigned axis = 0; axis < 3; ++axis) {
        projected.world_position[axis] = object.world_position[axis];
        projected.local_rotation_degrees[axis] = object.local.rotation_degrees[axis];
        projected.local_scale[axis] = object.local.scale[axis];
    }
    *output = std::move(projected);
    return Status::complete;
}

const char* status_name(Status status) noexcept {
    switch (status) {
    case Status::complete: return "complete";
    case Status::invalid_argument: return "invalid_argument";
    case Status::not_character: return "not_character";
    case Status::missing_source_identity: return "missing_source_identity";
    case Status::template_resolution_failed: return "template_resolution_failed";
    }
    return "unknown";
}

}  // namespace dh2::world::crypt_template_character_projection_v1
