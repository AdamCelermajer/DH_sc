#include "../game-data/combat_result.hpp"
#include "character_skill_combat_v6.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
namespace {
using namespace dh2::data;
std::int32_t signed_word(std::uint32_t raw){std::int32_t value;std::memcpy(&value,&raw,4);return value;}
std::int32_t add(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)+std::uint32_t(b));}
std::int32_t sub(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)-std::uint32_t(b));}
std::int32_t mul(std::int32_t a,std::int32_t b){return signed_word(std::uint32_t(a)*std::uint32_t(b));}
std::int32_t shift(std::int32_t a,unsigned n){return signed_word(std::uint32_t(a)<<n);}
std::int32_t shr(std::int32_t value,unsigned n){auto bits=std::uint32_t(value);return signed_word((bits>>n)|(value<0?(~0u<<(32-n)):0u));}
std::int32_t low24(std::int32_t a){return shr(shift(a,8),8);}
std::int32_t prop(const CombatantView& a,std::int32_t id){return id>=0&&id<224?a.properties[id]:-1;}
bool valid(const CombatantView* a){return a&&a->properties&&a->main_damage_class>=-1&&a->main_damage_class<141&&a->off_damage_class>=-1&&a->off_damage_class<141&&a->two_hander<=1&&a->dual_wield<=1&&a->shield<=1&&a->combo_hits<=65535;}
std::int32_t attack_bonus(const CombatantView& a,bool offhand){auto category=offhand?a.off_damage_class:a.main_damage_class;if(category==-1)return 0;auto result=prop(a,51+category);return a.dual_wield?add(result,prop(a,58)):result;}
std::int32_t truncate(float value){if(value>=2147483648.f)return std::numeric_limits<std::int32_t>::max();if(value<=-2147483648.f)return std::numeric_limits<std::int32_t>::min();return std::int32_t(value);}
bool status(const CombatantView& attacker,const CombatantView& defender,std::int32_t roll,unsigned field,bool magic){
 static const unsigned normal[]={135,137,139,142,145},magical[]={182,183,184,186,188},resist[]={134,136,138,141,144};
 auto threshold=prop(attacker,magic?magical[field]:normal[field]);
 if(threshold>0){
  if(field==4){
   volatile float bonus=float(prop(attacker,19))*.125f;
   volatile float combined=float(threshold)+bonus;
   auto reduced=sub(truncate(combined),prop(defender,resist[field]));
   volatile float penalty=float(prop(defender,19))*.125f;
   volatile float chance=float(reduced)-penalty;threshold=truncate(chance);
  }else threshold=sub(add(threshold,mul(prop(attacker,19),5)),add(prop(defender,resist[field]),mul(prop(defender,19),5)));
 }
 return threshold>roll;
}
unsigned miss_dodge(const CombatantView& a,const CombatantView& d,CombatRandom& random,std::int32_t roll,bool magic,std::int32_t element,bool offhand,std::int32_t delta){
 const auto dodge_roll=shift(dh2_combat_random(&random,100),8);
 if(roll>=0x6200)return 1;
 if(roll<0x3200)return 0;
 auto accuracy=add(0x4b00,shr(shift(delta,9),8));
 if(magic){auto rating=prop(a,158);if(element!=-1)rating=add(rating,prop(a,add(element,159)));accuracy=add(accuracy,shr(shift(sub(rating,prop(d,164)),9),8));}
 else accuracy=add(accuracy,shr(mul(add(sub(std::max(256,prop(a,50)),prop(d,59)),attack_bonus(a,offhand)),51),8));
 if(roll>=accuracy)return 1;
 return !magic&&add(prop(d,60),low24(sub(0,delta)))>dodge_roll?2:0;
}
}
extern "C" unsigned dh2::character::skills::dh2_combat_result_ordered_v6(dh2::data::CombatResult* out,const dh2::data::CombatResultRequest* request,void* debug_context,int(*debug)(void*)){
 if(!out||!request||!request->random||!valid(request->attacker)||!valid(request->defender))return 1;
 const auto& attacker=*request->attacker;const auto& defender=*request->defender;auto random=*request->random;const auto mask=request->mask;const bool offhand=mask&(1u<<26),magic_damage=mask&(1u<<27);
 CombatResult result;result.mask=mask;result.weapon_category=request->weapon_category;result.element=request->element;
 const auto delta=sub(prop(attacker,19),prop(defender,19));
 if(mask&15){const auto roll=shift(dh2_combat_random(&random,100),8);result.outcomes=miss_dodge(attacker,defender,random,roll,(mask&5)==0,request->element,offhand,delta);}
 if(result.outcomes&3){*out=result;*request->random=random;return 0;}
 bool blocked=false,critical=false;
 if(mask&16){const auto roll=shift(dh2_combat_random(&random,100),8);auto chance=add(prop(defender,61),low24(sub(0,delta)));if(defender.shield)chance=add(chance,prop(defender,62));blocked=roll<0x6200&&roll<chance;if(blocked)result.outcomes|=4;}
 std::int32_t critical_roll=-1;
 if(mask&96){critical_roll=shift(dh2_combat_random(&random,100),8);auto chance=add(prop(attacker,(mask&32)?63:165),low24(delta));critical=(!(mask&32)||critical_roll<0x6200)&&critical_roll<chance;if(critical)result.outcomes|=8;}
 if(mask&384){if(critical_roll==-1)critical_roll=shift(dh2_combat_random(&random,100),8);if(status(attacker,defender,critical_roll,0,(mask&128)==0))result.outcomes|=16;}
 for(const auto pair:{std::pair<unsigned,unsigned>{9,1},{11,2},{13,3},{15,4}}){const auto bits=3u<<pair.first;if(mask&bits){const auto roll=shift(dh2_combat_random(&random,100),8);if(status(attacker,defender,roll,pair.second,(mask&(1u<<pair.first))==0))result.outcomes|=1u<<(pair.second==1?7:pair.second==2?6:pair.second==3?5:8);}}
 *out=result;*request->random=random;
 if(!debug||debug(debug_context))return 2;
 if(mask&0xe0000){Damage damage;const auto type=mask&0x20000?0:mask&0x40000?2:3;DamageRequest calc{&attacker,&defender,&random,type==3?request->direct_amount:0,type,request->element,unsigned(offhand)|(unsigned(magic_damage)<<1)|(unsigned(blocked)<<2)|(unsigned(critical)<<3)};if(dh2_combat_damage(&damage,&calc)!=0)return 1;
  result.amount=damage.amount;
  if(type!=3){result.dot_element=damage.dot_element;result.dot_duration=damage.dot_duration;result.dot_amount=damage.dot_amount;result.hp_leech=damage.hp_leech;result.mp_leech=damage.mp_leech;if(type==0)result.element=damage.element;}
 }
 *out=result;*request->random=random;return 0;
}
