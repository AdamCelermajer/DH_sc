#pragma once
#include "../game-data/properties.hpp"
#include "../game-data/combat_result.hpp"
#include <array>
namespace dh2::character {
// Original cached sheet at CharProperties+0xa94 is PropertyView.resolved;
// DoT properties126..131 share that existing sheet, not a second buffer.
struct TimerEffectState32 {
 std::uintptr_t owner;
 data::PropertyView* properties;
 std::uintptr_t has_aggro,aggroed; // CharAI+8c,+a4 nonzero predicates.
};
enum TimerEffectService : std::uint32_t {
 effect_remote_update=0,effect_current_state,effect_is_dead,effect_debug_load,
 effect_debug_query,effect_dot_calculate,effect_dot_apply
};
struct TimerEffectRequest40 {
 std::uint32_t service,property;std::int32_t amount,element;
 std::uintptr_t subject,target;const char* name;
};
struct TimerEffectServices16 {
 void* context;
 // Delivery0, failure nonzero. Queries return real signed word through out.
 // Calculate must write ALL CombatResult fields as actual CalculateResult;
 // request is self/self, mask0x20080000, category-1, element and direct amount.
 // Apply is actual F_ApplyResult(result,self,self,false), never fake accepted.
 // Receivers/backings survive calls. Mutating source words/owner is permitted
 // but retained property backing/pointer fields must remain stable during the
 // synchronous run. Source cached/base/saved/gear sheets are inline storage.
 int(*invoke)(void*,TimerEffectState32*,const TimerEffectRequest40*,std::int32_t*,data::CombatResult*);
};
struct TimerEffectResult24 {std::uint32_t phase,calls,regen_adds,dot_attacks;std::int32_t hp_add,mp_add;};
struct TimerOwner8 {std::int32_t network_id;std::uint8_t remote_update,dead;std::uint16_t reserved;};
static_assert(sizeof(TimerEffectState32)==32&&sizeof(TimerEffectRequest40)==40&&sizeof(TimerEffectServices16)==16&&sizeof(TimerEffectResult24)==24);
}
extern "C" {
// Event33 full UpdateRegen + source AI_IsInCombat + RegenTick/HP/MP arithmetic;
// event34 source HandleDots + F_DotAttack debug/calculation request producer.
// No dt scaling or design tick lookup is performed by these expiry bodies.
// 1 complete,-1 malformed atomic,-2 service/property failure after prefixes.
int dh2_character_timer_effect(dh2::character::TimerEffectResult24*,dh2::character::TimerEffectState32*,std::uint32_t event,const dh2::character::TimerEffectServices16*);
// Exact verified Character v54/v34 implementations from source owner fields.
// kind0 ObjectBase.IsRemotelyUpdated; kind1 Character.IsDead. Borrowed fields
// must come from the genuine live ObjectBase/Character lifetime producers.
int dh2_character_timer_owner_query(std::int32_t*,const dh2::character::TimerOwner8*,std::uint32_t kind);
// Exact cached-sheet reset tail of ResetAllProperties, independently of
// remaining base/saved/gear reset and buff registry. Defaults/output disjoint.
int dh2_character_cached_reset(dh2::data::PropertyView*);
// Original PROPS_RemoveDot3de83c is bx lr for every argument. This does NOT
// delete a timed buff; PROPS_DelBuff/BuffExpired remain a distinct producer.
int dh2_character_dot_remove(dh2::data::PropertyView*,std::int32_t element);
}
