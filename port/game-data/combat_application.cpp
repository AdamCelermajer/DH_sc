#include "combat_application.hpp"
namespace {
bool valid(const dh2::data::CombatActorState* state){return state&&state->dead<=1&&state->low_health_armed<=1&&state->combo_hits<=65535&&state->push_death<=1;}
std::int32_t whole(std::int32_t n){return n>=0?n/256:std::int32_t((std::int64_t(n)-255)/256);}
unsigned apply(dh2::data::MonsterApplication* out,const dh2::data::MonsterApplicationRequest* request,bool player_defender,bool player_idle){
 using namespace dh2::data;
 if(!out||!request||!request->result||request->result->mask&0x400000u||!valid(request->attacker_state)||!valid(request->defender_state)||dh2_property_validate(request->attacker)||dh2_property_validate(request->defender))return 1;
 auto& result=*request->result;auto& attacker=*request->attacker;auto& defender=*request->defender;auto& as=*request->attacker_state;auto& ds=*request->defender_state;
 MonsterApplication next{};next.health={0,defender.resolved[36],defender.resolved[36],0,ds.low_health_armed,0,-1,0};
 as.combo_hits=(result.outcomes&3)?0:((as.combo_hits+1)&65535u);
 if(result.amount>0){
  volatile float per_damage=float(attacker.resolved[204])*.00390625f;
  volatile float damage=float(result.amount)*.00390625f;next.threat=per_damage*damage;
  ds.push_death=(result.outcomes&128)?((result.mask>>20)&1):0;
  HealthRequest hit{&defender,std::uint32_t(result.amount),(player_defender?health_player:health_monster)|health_game_present|health_main_player_present|(ds.dead?health_dead:0u),0,ds.low_health_armed};
  dh2_health_hit(&next.health,&hit);next.hit_called=1;ds.low_health_armed=next.health.low_health_armed;
  if(next.health.kill_requested&&!ds.dead){
   // Ctrl_Kill -> Kill first sets IsDead, then writes HP zero, before the
   // remaining reward/AI/script services and RaiseEvent(2).
   ds.dead=1;dh2_property_set(&defender,36,0);
  }
  if(next.health.lifecycle_write!=-1)ds.lifecycle=next.health.lifecycle_write;
  if(ds.dead)result.outcomes&=~0x160u;
 }
 dh2_vitals_regen(&attacker,0,result.hp_leech,&next.hp_leech);
 dh2_vitals_regen(&attacker,1,result.mp_leech,&next.mp_leech);
 if(!ds.dead){
  // Player F_ApplyResult checks SM_IsIdle(false). An otherwise unclassified
  // idle hit becomes dodge for nonpositive damage, hurt for positive damage.
  if(player_defender&&player_idle&&!(result.outcomes&0x16u))result.outcomes|=result.amount>0?16u:2u;
  next.special=bool(result.mask&0x18000000u);next.push=(result.mask>>20)&1;
  if(result.dot_duration>0&&result.dot_amount>0){next.status_requests|=request_dot;next.dot_duration=whole(result.dot_duration);next.dot_amount=result.dot_amount;next.dot_element=result.dot_element;}
  if(result.outcomes&2){next.status_requests|=request_dodge;if(player_defender)dh2_property_add(&defender,215,256);}
  if(result.outcomes&4){next.status_requests|=request_block;if(player_defender)dh2_property_add(&defender,214,256);}
  if(result.outcomes&16)next.status_requests|=request_hurt;
  if(result.outcomes&128){next.status_requests|=request_push;if(player_defender)dh2_property_add(&defender,222,256);}
  if(result.outcomes&64){next.status_requests|=request_stun;next.stun_duration=whole(attacker.resolved[result.mask&4096?185:140]);}
  if(result.outcomes&32){next.fear_duration=whole(attacker.resolved[result.mask&16384?187:143]);if(next.fear_duration)next.status_requests|=request_fear;}
  if(result.outcomes&256){next.status_requests|=request_slow;next.slow_duration=whole(attacker.resolved[result.mask&65536?189:146]);}
 }
 *out=next;return 0;
}
}
extern "C" unsigned dh2_combat_apply_monster(dh2::data::MonsterApplication* out,const dh2::data::MonsterApplicationRequest* request){return apply(out,request,false,false);}
extern "C" unsigned dh2_combat_apply_player_to_monster(dh2::data::MonsterApplication* out,const dh2::data::MonsterApplicationRequest* request){
 // Player-attacker branches after these common mutations notify achievements;
 // the critical branch invokes a skill only when its manager/skill is present.
 // This entry point has the explicit empty-skill/external-service contract.
 return dh2_combat_apply_monster(out,request);
}
extern "C" unsigned dh2_combat_apply_monster_to_player(dh2::data::MonsterApplication* out,const dh2::data::MonsterApplicationRequest* request,unsigned player_idle){
 if(player_idle>1)return 1;
 return apply(out,request,true,player_idle!=0);
}
