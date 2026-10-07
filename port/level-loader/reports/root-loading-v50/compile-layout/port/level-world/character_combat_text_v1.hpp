#pragma once
#include "../game-data/combat_result.hpp"
#include <cstdint>
namespace dh2::character::skills {
// Whole Character F_ApplyScrollingCombatText(AttackResult,Character,Character)
// 3af77c. Queue callback must retain the original FlashAnimManager owner.
struct CombatTextRequestV1 {
 const char* style{};float position[3]{};const char* text{};
 std::int32_t number{},color{};bool numeric{};
};
struct CombatTextServicesV1 {
 void* context{};
 int(*follower)(void*,std::uintptr_t,bool*){};
 int(*position)(void*,std::uintptr_t,float[3]){};
 int(*height)(void*,std::uintptr_t,float*){}; // actual bounds+158 minus+14c
 int(*property)(void*,std::uintptr_t,int,std::int32_t*){};
 int(*dual_wield)(void*,std::uintptr_t,bool*){};
 int(*is_player)(void*,std::uintptr_t,bool*){};
 int(*constant)(void*,const char* group,const char* key,std::int32_t*){};
 int(*localized)(void*,std::int32_t,const char**){}; // same retained StringManager
 int(*enqueue)(void*,const CombatTextRequestV1*){};
};
//1 complete (includes genuine suppressed/drop branches),negative required
// producer failure. Reached queue entries persist when a later query fails.
int character_combat_text_v1(const data::CombatResult&,std::uintptr_t attacker,
 std::uintptr_t target,const CombatTextServicesV1&);
}
