#include "combat.hpp"
#include <algorithm>
#include <cstring>
namespace {
using namespace dh2::data;
std::int32_t signed_word(std::uint32_t raw){std::int32_t value;std::memcpy(&value,&raw,4);return value;}
std::int32_t add(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)+std::uint32_t(b));}
std::int32_t sub(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)-std::uint32_t(b));}
std::int32_t mul(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)*std::uint32_t(b));}
std::int32_t shr(std::int32_t value,unsigned bits){const auto raw=std::uint32_t(value);return signed_word((raw>>bits)|(value<0?(~0u<<(32-bits)):0u));}
bool valid(const CombatantView* actor){return actor&&actor->properties&&actor->main_damage_class>=-1&&actor->main_damage_class<141&&actor->off_damage_class>=-1&&actor->off_damage_class<141&&actor->two_hander<=1&&actor->dual_wield<=1&&actor->shield<=1&&actor->combo_hits<=65535;}
std::int32_t prop(const CombatantView& actor,std::int32_t index){return index>=0&&index<224?actor.properties[index]:-1;}
std::int32_t bonus(const CombatantView& actor,bool offhand){
 const auto category=offhand?actor.off_damage_class:actor.main_damage_class;if(category==-1)return 0;
 auto value=prop(actor,83+category);if(actor.two_hander)value=add(value,prop(actor,91));if(actor.dual_wield)value=add(value,prop(actor,90));return value;
}
std::int32_t roll(CombatRandom& random,std::int32_t low,std::int32_t high){return add(low,dh2_combat_random(&random,sub(high,low)));}
std::int32_t percent_product(std::int32_t value,std::int32_t percent){return shr(mul(value,percent),8)/100;}
std::int32_t resistance_factor(std::int32_t resistance){return add(1,-(shr(resistance,8)/100));}
void dot(Damage& result,const CombatantView& attacker,const CombatantView& defender,CombatRandom& random,bool magic){
 result.dot_duration=prop(attacker,magic?181:125);result.dot_amount=0;result.dot_element=-1;
 if(result.dot_duration<=0)return;
 result.dot_amount=roll(random,prop(attacker,magic?179:123),prop(attacker,magic?180:124));
 if(magic){result.dot_element=shr(prop(attacker,178),8);if(result.dot_element!=-1)result.dot_amount=std::max(0,sub(result.dot_amount,prop(defender,add(result.dot_element,74))));}
}
}
extern "C" std::int32_t dh2_combat_random(dh2::data::CombatRandom* random,std::int32_t range){
 if(!random)return 0;
 std::int32_t result=0;
 if(range){random->seed=(random->seed*59051u+177149u)%14348907u;result=std::int32_t(random->seed%std::uint32_t(range));}
 ++random->calls;return result;
}
extern "C" unsigned dh2_combat_bonus(const dh2::data::CombatantView* actor,unsigned offhand,std::int32_t* result){
 if(!result||offhand>1||!valid(actor))return 1;
 *result=bonus(*actor,offhand!=0);return 0;
}
extern "C" unsigned dh2_combat_dot(dh2::data::Damage* out,const dh2::data::CombatantView* attacker,const dh2::data::CombatantView* defender,dh2::data::CombatRandom* random,unsigned magic){
 if(!out||!random||magic>1||!valid(attacker)||!valid(defender))return 1;
 auto rng=*random;Damage result{};dot(result,*attacker,*defender,rng,magic!=0);*out=result;*random=rng;return 0;
}
extern "C" unsigned dh2_combat_damage(dh2::data::Damage* out,const dh2::data::DamageRequest* request){
 if(!out||!request||!request->random||request->flags>15||!valid(request->attacker)||!valid(request->defender))return 1;
 const auto& attacker=*request->attacker;const auto& defender=*request->defender;auto random=*request->random;Damage result{};
 const bool offhand=request->flags&1,magic=request->flags&2,blocked=request->flags&4,critical=request->flags&8;
 const auto type=std::uint32_t(request->type);
 if(type<=1){
  auto minimum=prop(attacker,offhand?81:79),maximum=prop(attacker,offhand?82:80);result.element=shr(prop(attacker,offhand?100:97),8);
  if(magic){minimum=prop(attacker,174);maximum=prop(attacker,175);}
  minimum=std::max(0,minimum);maximum=std::max(0,maximum);auto amount=add(roll(random,minimum,maximum),bonus(attacker,offhand));
  if(defender.state==9)amount=add(amount,prop(attacker,92));
  if(prop(attacker,198)>prop(defender,199))amount=add(amount,prop(attacker,93));
  if(attacker.combo_hits)amount=add(amount,mul(std::int32_t(attacker.combo_hits),prop(attacker,94)));
  if(blocked)amount=shr(mul(amount,prop(defender,121)),8);
  if(critical)amount=add(amount,maximum);
  amount=sub(amount,shr(mul(25,prop(defender,71)),8));if(amount<=0)amount=256;
  if(!magic){result.hp_leech=percent_product(amount,prop(attacker,132));result.mp_leech=percent_product(amount,prop(attacker,133));}
  if(result.element!=-1){
   const auto lo=prop(attacker,offhand?98:95),hi=prop(attacker,offhand?99:96);
   if(lo>=0&&hi>=0){
    const auto rolled=roll(random,lo,hi);auto factor=resistance_factor(prop(defender,add(result.element,74)));
    if(factor>=0){factor=std::min(1,factor);amount=add(amount,mul(rolled,factor));if(amount<=0)amount=256;}
   }
  }
  result.amount=amount;dot(result,attacker,defender,random,magic);
 }else if(type==2){
  const auto minimum=prop(attacker,magic?174:79),maximum=prop(attacker,magic?175:80);auto amount=0;
  if((std::uint32_t(minimum)|std::uint32_t(maximum))!=0){
   amount=roll(random,std::max(0,minimum),std::max(0,maximum));
   if(!magic)amount=add(amount,bonus(attacker,offhand));
   const auto element=magic?request->element:shr(prop(attacker,97),8);
   if(element!=-1){auto factor=resistance_factor(prop(defender,add(element,74)));if(factor<0)amount=256;else{factor=std::min(1,factor);amount=mul(add(amount,prop(attacker,add(element,166))),factor);if(amount<=0)amount=256;}}
   if(critical)amount=add(amount,std::max(0,maximum));
   amount=sub(amount,shr(mul(51,prop(defender,73)),8));if(amount<=0)amount=256;
  }
  result.amount=amount;
  if(magic)dot(result,attacker,defender,random,true);
  else{result.hp_leech=percent_product(amount,prop(attacker,132));result.mp_leech=percent_product(amount,prop(attacker,133));}
 }else if(type==3)result.amount=request->direct_amount;
 *out=result;*request->random=random;return 0;
}
