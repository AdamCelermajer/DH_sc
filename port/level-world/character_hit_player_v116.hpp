#pragma once
#include "character_skill_combat_v6.hpp"
#include "character_hit_player_tail_v7.hpp"
namespace dh2::character::skills {
enum HitPlayerSourceServiceV116:unsigned {hit_master_v116=17};
struct CharacterHitPlayerServicesV116 {
 const HitPlayerTailBorrowV7* actor{};
 const HitPlayerTailServicesV7* services{};
};
//Whole offline source HitFor with both actual Player and ordinary Character
//receivers. Source common/trophy order is unchanged; positive player calls
//require its real source tail, including tutorial/settings/audio ownership.
int character_hit_for_player_v116(HitResult40*,HitActor32*,std::uint32_t,
 const HitAttacker24*,const HitServices16*,const CharacterHitPlayerServicesV116*);
}
