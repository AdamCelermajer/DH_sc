#include "character_dot_player_reaction_v7.hpp"
namespace dh2::character {
int dot_player_reaction_v7(data::CombatResult* attack,std::uintptr_t defender,
 const DotPlayerReactionServicesV7* services) {
 if(!attack||!defender||!services)return -1;
 if(!services->is_idle)return -2;
 bool idle{};
 if(services->is_idle(services->context,defender,&idle)!=0)return -2;
 if(idle&&(attack->outcomes&0x16u)==0)
  attack->outcomes|=attack->amount>0?0x10u:2u;
 return 1;
}
}
