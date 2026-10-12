#pragma once
#include "character_state.hpp"
#include "character_state_empty.hpp"
namespace dh2::character {
// Original CharAnim rows are40 signed words (160B); use genuine decoded rows.
// Borrowed backings remain alive across synchronous callbacks, including rows
// whose pointer was captured before a later table/index provider changes it.
struct PreSpawnState48 {
 State* state;std::uintptr_t character;const std::int32_t(*animation_rows)[40];
 std::uint32_t animation_count,stay_enabled,ai_kind,reserved;
 // Projection of the pointed Character+3fc object word+24. Caller refreshes
 // kind/pointer after SetSpawnState before returning. This is not AIS ownership.
 std::uint32_t* ai_word;
};
enum PreSpawnService:std::uint32_t {
 pre_spawn_animation_index,pre_spawn_stance_mask,pre_spawn_stance,
 pre_spawn_set_animation,pre_spawn_set_speed,pre_spawn_remove_physical,
 pre_spawn_enable,pre_spawn_disable_collisions,pre_spawn_revive,
 pre_spawn_enable_collisions,pre_spawn_init_physical,pre_spawn_predicate,
 pre_spawn_set_spawn,pre_spawn_assert_policy,pre_spawn_assert_log
};
struct PreSpawnRequest32 {std::uint32_t service,argument0,argument1,argument2;std::uintptr_t character,payload;};
struct PreSpawnResponse8 {std::uint32_t word;std::int32_t next;};
struct PreSpawnServices16 {void* context;int(*invoke)(void*,PreSpawnState48*,const PreSpawnRequest32*,PreSpawnResponse8*);};
static_assert(sizeof(PreSpawnState48)==48&&sizeof(PreSpawnRequest32)==32);
static_assert(sizeof(PreSpawnResponse8)==8&&sizeof(PreSpawnServices16)==16);
//0 delivered; nonzero fails at the delivered source prefix. Providers can
// synchronously reenter the state owner; this body does not perform transitions.
// animation_index is actual GetCharAnimTableId (including its proven fallback);
// stance_mask is AnimStancedAnim/SL__LIST_IPHONE; stance is actual GetAnimStance.
// set_animation argument0=sequence signed bits; speed argument0=float32 bits.
// remove_physical=SetPhysicalObject(NULL,false), enable argument0=source v40 bool;
// revive=Revive(NULL,false), init_physical=actual InitPhysicalObject.
// predicate args event,state17; payload original event payload; response.next
// starts1 and is by-reference, response.word is actual CSM_Spawn return word.
// set_spawn args true,false; assertion policy/log model actual source debug
// manager dependency. Policy2 source would deliberately dereferenceNULL: native
// returns-3. Policy1 requires real diagnostic delivery at source line114.
}
extern "C" {
// Methods use StateMethod selectors0..3, current17 required on entry. Focus/
// Blur ignore event/payload; Update genuinely empty. Event28 payload must be a
// borrowed NUL-terminated string; all other payloads remain opaque identities.
//1 complete,-1 malformed atomic,-2 missing/failed source provider at prefix,
//-3 original invalid row/null-pointer/assertion boundary. No effect rollback.
int dh2_character_pre_spawn_body(dh2::character::PreSpawnState48*,std::uint32_t operation,std::uint32_t event,std::uintptr_t payload,const dh2::character::PreSpawnServices16*);
}
