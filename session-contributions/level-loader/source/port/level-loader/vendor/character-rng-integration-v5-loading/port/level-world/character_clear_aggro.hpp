#pragma once
#include "character_target_bindings.hpp"
#include "../script-runtime/script_object_bridge.h"
#include "../game-data/aggro.hpp"
namespace dh2::character {
struct ClearAggroState16 {TargetState48* receiver;data::AggroTable* outgoing;};
struct ClearAggroCharacter32 {std::uintptr_t identity;data::AggroTable* incoming;TargetBindings48* target;std::uintptr_t controller;};
enum ClearAggroService:std::uint32_t {clear_aggro_notify,clear_aggro_stop};
struct ClearAggroRequest32 {std::uint32_t service,reserved;std::uintptr_t receiver,other;const dh2_script_callback_scope* scope;};
struct ClearAggroServices24 {
 void* context;
 int(*invoke)(void*,ClearAggroState16*,ClearAggroCharacter32*,const ClearAggroRequest32*);
 // Native ownership marshal of the original raw Character Value identity.
 // Must return its genuine retained projection, or delivery failure.
 int(*resolve)(void*,std::uintptr_t,ClearAggroCharacter32**);
};
struct ClearAggroBindings16 {ClearAggroState16* state;const ClearAggroServices24* services;};
static_assert(sizeof(ClearAggroState16)==16&&sizeof(ClearAggroCharacter32)==32&&sizeof(ClearAggroRequest32)==32&&sizeof(ClearAggroServices24)==24&&sizeof(ClearAggroBindings16)==16);
}
// Source AI_ClearAggro storage/notification/ownership coordinator. Null target
// returns before dereferencing receiver storage. Services and all borrowed
// records survive synchronous callbacks; no missing provider is accepted.
// 0complete;1malformed entry;2failed service/lifetime after observed prefix.
extern "C" int dh2_character_clear_aggro(dh2::character::ClearAggroState16*,
 dh2::character::ClearAggroCharacter32*,const dh2::character::ClearAggroServices24*,
 const dh2_script_callback_scope* scope);
// Genuine source Lua callback body on projected Values; accepts first2/7,
// ignores extras, adds zero results. Native scoped callback owns no VM.
extern "C" int dh2_character_clear_aggro_values(dh2::character::ClearAggroBindings16*,
 const dh2_script_callback_scope*,const dh2_script_value*,std::uint32_t);
extern "C" int dh2_character_clear_aggro_scoped(void*,const dh2_script_callback_scope*,
 const dh2_script_value*,std::uint32_t,char*,std::size_t);
extern "C" int dh2_character_clear_aggro_bind(dh2_script_vm*,dh2::character::ClearAggroBindings16*);
