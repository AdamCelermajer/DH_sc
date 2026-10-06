#pragma once
#include "character_state_owner.hpp"
namespace dh2::character {
// Four retained words of the original160-byte CharAnim row, source+10..1c.
struct DeadAnimationRow16 {std::int32_t deadly_great_kb,despawn,despawn_great_kb,died;};
// pending_alternate projects machine+3f, distinct from State.dead_alternate
// (machine+3e). secondary_animation projects machine+38. Borrowed rows remain
// alive through synchronous callbacks, even when the global row pointer changes.
struct DeadSelect32 {StateOwnerMachine40* machine;const DeadAnimationRow16* rows;
 std::uint32_t count,pending_alternate;std::int32_t secondary_animation;std::uint32_t reserved;};
enum DeadSelectOperation:std::uint32_t {dead_animation_index,dead_stance_bits,dead_anim_stance};
struct DeadSelectRequest24 {std::uint32_t operation,mask;std::uintptr_t character;std::uint32_t reserved[2];};
struct DeadSelectServices16 {void* context;int(*invoke)(void*,DeadSelect32*,const DeadSelectRequest24*,std::uint32_t*);};
// index returns authored Character+1000 bits; recovered GetCharAnimTableId
// falls back17 for invalid indices. Constants query exact
// ('AnimStancedAnim','SL__LIST_IPHONE'); mask identifies this source branch.
// Stance returns signed source GetAnimStance bits. Zero provider status means
// delivered; missing/failed services stop at their actual prefix with-2.
// Callbacks may reenter and update fields, but cannot destroy borrowed objects
// or replace machine/FSM/State bindings while a call is in progress.
static_assert(sizeof(DeadAnimationRow16)==16&&sizeof(DeadSelect32)==32);
static_assert(sizeof(DeadSelectRequest24)==24&&sizeof(DeadSelectServices16)==16);
}
extern "C" {
//1 dispatched (including an ignored source event),0 invalid table after fallback,
//-1 malformed pre-mutation,-2 missing service; deeper owner diagnostics propagate.
// mode retains its lowbyte and force uses nonzero, matching original registers.
int dh2_character_dead_select(dh2::character::DeadSelect32*,std::uint32_t mode,
 std::uintptr_t payload,std::uint32_t force,const dh2::character::DeadSelectServices16*,
 const dh2::character::StateOwnerServices16*);
}
