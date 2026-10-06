#pragma once
#include "combat.hpp"
namespace dh2::data {
// Original AttackResult word order retained in a new native interface.
struct CombatResult {
 std::int32_t amount=-1,dot_element=-1,dot_duration=-1,dot_amount=-1;
 std::int32_t hp_leech=0,mp_leech=0;
 // miss, dodge, block, critical, hurt, fear, stun, push, slow (bits 0..8).
 std::uint32_t outcomes=0,mask=0;
 std::int32_t weapon_category=-1,element=-1;
};
struct CombatResultRequest {
 const CombatantView *attacker,*defender;
 CombatRandom* random;
 std::uint32_t mask;
 std::int32_t weapon_category,element,direct_amount;
};
}
// Reconstructs _F_CalculateResult including hit/status rolls and RNG order.
// Applying results to actor properties, AI and FX/audio remains separate.
extern "C" unsigned dh2_combat_result(dh2::data::CombatResult*,const dh2::data::CombatResultRequest*);
// F_MeleeAttack request construction with supplied equipment facts. The
// alternate flag preserves the original second bool's mask choice. Its buff
// removal side effect (when alternate is set) is a separate lifecycle service.
extern "C" unsigned dh2_combat_melee(dh2::data::CombatResult*,const dh2::data::CombatantView*,const dh2::data::CombatantView*,dh2::data::CombatRandom*,unsigned offhand,unsigned alternate);
