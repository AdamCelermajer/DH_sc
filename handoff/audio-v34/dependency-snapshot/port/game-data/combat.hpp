#pragma once
#include <cstdint>
namespace dh2::data {
// New native ABI. Combat consumes resolved 224-property sheets and supplied
// equipment facts; inventory/equipment producers remain separate modules.
struct CombatantView {
 const std::int32_t* properties;
 std::int32_t main_damage_class=-1,off_damage_class=-1;
 std::uint32_t two_hander=0,dual_wield=0,shield=0;
 std::int32_t state=-1;
 std::uint32_t combo_hits=0;
};
struct CombatRandom {std::uint32_t seed=1,calls=0;};
struct Damage {std::int32_t amount=0,element=0,hp_leech=0,mp_leech=0,dot_duration=0,dot_amount=0,dot_element=0;};
struct DamageRequest {
 const CombatantView *attacker,*defender;
 CombatRandom* random;
 std::int32_t direct_amount,type,element;
 // bit 0 off-hand, 1 magic, 2 blocked, 3 critical; higher bits rejected.
 std::uint32_t flags;
};
}
extern "C" {
std::int32_t dh2_combat_random(dh2::data::CombatRandom*,std::int32_t range);
unsigned dh2_combat_bonus(const dh2::data::CombatantView*,unsigned offhand,std::int32_t* result);
unsigned dh2_combat_dot(dh2::data::Damage*,const dh2::data::CombatantView*,const dh2::data::CombatantView*,dh2::data::CombatRandom*,unsigned magic);
unsigned dh2_combat_damage(dh2::data::Damage*,const dh2::data::DamageRequest*);
}
