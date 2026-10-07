#pragma once
#include "../game-data/properties.hpp"
#include <cstdint>
namespace dh2::character {
// One retained Character. Only fields accessed by Kill are projected here;
// original animation/physics/state behavior belongs to the RaiseEvent backend.
struct KillActor56 {
 std::uintptr_t identity;data::PropertyView* properties;
 std::uintptr_t killer;KillActor56* owner;std::uintptr_t tracked_target;
 std::int32_t oid;std::int16_t property_id,template_id;
 std::uint8_t dead,suppress_quest,reserved[6];
};
struct KillLevel16 {std::uintptr_t identity;std::uint32_t loot_gate150,reserved;};
struct KillWorld16 {std::uintptr_t trophy_manager,constants;};
// Copyable projection of the source stack QE_* IEvent. The async provider
// must copy/own it before returning; keeping this borrowed pointer is invalid.
struct KillQuest48 {
 std::uint32_t kind,type;std::uintptr_t killer;
 std::int32_t oid,network_id,subject_id;
 std::uint8_t flag0,flag1;std::uint16_t reserved;
 std::uintptr_t level;std::uint64_t reserved2;
};
enum KillService:std::uint32_t {
 kill_is_dead=0,kill_is_player,kill_is_local_player,kill_online,
 kill_get_local_player,kill_current_level,kill_drop_loot,kill_aggro_count,
 kill_aggro_entry,kill_raise_event,kill_trophy_id,kill_unlock_trophy,
 kill_is_character,kill_handle_character,kill_distribute_xp,
 kill_is_remotely_updated,kill_constant,kill_raise_async
};
struct KillRequest56 {
 std::uint32_t service;std::int32_t argument;
 std::uintptr_t subject,target;const char* name;const char* key;
 const KillQuest48* event;std::int32_t index;std::uint32_t reserved;
};
struct KillResponse16 {std::uintptr_t pointer;std::int32_t word;std::uint32_t reserved;};
struct KillServices16 {
 void* context;
 //0 actual synchronous delivery. current_level/aggro_entry return retained
 // KillLevel16*/KillActor56* in pointer; handle_character returns the actual
 // nullable GetHandle->Character-cast identity (existing genuine handle helper
 // may supply this). Queries write their actual word/identity. Game/loot/XP/
 // trophies/quest/AI backends are required, never accepted as no-op fixtures.
 int(*invoke)(void*,KillActor56*,const KillRequest56*,KillResponse16*);
};
struct KillResult24 {std::uint32_t phase,calls,dead_written,events_raised,trophies_unlocked;std::int32_t status;};
static_assert(sizeof(KillActor56)==56&&sizeof(KillLevel16)==16&&sizeof(KillWorld16)==16);
static_assert(sizeof(KillQuest48)==48&&sizeof(KillRequest56)==56&&sizeof(KillResponse16)==16&&sizeof(KillServices16)==16&&sizeof(KillResult24)==24);
}
extern "C" {
// Complete recoverable Character.Kill body, with explicit deeper services.
// Source player/trophy, force, nullable attacker, dynamic aggro/owner/quest gates
// and property/field order are retained. Actor/world/pointers/backings survive
// all callbacks; mutable actor values and global world identities remain live.
// No arbitrary count cap, callback suppression, animation or physics mutation.
// Provider responses are truthful retained projections, not fabricated owners.
// 1 complete,-1 malformed atomic,-2 required service/provider failure,-3 unsafe
// source null current-Level producer. Source effects before failure remain.
int dh2_character_kill(dh2::character::KillResult24*,dh2::character::KillActor56*,
 std::uintptr_t attacker,std::uint32_t force,dh2::character::KillWorld16*,const dh2::character::KillServices16*);
// Original Ctrl_Kill: live outer IsDead gate, actual Kill (which queries again),
// then RaiseEvent(2,attacker), including when the inner Kill skipped on its own
// reread. A failed required backend does not get a fake successful event tail.
int dh2_character_ctrl_kill(dh2::character::KillResult24*,dh2::character::KillActor56*,
 std::uintptr_t attacker,std::uint32_t force,dh2::character::KillWorld16*,const dh2::character::KillServices16*);
}
