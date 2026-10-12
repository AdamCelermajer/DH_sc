#pragma once
#include "character_state.hpp"
#include "character_state_empty.hpp"
namespace dh2::character {
// Borrowed source Character, decoded CharAnim rows (40signed words), live
// VisualObject identity+2d8 and float32 fade word+1440. Refresh live fields at
// synchronous provider return; captured table backings must remain alive.
struct SpawnBody48 {State* state;std::uintptr_t character;const std::int32_t(*animation_rows)[40];std::uintptr_t visual;std::uint32_t animation_count,fade_word,reserved[2];};
enum SpawnBodyService:std::uint32_t {spawn_debug_load,spawn_debug_query,spawn_animation_index,spawn_stance_mask,spawn_stance,spawn_set_animation,spawn_set_target,spawn_sync_last_target,spawn_cancel_sneaking,spawn_fade_in,spawn_init_physical};
struct SpawnBodyRequest32 {std::uint32_t service,argument0,argument1,argument2;std::uintptr_t character,payload;};
struct SpawnBodyServices16 {void* context;int(*invoke)(void*,SpawnBody48*,const SpawnBodyRequest32*,std::uint32_t*);};
static_assert(sizeof(SpawnBody48)==48&&sizeof(SpawnBodyRequest32)==32&&sizeof(SpawnBodyServices16)==16);
// Query0 isTracingCharState,query1 isTracingCSSpawn. Their results are ignored,
// but actual DebugSwitches load/get effects remain required. index is genuine
// GetCharAnimTableId; mask ('AnimStancedAnim','SL__LIST_IPHONE'); stance genuine
// GetAnimStance. animation argument0 is wrapping signed sequence bits. Target
// service calls AI_SetTarget(NULL,false), then sync separate live target call.
// CancelSneaking/InitPhysicalObject require genuine Character implementations.
// Fade payload is live VisualObject,argument0 raw float32 duration. Its actual
// shipping body470ce4 is bx-lr; provider can use the proved empty export below.
//0 callback delivered,nonzero stops at the delivered prefix. All borrowed
// backings/Character binding remain alive through synchronous reentry.
}
extern "C" {
// Focus0,Blur1,Update2,Event3. Current1 required on ENTRY. previous is source
// incoming Focus previous-state ID; other methods ignore it. Event28 payload
// is borrowed NUL string; other event payloads opaque.1complete,-1malformed
// atomic,-2required service failure,-3source invalid row/null string boundary.
// This body does not select a current StateInfo or perform an outer transition.
int dh2_character_spawn_body(dh2::character::SpawnBody48*,std::uint32_t operation,std::int32_t previous,std::uint32_t event,std::uintptr_t payload,const dh2::character::SpawnBodyServices16*);
// Exact original VisualObject::StartFadeIn470ce4 bx-lr; no visual/clock mutation.
int dh2_character_spawn_fade_in_empty();
}
