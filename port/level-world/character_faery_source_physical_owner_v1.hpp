#pragma once

#include "character_physical_component_v1.hpp"
#include "character_runtime_factory_v1.hpp"
#include "character_scene.hpp"

#include <string>

namespace dh2::character_faery_source_physical_owner_v1 {

namespace factory = character_runtime_factory_v1;
namespace physical = dh2::physical;

enum class Status : unsigned char {
    complete,
    incomplete_record,
    invalid_source_bounds,
    already_attached,
    world_unavailable,
    body_creation_failed,
};

// Creates the semantic POFaerie heap owner after the source InitPhysicalObject
// provider selects the Faery branch. The passed bounds must come from the
// selected Character BDAE, character visual-scale producer, and resolved
// Collision_Scale owner-bounds producer. The supplied NativeWorld is the
// already-active Level/actor world; this provider never creates a world.
Status attach(factory::Record&, physical::NativeWorld&,
              const physical::CharacterOwnerBounds& source_bounds,
              std::string& error);

// Use before retiring the Character Record. The Record's existing physical
// attachment owns both the typed component and its callback context.
Status detach(factory::Record&, std::string& error);

} // namespace dh2::character_faery_source_physical_owner_v1
