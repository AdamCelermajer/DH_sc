#pragma once
#include "../script-runtime/script_runtime.h"
#include <cstddef>
#include <cstdint>
namespace dh2::character {
// Logical payload of Structs::CharacterProperties; excludes ARM32 vtable word.
// Source table offsets are 4*property for all 224 serialized fields.
struct PropertySheet16 {const std::int32_t* words;std::uint32_t count,reserved;};
using PropertySheetResolver=int(*)(void*,std::uintptr_t,PropertySheet16*);
struct PropertyBindings48 {
 // temporary is source CharProperties::s_temp, not the defaults/base sheet.
 // Supply its genuine current producer-owned contents separately.
 PropertySheet16 resolved,temporary;
 void* context;
 // Map a genuine source light-userdata sheet identity to its borrowed payload.
 // No arbitrary pointer dereference/truncation. Return0 delivers a sheet.
 // Context, sheet backing and identities remain live through VM callbacks.
 // Result/count/error storage is disjoint from all borrowed inputs/sheets.
 // Resolver must not destroy the receiver/backing or reenter this Lua VM.
 PropertySheetResolver sheet;
};
static_assert(sizeof(PropertySheet16)==16&&sizeof(PropertyBindings48)==48);
}
// Source _GetProperty/PROPS_GetFromSheet read-only word getter. Out-of-range
// signed property or null sheet returns signed -1 (nonfatal diagnostic mode).
// Nonnull projections are validated even for invalid indices. Malformed input
// returns1, output unchanged. No recalculation or fixed-point conversion.
extern "C" int dh2_character_property_word(std::int32_t*,
 const dh2::character::PropertySheet16*,std::int32_t);
// Character::_GetProp with source-projected Value types: number first,
// optional boolean selects shared temporary/cached; only type2 is external.
// No values for guard failures/null type2. Raw signed word -> Lua float.
extern "C" int dh2_character_get_prop(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
extern "C" int dh2_character_property_bind(dh2_script_vm*,
 const dh2::character::PropertyBindings48*);
