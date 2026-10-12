#pragma once
#include "../game-data/combat_result.hpp"
#include "../game-data/properties.hpp"
#include <cstdint>
namespace dh2::character {
// Projection of one retained Character lifetime. Byte/halfword source fields
// stay live through synchronous callbacks; identity/property backing is stable.
struct DotActor32 {
 std::uintptr_t identity;
 data::PropertyView* properties;
 std::int32_t network_id;
 std::uint16_t combo_hits;
 std::uint8_t push_death,god;
 std::uint64_t reserved;
};
// CF_SetCombatants' shared source words +1c..33, projected to native pointers.
// Caller owns this shared context; no private replacement combat singleton.
struct DotCombatContext32 {
 std::uintptr_t attacker,defender;
 std::int32_t level_delta,reverse_level_delta,element;
 std::uint8_t offhand,magic,blocked,critical;
};
enum DotService : std::uint32_t {
 dot_debug_load=0,dot_debug_query,dot_profile_begin,dot_profile_end,
 dot_online,dot_application_switch,dot_party_count,dot_is_player,
 dot_add_aggro,dot_hit_for,dot_is_dead,dot_regen_hp,dot_regen_mp,
 dot_cancel_sneaking,dot_combat_text,dot_combat_sound
};
struct DotRequest40 {
 std::uint32_t service;std::int32_t amount;
 std::uintptr_t subject,target;
 const char* name;
 float threat;std::uint32_t reserved;
};
struct DotResponse8 {std::int32_t word;float number;};
struct DotServices16 {
 void* context;
 // 0 genuine synchronous delivery, nonzero failure. Queries write their
 // actual result. HitFor includes required kill/AI/FX services; never accept
 // a health-only projection as the full Character method. Text/sound/sneak
 // are required even for self/self DoT. Fixtures must be labelled explicitly.
 int(*invoke)(void*,DotActor32*,const DotRequest40*,DotResponse8*,data::CombatResult*);
};
struct DotResult24 {
 std::uint32_t phase,calls,hit_called,reserved;
 float threat;std::int32_t status;
};
static_assert(sizeof(DotActor32)==32&&sizeof(DotCombatContext32)==32);
static_assert(sizeof(DotRequest40)==40&&sizeof(DotResponse8)==8&&sizeof(DotServices16)==16&&sizeof(DotResult24)==24);
}
extern "C" {
// F_DotAttack -> complete direct-damage CalculateResult branch, self/self.
// Real CombatResult/Damage kernels run; no rate, duration or dt scaling.
// Profiles/debug order and source combat-context writes are preserved.
int dh2_character_dot_calculate(dh2::character::DotResult24*,dh2::data::CombatResult*,
 dh2::character::DotCombatContext32*,dh2::character::DotActor32*,std::int32_t amount,
 std::int32_t element,const dh2::character::DotServices16*);
// Entry at _F_CalculateResult, for TimerEffect.effect_dot_calculate: that
// existing caller has ALREADY delivered the F_DotAttack outer debug pair.
int dh2_character_dot_calculate_result(dh2::character::DotResult24*,dh2::data::CombatResult*,
 dh2::character::DotCombatContext32*,dh2::character::DotActor32*,std::int32_t amount,
 std::int32_t element,const dh2::character::DotServices16*);
// F_ApplyResult's self/self DoT orchestration: offline/nonplayer, party<=1.
// TimerEffect may supply a DIFFERENT retained actor after its source owner
// reread; this API does not bind application to the prior calculation owner.
// Any network_id is retained: source HitFor runs ONLY when network_id==-1.
// Queries stay live. Online/player/co-op branches return -3 at their reached
// boundary with source prefixes intact; they are not silently accepted.
// Input result must be an unmodified result of the calculation branch above.
// 1 complete, -1 malformed atomic, -2 required service failure, -3 unsupported.
// Result/actor/context/services/output must be disjoint; identities, backing
// sheets and receiver lifetimes survive callbacks. No callback closes owner.
// During calculation, callbacks do not mutate the result/shared context;
// live actor byte/halfword fields and property values may change. Projection
// buffers are explicit source snapshots, not a new Character/AI lifetime.
int dh2_character_dot_apply(dh2::character::DotResult24*,dh2::data::CombatResult*,
 dh2::character::DotActor32*,const dh2::character::DotServices16*);
}
