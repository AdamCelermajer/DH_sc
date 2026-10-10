#include "status_effects.hpp"
namespace dh::foundation::status {
using namespace dh2::character;
bool StatusEffects::valid(std::string& e)const{
 if(!b_.actor||b_.actor->id==invalid_actor_id||!b_.character||!b_.properties||dh2_property_validate(b_.properties)||!b_.buffs||!b_.timers||b_.timers->owner!=b_.character){e="Required SAME ActorState/character/properties/buffs/timer owner";return false;}
 if(b_.effects&&(!b_.effects->fsm||b_.effects->fsm->character!=b_.character)){e="Status effects require SAME original FSM character";return false;}
 if(b_.ticking&&(b_.ticking->owner!=b_.character||b_.ticking->properties!=b_.properties)){e="Status ticks require SAME original property/character owner";return false;}
 if(b_.slow.properties&&(b_.slow.properties!=b_.properties||b_.slow.buffs!=b_.buffs)){e="Slow requires SAME original property/buff owner";return false;}
 return true;
}
bool StatusEffects::enter(){if(busy_){error_="Synchronous status adapter reentry rejected";return false;}if(!valid(error_))return false;busy_=true;error_.clear();return true;}
int StatusEffects::add(BuffResult24& r,std::int32_t id,std::uint32_t duration,std::int32_t capacity,std::uint32_t strength,std::int32_t fx,const char* name){if(!enter())return -1;Scope s{*this};return dh2_character_buff_add(&r,b_.buffs,id,duration,capacity,strength,fx,name);}
int StatusEffects::remove(BuffResult24& r,std::int32_t id,std::uintptr_t instance){if(!enter())return -1;Scope s{*this};return dh2_character_buff_delete(&r,b_.buffs,id,instance);}
int StatusEffects::dot(BuffResult24& r,std::int32_t duration,std::int32_t amount,std::int32_t element){if(!enter())return -1;Scope s{*this};if(!b_.dot_ids||!b_.dot_fx_ids){error_="Required original DoT class/FX dictionaries";return -2;}return dh2_character_buff_add_dot(&r,b_.buffs,duration,amount,element,b_.dot_ids,b_.dot_fx_ids);}
int StatusEffects::expired(BuffResult24& r,const Timer32* timer){if(!enter())return -1;Scope s{*this};if(!timer){error_="Required actual timer expiry object";return -1;}bool same=false;for(unsigned i=0;i<b_.timers->count;++i)if(timer==&b_.timers->slots[i])same=true;if(!same){error_="Expiry object must belong to SAME TimerStore";return -1;}return dh2_character_buff_expired(&r,b_.buffs,timer);}
int StatusEffects::tick(TimerEffectResult24& r,std::uint32_t event){if(!enter())return -1;Scope s{*this};if(!b_.ticking||!b_.tick_services||!b_.tick_services->invoke){error_="Required original timer-effect CalculateResult/full F_ApplyResult services";return -2;}return dh2_character_timer_effect(&r,b_.ticking,event,b_.tick_services);}
int StatusEffects::application(const skills::SkillApplyRequestV6& q,skills::SkillApplyResponseV6& r){
 using namespace skills;if(q.service!=skill_apply_stun_v6&&q.service!=skill_apply_scare_v6&&q.service!=skill_apply_slow_v6)return 0;
 if(!enter())return -1;Scope s{*this};if(q.subject!=b_.character||q.target!=b_.character){error_="Status request must target SAME borrowed character";return -1;}
 int status;
 if(q.service==skill_apply_slow_v6){if(!b_.slow.properties){error_="Required source slow tables/owner";return -2;}BuffResult24 out{};status=character_slow_reaction_v1(b_.slow,q.word,out,error_);}
 else{if(!b_.effects||!b_.effect_services||!b_.effect_services->invoke){error_="Required original native stun/scare services";return -2;}status=dh2_character_native_effect_set(b_.effects,q.service==skill_apply_scare_v6,q.word,1,q.attacker,q.service==skill_apply_scare_v6?q.flags:0,b_.effect_services);}
 if(status<0)return status;r={};return 1;
}
int StatusEffects::remove_all(BuffResult24& r){if(!enter())return -1;Scope s{*this};return dh2_character_buffs_remove_all(&r,b_.buffs);}
}
