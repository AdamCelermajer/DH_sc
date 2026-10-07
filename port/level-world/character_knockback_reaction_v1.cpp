#include "character_knockback_reaction_v1.hpp"
#include <cstring>
namespace dh2::character {namespace {
bool valid(const KnockbackReactionBorrowV1& b){return b.fsm&&b.fsm->state&&b.fsm->character&&b.properties&&!dh2_property_validate(b.properties);}
int add(int a,int b){std::uint32_t u=std::uint32_t(a)+std::uint32_t(b);int out;std::memcpy(&out,&u,4);return out;}
}
int character_set_knockback_v1(const KnockbackReactionBorrowV1& b,bool great,std::uintptr_t attacker,bool direct,const KnockbackReactionServicesV1& s){
 if(!valid(b)||!b.ai||!b.animations)return -1;
 const auto* ai=data::ai_props(*b.ai,b.properties->resolved[1]);if(!ai)return -2;if(ai->flags&4)return 1;
 int table=b.properties->resolved[2];if(table<0||std::size_t(table)>=b.animations->characters.size())table=17;
 if(table<0||std::size_t(table)>=b.animations->characters.size())return 1;
 const auto& row=b.animations->characters[table].fields[great?8:16];if(row.size()!=1)return -2;
 int mask{},stance=0;if(!s.constant||s.constant(s.context,"AnimStancedAnim","SL__LIST_IPHONE",&mask))return -2;
 if(mask&(great?0x800:0x400)){if(!s.stance||s.stance(s.context,b.fsm->character,&stance))return -2;}
 auto& state=*b.fsm->state;state.animation_override=add(row.front(),stance);
 if(great)state.attack_gate=0x18;else state.attack_gate&=~0x18u;
 const auto status=direct?(s.transition?s.transition(s.context,10,0xc35b,attacker):-1):(s.event?s.event(s.context,0xc35b,attacker):-1);
 return status? -2:1;
}
int character_knockback_focus_v1(const KnockbackReactionBorrowV1& b,std::uintptr_t attacker,const KnockbackReactionServicesV1& s){
 if(!valid(b))return -1;auto& state=*b.fsm->state;
 if(!s.debug||s.debug(s.context,"isTracingCharState")||s.debug(s.context,"isTracingCSKnockedBack"))return -2;
 state.flags=0x2341;if(!s.animation||s.animation(s.context,-1))return -2;
 if((state.attack_gate&8)&&state.body_present){if(!s.filter||s.filter(s.context,0,0x51c,3,false))return -2;}
 if(state.attack_gate&0x10)state.attack_gate=0x20;else state.attack_gate&=~0x20u;
 state.controller_locked=1;
 if(!s.look_at||s.look_at(s.context,attacker)||!s.cancel_sneaking||s.cancel_sneaking(s.context))return -2;
 if(state.body_present&&(!s.unpin||s.unpin(s.context)))return -2;return 1;
}
int character_knockback_blur_v1(const KnockbackReactionBorrowV1& b,const KnockbackReactionServicesV1& s){
 if(!valid(b))return -1;auto& state=*b.fsm->state;if(!s.debug||s.debug(s.context,"isTracingCharState"))return -2;
 state.controller_locked=0;
 if(state.body_present){if(!s.reset_filter||s.reset_filter(s.context))return -2;if(state.body_present&&(!s.pin||s.pin(s.context)))return -2;}return 1;
}
int character_knockback_event_v1(const KnockbackReactionBorrowV1& b,std::uint32_t event,const KnockbackReactionServicesV1& s){
 if(!valid(b))return -1;
 if(event==0x27){if(b.fsm->state->body_present&&(!s.reset_filter||s.reset_filter(s.context)))return -2;}
 else if(event==0x23){bool dead{};if(!s.is_dead||s.is_dead(s.context,&dead))return -2;if(dead){if(!s.stop_animation||s.stop_animation(s.context)||!s.set_dead||s.set_dead(s.context,true,0,true))return -2;}}
 return 1;
}
}
