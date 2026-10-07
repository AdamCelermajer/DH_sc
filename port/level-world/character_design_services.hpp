#pragma once
#include "character_level.hpp"
namespace dh2::character {
struct DebugFileServices24 {
 void* context;
 // Delivery0. A zero handle is genuine filesystem/open notfound. No provider
 // is treated as an absent file. Providers remain alive, do not throw/reenter
 // this owner, and close an actually returned file through close_read.
 int(*open_read)(void*,const char*,std::uintptr_t*);
 int(*close_read)(void*,std::uintptr_t);
};
struct DebugSwitches;
struct DebugLevelBinding16 {DebugSwitches* owner;const DebugFileServices24* files;};
static_assert(sizeof(DebugFileServices24)==24&&sizeof(DebugLevelBinding16)==16);
}
// Genuine owned source singleton projection: empty switch map and loaded0.
// Missing-file loading/query insertion is implemented. An existing file is
// closed and returns -3 (parser outside this staged domain), retaining loaded1;
// this native incomplete owner remains quarantined until destroyed. No values
// from an unreconstructed file are published. No global Application is owned.
extern "C" dh2::character::DebugSwitches* dh2_character_debug_create();
extern "C" void dh2_character_debug_destroy(dh2::character::DebugSwitches*);
// 1 completed,-1 malformed,-2 provider/allocation failure,-3 existing file.
extern "C" int dh2_character_debug_load(dh2::character::DebugSwitches*,const dh2::character::DebugFileServices24*);
extern "C" int dh2_character_debug_get(std::uint32_t*,dh2::character::DebugSwitches*,const char*,const dh2::character::DebugFileServices24*);
//Whole SetSwitch337ddc map/query prefix. Changed values retain their source
//store, then require the actual Debug.save backend (return-3), never skip it.
extern "C" int dh2_character_debug_set_v102(dh2::character::DebugSwitches*,const char*,std::uint8_t,const dh2::character::DebugFileServices24*);
// Read-only owned-map diagnostics; names borrow owner storage. No insertion.
extern "C" int dh2_character_debug_snapshot(const dh2::character::DebugSwitches*,std::uint32_t*,std::uint32_t*);
extern "C" int dh2_character_debug_entry(const dh2::character::DebugSwitches*,std::uint32_t,const char**,std::uint32_t*);
// Exact LevelServices16 callback; query result is deliberately ignored by
// source. Context/owners/files outlive Level bindings and all VM callbacks.
extern "C" int dh2_character_debug_level_service(void*,dh2::character::LevelModel32*,const dh2::character::LevelRequest24*);
// Original CharAI.OnInit group/key projection for timer33/34. Output unchanged
// on malformed/delivery failure; signed word bits retained as timer duration.
extern "C" int dh2_character_design_tick(std::uint32_t*,const dh2_script_design_bindings*,std::uint32_t);
