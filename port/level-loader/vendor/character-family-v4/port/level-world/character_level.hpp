#pragma once
#include "../game-data/properties.hpp"
#include "../script-runtime/script_design_bindings.h"
namespace dh2::character {
struct LevelModel32 {
 data::PropertyView* properties;
 const data::ClassRow* classes;
 std::uint32_t class_count,reserved;
 const dh2_script_design_bindings* design;
};
enum LevelService : std::uint32_t {level_debug_load=1,level_debug_query};
struct LevelRequest24 {std::uint32_t service,property;std::int32_t retained_delta;std::uint32_t reserved;const char* name;};
struct LevelServices16 {
 void* context;
 // Genuine DebugSwitches.load/GetSwitch effects; return is ignored by source.
 // Delivery0, failure nonzero. Called only before a positive retained Add.
 // Property backing/receiver survives synchronous calls; do not retarget the
 // retained properties pointer. Sheet mutations preserve source rereads.
 // No same-VM reentry.
 int(*invoke)(void*,LevelModel32*,const LevelRequest24*);
};
struct LevelBindings48 {LevelModel32* model;LevelServices16 services;std::uint64_t reserved[3];};
static_assert(sizeof(LevelModel32)==32&&sizeof(LevelRequest24)==24&&sizeof(LevelServices16)==16&&sizeof(LevelBindings48)==48);
}
// Raw fixed-point f32 argument; source signed upper cap only. Writes base19,
// applies actual class/base formulas, resolves224 then HP and MP in order.
// 0 success,1 malformed before effects,2 provider/data failure after prefixes.
// Mutable sheets are genuine caller-produced properties; lifetime/disjoint
// backing is required. Base/saved/gear/class/buff producers remain borrowed.
extern "C" int dh2_character_set_level(dh2::character::LevelModel32*,float,
 const dh2::character::LevelServices16*);
// Source GetLevel: cached property19 ASR8, no recalc/fixed float division.
extern "C" int dh2_character_get_level(std::int32_t*,const dh2::data::PropertyView*);
extern "C" int dh2_character_set_level_lua(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
extern "C" int dh2_character_level_bind(dh2_script_vm*,const dh2::character::LevelBindings48*);
