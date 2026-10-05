#include "player_timer_helpers_v1.hpp"
namespace dh2::android_ui {
int player_timer_helper_v1(const model_renderer::PlayerGameplayBinding& p,unsigned key,const PlayerTimerServicesV1& s,std::string& e){
 if(!p.active||!p.character||!p.state||!p.properties||!p.properties->resolved||!p.life||!p.skills||p.skills->session().property_view().resolved!=p.properties->resolved){e="Same player timer-helper authority unavailable";return -1;}
 if(key==0x3df3f0){
  // Source uses freshly read resolved raw property126..131, element-1..4.
  // Each positive value freshly consults same Character IsDead before hit.
  for(int element=-1;element<5;++element){const auto amount=p.properties->resolved[127+element];
   if(amount<=0||p.life->dead)continue;
   if(!s.dot||s.dot(s.context,p.character,amount,element)){e="Required same-player F_DotAttack/ApplyResult failed at element "+std::to_string(element);return -2;}
  }
  return 0;
 }
 if(key!=0x3cb77c){e="Unknown original player timer helper";return -1;}
 bool remote=false;if(!s.remote_updated||s.remote_updated(s.context,p.character,&remote)){e="Actual IsRemotelyUpdated producer unavailable";return -2;}
 if(remote)return 0;
 // AI_IsInCombat source ordered HasAggro(+8c), IsAggroed(+a4), then
 // Attack5,Skill6,Cast7. These are source aggro containers, not target pointers.
 bool current=false,last=false;
 if(!s.aggro||s.aggro(s.context,p.character,&current,&last)){e="Actual AI aggro containers unavailable";return -2;}
 const bool combat=current||last||p.state->current==5||p.state->current==6||p.state->current==7;
 const auto hp=p.properties->resolved[combat?40:39];
 if(!s.debug||dh2::character::skills::dh2_character_skill_regen_v6(p.properties,0,hp,s.debug)){e="Original HP RegenTick failed";return -2;}
 // Source reloads MP raw amount after the completed HP call.
 const auto mp=p.properties->resolved[combat?45:44];
 if(dh2::character::skills::dh2_character_skill_regen_v6(p.properties,1,mp,s.debug)){e="Original MP RegenTick failed after HP";return -2;}
 return 0;
}
}
