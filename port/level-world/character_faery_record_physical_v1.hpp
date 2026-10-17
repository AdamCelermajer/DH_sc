#pragma once

#include "character_physical_component_v1.hpp"
#include "character_runtime_factory_v1.hpp"

#include <string>

namespace dh2::character_faery_record_physical_v1 {

namespace physical = dh2::physical;
namespace factory = character_runtime_factory_v1;
namespace physical_component = character_physical_component_v1;

enum class Status : unsigned char {
    complete,
    incomplete_record,
    source_owner_missing,
    already_attached,
    identity_mismatch,
    owner_rejected,
};

// Attaches the typed Box2D physical component to the existing factory Record.
// The source POFaerie token must come from the real source constructor/provider;
// this adapter never manufactures a source pointer or changes manager/roster rows.
Status attach(factory::Record&, std::uintptr_t source_pofaerie_identity,
              physical::NativeWorld&, const physical::CharacterBodyInput&,
              physical::WorldObject&, std::string& error);
Status set_transform(factory::Record&, std::uintptr_t source_pofaerie_identity,
                     float game_x, float game_y, float angle,
                     std::string& error);
Status observe(const factory::Record&, std::uintptr_t source_pofaerie_identity,
               physical_component::Transform*, std::string& error);
Status detach(factory::Record&, std::uintptr_t source_pofaerie_identity,
              std::string& error);

} // namespace dh2::character_faery_record_physical_v1
