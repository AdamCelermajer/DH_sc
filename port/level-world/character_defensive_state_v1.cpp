#include "character_defensive_state_v1.hpp"
#include <cstring>
namespace dh2::character {
int character_defensive_state_v1(const PlayerInjureBorrowV7* b,std::uintptr_t attacker,
 bool direct,DefensiveAnimationV1 kind,const DefensiveStateServicesV1* services){
 if(!b||!b->character||!b->state||!b->source_gate_14fc||!services||!services->common||
    unsigned(kind)>1)return -1;
 const auto& s=*services->common;
 if(*b->source_gate_14fc>0.f)return 1;
 bool player{};
 if(!s.is_player||s.is_player(s.context,b->character,&player))return -2;
 // The source calls virtual IsPlayer but deliberately ignores its answer.
 *b->source_gate_14fc=3000.f;
 int table{};
 if(!s.animation_table||s.animation_table(s.context,b->character,&table))return -2;
 if(table<0)return 1;
 bool found{};int animation{};
 if(!services->animation||services->animation(s.context,table,kind,&found,&animation))return -2;
 if(!found||animation==-1)return 1;
 int stanced{};
 if(!s.constant||s.constant(s.context,"AnimStancedAnim","SL__LIST_IPHONE",&stanced))return -2;
 int offset=0;
 if(stanced&(kind==DefensiveAnimationV1::blocking?0x2000:0x4000)){
  if(!s.stance||s.stance(s.context,b->character,&offset))return -2;
 }
 const std::uint32_t word=std::uint32_t(animation)+std::uint32_t(offset);
 std::memcpy(&b->state->animation_override,&word,4);
 if(direct){if(!s.transition||s.transition(s.context,11,50010,attacker))return -2;}
 else if(!s.event||s.event(s.context,50010,attacker))return -2;
 return 1;
}
}
