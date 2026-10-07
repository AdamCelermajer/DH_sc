#pragma once
#include "character_pre_spawn.hpp"
#include "character_state_owner_frame.hpp"
namespace dh2::character {
// Native composition adapter, not a recovered original wrapper. Install the
// methods callback as StateOwnerBehaviorContext40.remaining and the updates
// callback as StateOwnerFrameContext56.other_updates. All borrowed projections
// and callback providers must survive synchronous reentry.
struct StateOwnerExtensions48 {
 PreSpawnState48* pre_spawn;
 const PreSpawnServices16* spawn_services;
 StateOwnerServices16 remaining_methods;
 StateOwnerUpdateServices16 remaining_updates;
};
static_assert(sizeof(StateOwnerExtensions48)==48);
}
extern "C" {
// Only original-proven empty methods and complete PreSpawn bodies are delivered.
// Other families and outer operations retain their explicit required provider.
// StateOwner callback convention:0 delivered, nonzero failure at source prefix.
int dh2_character_state_owner_extensions_method(void*,dh2::character::StateOwnerMachine40*,
 const dh2::character::StateOwnerRequest48*,dh2::character::StateOwnerResponse8*);
int dh2_character_state_owner_extensions_update(void*,dh2::character::StateOwnerMachine40*,
 const dh2::character::StateOwnerUpdateRequest24*);
}
