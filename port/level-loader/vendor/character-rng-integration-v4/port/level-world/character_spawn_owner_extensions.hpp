#pragma once
#include "character_state_owner_extensions.hpp"
#include "character_spawn_body.hpp"
#include "character_spawn_select.hpp"
#include "character_spawn_permission.hpp"
#include "character_timers.hpp"
namespace dh2::character {
// Additive native composition, not an original wrapper. Install methods after
// StateOwnerBehavior and updates after StateOwnerFrame. All nine borrowed
// objects, their providers and captured row backings survive synchronous reentry.
// outer_methods is the FULL behavior chain, used for real owner transitions.
struct SpawnOwnerExtensions72 {
 StateOwnerMachine40* machine;
 const StateOwnerExtensions48* remaining;
 SpawnBody48* spawn;
 const SpawnBodyServices16* spawn_services;
 NativeSpawn24* selection;
 const SpawnPermission16* permission;
 TimerStore32* timers;
 const TimerServices32* timer_services;
 const StateOwnerServices16* outer_methods;
};
static_assert(sizeof(SpawnOwnerExtensions72)==72);
}
extern "C" {
// StateOwner callback convention:0 delivered, nonzero prefix failure. Genuine
// Spawn1 Focus/Blur/Event, original empty Update, and PreSpawn17 permission and
// selection are composed. Every other operation/method delegates unchanged.
int dh2_character_spawn_owner_extensions_method(void*,dh2::character::StateOwnerMachine40*,
 const dh2::character::StateOwnerRequest48*,dh2::character::StateOwnerResponse8*);
int dh2_character_spawn_owner_extensions_update(void*,dh2::character::StateOwnerMachine40*,
 const dh2::character::StateOwnerUpdateRequest24*);
// Source SM_SetSpawnState composition. No direct current write. Timer expiry
// is borrowed from the caller (AI/Character event routing remains separate).
//1 completed,-1 malformed before effects,-2 required provider failure prefix.
int dh2_character_spawn_owner_select(dh2::character::SpawnOwnerExtensions72*,
 std::uint32_t delay_enabled,std::uint32_t ignored_mode);
}
