#pragma once
#include "../script-runtime/script_runtime.h"
#include <cstddef>
#include <cstdint>
namespace dh2::character {
struct TargetOwner16 {
 std::uintptr_t identity;
 std::uint16_t word14d0,reserved16;
 std::uint32_t reserved;
};
struct TargetState48 {
 std::uintptr_t identity;
 TargetOwner16* owner;
 std::uintptr_t candidate,target,last_target;
 std::uint8_t alive,sight,changed,reserved8;
 std::uint32_t reserved;
};
enum TargetService : std::uint32_t {target_debug_load=0,target_debug_query,
 target_owner_ai_id,target_virtual_dead,target_in_sight};
struct TargetRequest24 {std::uint32_t service,reserved;std::uintptr_t subject;const char* text;};
struct TargetServices16 {
 void* context;
 // Synchronous original calls. Return0 delivers; nonzero fails after the
 // executed prefix. Query outputs are raw words, NOT forced Boolean values.
 // Providers may change the live State/Owner at original reload points, or
 // recursively call the setter, but must retain receiver/backing lifetimes.
 int(*invoke)(void*,TargetState48*,const TargetRequest24*,std::uint32_t*);
};
struct TargetBindings48 {
 TargetState48* state;
 TargetServices16 services;
 // Ephemeral capability only during SetTarget/ClearTarget. Providers can use
 // its dedicated discarded source call, never generic same-VM reentry. Do
 // not retain it after return or destroy this binding/VM while executing.
 const dh2_script_callback_scope* scope;
 std::uint64_t reserved[2];
};
static_assert(sizeof(TargetOwner16)==16&&sizeof(TargetState48)==48);
static_assert(sizeof(TargetRequest24)==24&&sizeof(TargetServices16)==16&&sizeof(TargetBindings48)==48);
}
extern "C" {
// 0 complete;1 malformed entry before effects;2 failed provider/lifetime
// projection after prefixes. The incoming identity remains captured across
// callbacks. mode!=0 only writes candidate and target. Owner may be null for
// paths that never use it; nonnull Owner must be a valid borrowed projection.
int dh2_character_ai_set_target(dh2::character::TargetState48*,std::uintptr_t,
 std::uint32_t mode,const dh2::character::TargetServices16*);
int dh2_character_ai_sync_last_target(dh2::character::TargetState48*);
int dh2_character_clear_target(dh2::character::TargetState48*,const dh2::character::TargetServices16*);
// Read-only source scalar/result identity. GetTarget's Lua GameObject Value
// producer is NOT replaced by this identity getter or installed as a global.
int dh2_character_has_target(std::uint32_t*,const dh2::character::TargetState48*);
int dh2_character_target_identity(std::uintptr_t*,const dh2::character::TargetState48*);
// Complete discarded GetCharAIId helper: signed cached property1, valid ID
// iff nonnegative and below the genuine global AI row count, otherwise8.
int dh2_character_target_ai_id(std::int32_t*,const std::int32_t* resolved224,std::uint32_t ai_count);
// Exact sight arithmetic over genuine GetTargetPosition/radius results.
// Strict radius*radius > ((dx*dx+dy*dy)+dz*dz), with all IEEE values allowed.
int dh2_character_target_sight(std::uint32_t*,const float*,const float*,float);
int dh2_character_target_has_lua(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
int dh2_character_target_set_values(dh2::character::TargetBindings48*,const dh2_script_value*,std::uint32_t);
// Installs HasTarget and scoped zero-return SetTarget/ClearTarget only.
// Source SetTarget accepts first Value type2 OR7 (including null identity),
// ignores additional arguments, and rejects nil/string/other kinds silently.
int dh2_character_target_bind(dh2_script_vm*,dh2::character::TargetBindings48*);
}
