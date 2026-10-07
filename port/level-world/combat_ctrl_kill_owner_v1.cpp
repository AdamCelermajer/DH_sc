#include "combat_ctrl_kill_owner_v1.hpp"
namespace dh2::character {
CombatCtrlKillOwnerV1::CombatCtrlKillOwnerV1(KillActor56& actor,data::CombatActorState& life,
 KillWorld16& world,KillServices16 services):actor_(actor),life_(life),world_(world),services_(services),published_dead_(actor.dead){}
bool CombatCtrlKillOwnerV1::matches(const data::PropertyView* p,const data::CombatActorState* life)const noexcept{
 return actor_.identity&&p==actor_.properties&&life==&life_&&life_.dead<=1&&
 actor_.dead==life_.dead&&services_.invoke;
}
void CombatCtrlKillOwnerV1::publish(){
 // Kill writes projection.dead before its HP store and first external query.
 // Commit the byte logically to the SAME uint32 life authority before any
 // user/backend callback. Do not reinterpret_cast a byte within that word.
 if(actor_.dead!=published_dead_)life_.dead=actor_.dead;
 else actor_.dead=static_cast<std::uint8_t>(life_.dead);
 published_dead_=actor_.dead;
}
int CombatCtrlKillOwnerV1::invoke(void* raw,KillActor56* actor,const KillRequest56* q,KillResponse16* out){
 auto& owner=*static_cast<CombatCtrlKillOwnerV1*>(raw);
 if(actor!=&owner.actor_||!q||!out||owner.life_.dead>1)return -1;
 owner.publish();
 const auto result=owner.services_.invoke(owner.services_.context,actor,q,out);
 // A genuine source callback may write life (Revive, reentrant world event).
 // Its canonical store remains authoritative if projection was unchanged.
 owner.publish();return result;
}
int CombatCtrlKillOwnerV1::ctrl_kill(KillResult24& out,std::uintptr_t attacker,std::uint32_t force,std::string& error){
 error.clear();if(!matches(actor_.properties,&life_)){error="Required same property/life Kill projection and actual services";return -1;}
 if(delivering_){
  // Actual reentry after published source death is the literal outer dead
  // gate. Before that store, do not fabricate an accepted recursive Kill.
  if(life_.dead){out={};out.phase=kill_is_dead+1;out.status=1;return 1;}
  error="Reentrant Ctrl_Kill before source dead publication requires source continuation";return -2;
 }
 struct Scope {bool& value;~Scope(){value=false;}} scope{delivering_};delivering_=true;
 const KillServices16 wrapper{this,invoke};const auto status=dh2_character_ctrl_kill(&out,&actor_,attacker,force,&world_,&wrapper);
 publish();if(status!=1)error="Required whole Ctrl_Kill provider at source service "+std::to_string(out.phase?out.phase-1:0)+"; prefix retained";
 return status;
}
namespace {
bool valid(const data::CombatActorState* s){return s&&s->dead<=1&&s->low_health_armed<=1&&s->combo_hits<=65535&&s->push_death<=1;}
std::int32_t whole(std::int32_t n){return n>=0?n/256:std::int32_t((std::int64_t(n)-255)/256);}
}
int combat_apply_ctrl_kill_v1(CombatCtrlKillResultV1& out,const data::MonsterApplicationRequest& q,
 std::uintptr_t attacker_id,CombatCtrlKillOwnerV1& owner,bool player,bool idle,std::string& error){
 using namespace data;error.clear();
 if(!attacker_id||!q.result||(q.result->mask&0x400000u)||!valid(q.attacker_state)||!valid(q.defender_state)||
 dh2_property_validate(q.attacker)||dh2_property_validate(q.defender)||!owner.matches(q.defender,q.defender_state)){
  error="Required bounded same actor/property/life application domain";return -1;}
 out={};auto& app=out.application;auto& result=*q.result;auto& as=*q.attacker_state;auto& ds=*q.defender_state;
 app.health={0,q.defender->resolved[36],q.defender->resolved[36],0,ds.low_health_armed,0,-1,0};
 as.combo_hits=(result.outcomes&3)?0:((as.combo_hits+1)&65535u);
 if(result.amount>0){
  volatile float per_damage=float(q.attacker->resolved[204])*.00390625f;
  volatile float damage=float(result.amount)*.00390625f;app.threat=per_damage*damage;
  ds.push_death=(result.outcomes&128)?((result.mask>>20)&1):0;
  HealthRequest hit{q.defender,std::uint32_t(result.amount),(player?health_player:health_monster)|health_game_present|health_main_player_present|(ds.dead?health_dead:0u),0,ds.low_health_armed};
  if(dh2_health_hit(&app.health,&hit)){error="Source health prefix failed";return -2;}
  app.hit_called=1;ds.low_health_armed=app.health.low_health_armed;
  if(app.health.kill_requested){
   out.kill_reached=true;
   const auto status=owner.ctrl_kill(out.kill,attacker_id,0,error);
   if(status!=1)return status; // BEFORE lifecycle, leech and common tails.
  }
  if(app.health.lifecycle_write!=-1)ds.lifecycle=app.health.lifecycle_write;
  if(ds.dead)result.outcomes&=~0x160u;
 }
 out.leech_reached=true;
 dh2_vitals_regen(q.attacker,0,result.hp_leech,&app.hp_leech);
 dh2_vitals_regen(q.attacker,1,result.mp_leech,&app.mp_leech);
 if(!ds.dead){
  if(player&&idle&&!(result.outcomes&0x16u))result.outcomes|=result.amount>0?16u:2u;
  app.special=bool(result.mask&0x18000000u);app.push=(result.mask>>20)&1;
  if(result.dot_duration>0&&result.dot_amount>0){app.status_requests|=request_dot;app.dot_duration=whole(result.dot_duration);app.dot_amount=result.dot_amount;app.dot_element=result.dot_element;}
  if(result.outcomes&2){app.status_requests|=request_dodge;if(player)dh2_property_add(q.defender,215,256);}
  if(result.outcomes&4){app.status_requests|=request_block;if(player)dh2_property_add(q.defender,214,256);}
  if(result.outcomes&16)app.status_requests|=request_hurt;
  if(result.outcomes&128){app.status_requests|=request_push;if(player)dh2_property_add(q.defender,222,256);}
  if(result.outcomes&64){app.status_requests|=request_stun;app.stun_duration=whole(q.attacker->resolved[result.mask&4096?185:140]);}
  if(result.outcomes&32){app.fear_duration=whole(q.attacker->resolved[result.mask&16384?187:143]);if(app.fear_duration)app.status_requests|=request_fear;}
  if(result.outcomes&256){app.status_requests|=request_slow;app.slow_duration=whole(q.attacker->resolved[result.mask&65536?189:146]);}
 }
 out.complete=true;return 1;
}
}
