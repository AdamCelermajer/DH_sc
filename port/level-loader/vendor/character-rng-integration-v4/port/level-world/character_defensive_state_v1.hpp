#pragma once
#include "player_injure_state_v7.hpp"
namespace dh2::character {
enum class DefensiveAnimationV1 : unsigned { blocking, dodging };
struct DefensiveStateServicesV1 {
 const PlayerInjureServicesV7* common{};
 int(*animation)(void*,int,DefensiveAnimationV1,bool*,int*){};
};
// Whole original SM_SetBlockingState3c5c60/SM_SetDodgingState3c5b3c.
int character_defensive_state_v1(const PlayerInjureBorrowV7*,std::uintptr_t,
 bool,DefensiveAnimationV1,const DefensiveStateServicesV1*);
}
