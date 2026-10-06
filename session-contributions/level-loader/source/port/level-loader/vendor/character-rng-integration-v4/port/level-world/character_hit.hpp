#pragma once
#include "character_target_providers.hpp"
#include "../game-data/properties.hpp"
namespace dh2::character {
struct HitActor32 {
 std::uintptr_t identity;
 data::PropertyView* properties;
 std::uintptr_t controller;
 std::int32_t lifecycle;
 std::uint32_t reserved;
};
struct HitAttacker24 {
 std::uintptr_t identity;
 target_providers::Handle16* shared_handle;
 target_providers::Registry24* registry;
};
enum HitService : std::uint32_t {
 hit_is_dead=0,hit_main_player,hit_debug_load,hit_debug_query,
 hit_is_monster,hit_online,hit_application_switch,hit_is_player,
 hit_controller_kill,hit_is_remotely_updated,hit_is_character
};
struct HitRequest32 {
 std::uint32_t service,force;
 std::uintptr_t subject,target;
 const char* name;
};
struct HitServices16 {
 void* context;
 // Zero means actual synchronous delivery. Queries write a native identity
 // or source boolean. main_player is GetCoopGame(0,true)->mainPlayer (+660),
 // not a replacement host-player query. Kill is the full Cmd_Kill call and
 // its required backend, including source Ctrl_Kill/Kill services.
 int(*invoke)(void*,HitActor32*,const HitRequest32*,std::uintptr_t*);
};
struct HitResult40 {
 std::uint32_t phase,calls;
 std::int32_t raw_add,before_hp,maximum_hp,after_hp,credited_damage;
 std::uint32_t kill_called,lifecycle_written;
 std::int32_t status;
};
static_assert(sizeof(HitActor32)==32&&sizeof(HitAttacker24)==24);
static_assert(sizeof(HitRequest32)==32&&sizeof(HitServices16)==16&&sizeof(HitResult40)==40);
}
extern "C" {
// Actual Character::HitFor(3a8bc4), bounded offline/nonplayer branch. Dead
// receivers return before any game/debug/property work. All live queries,
// property writes, controller rereads and handle/cast side effects retain
// original order. Null attacker is accepted for the dead prefix only; reaching
// its original unsafe GetHandle dereference is an explicit unsupported boundary.
// Online, receiver-player and distinct-player-attacker achievement paths
// return -3 at their reached boundary. They are not silently accepted.
// Nonlethal self/self monster HitFor has no unconditional AI/hit-FX tail.
// Lethal HitFor requires real Kill delivery; this module never marks dead.
// 1 complete, -1 malformed atomic, -2 required provider/capacity failure,
// -3 unsupported after source effects. phase=last requested service+1.
// Receiver/attacker/services/output and property/registry projections do not
// overlap. Callback receivers and all pointer/backing identities survive the
// invocation. Callbacks may change sheet values/controller/lifecycle, not
// replace projection pointers or registry backing/order/capacity.
int dh2_character_hit_for(dh2::character::HitResult40*,dh2::character::HitActor32*,
 std::uint32_t damage,const dh2::character::HitAttacker24*,
 const dh2::character::HitServices16*);
}
