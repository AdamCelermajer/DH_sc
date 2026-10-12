#include "character_dot_attack.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
using dh2::data::CombatResult;
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
bool aligned(const void* p,unsigned n){return p&&(reinterpret_cast<std::uintptr_t>(p)&(n-1))==0;}
bool valid(DotResult24* o,CombatResult* r,DotActor32* a,const DotServices16* s){
 if(!aligned(o,4)||!aligned(r,4)||!aligned(a,8)||!a->identity||a->reserved||!aligned(s,8)||!s->invoke||!aligned(a->properties,8))return false;
 const auto& v=*a->properties;const void* sheets[]={v.defaults,v.types,v.base,v.saved,v.gear,v.resolved};
 for(auto sheet:sheets)if(!aligned(sheet,4))return false;
 if(v.group_count>10000||(v.group_count&&!aligned(v.groups,8)))return false;
 for(unsigned i=0;i<v.group_count;i++){
  const auto& g=v.groups[i];if(g.count>10000||(g.count&&!aligned(g.sheets,8)))return false;
  for(unsigned j=0;j<g.count;j++)if(!aligned(g.sheets[j],4))return false;
 }
 if(dh2_property_validate(a->properties))return false;
 const void* p[]={o,r,a,s,a->properties};const std::size_t z[]={sizeof(*o),sizeof(*r),sizeof(*a),sizeof(*s),sizeof(*a->properties)};
 for(unsigned i=0;i<5;i++)for(unsigned j=i+1;j<5;j++)if(overlap(p[i],z[i],p[j],z[j]))return false;
 for(auto sheet:sheets)for(unsigned i=0;i<5;i++)if(overlap(sheet,896,p[i],z[i]))return false;
 return true;
}
struct Run {
 DotResult24 result{};DotActor32* actor;CombatResult* attack;const DotServices16* services;
 bool call(std::uint32_t service,DotResponse8& response,std::int32_t amount=0,const char* name=nullptr,float threat=0){
  result.phase=service+1;++result.calls;
  DotRequest40 request{service,amount,actor->identity,service==dot_add_aggro||service==dot_hit_for||service==dot_combat_text||service==dot_combat_sound?actor->identity:0,name,threat,0};
  response={};return services->invoke(services->context,actor,&request,&response,attack)==0;
 }
 bool call(std::uint32_t service,std::int32_t amount=0,const char* name=nullptr){DotResponse8 response{};return call(service,response,amount,name);}
 bool debug(const char* name,std::int32_t& word){DotResponse8 response{};if(!call(dot_debug_load)||!call(dot_debug_query,response,0,name))return false;word=response.word;return true;}
 int end(DotResult24* out,int status){result.status=status;*out=result;return status;}
};
bool source_dot(const CombatResult& r){return r.mask==0x20080000u&&r.weapon_category==-1&&r.dot_element==-1&&r.dot_duration==-1&&r.dot_amount==-1&&r.hp_leech==0&&r.mp_leech==0&&r.outcomes==0;}
}
static int calculate(DotResult24* out,CombatResult* attack,DotCombatContext32* combat,DotActor32* actor,std::int32_t amount,std::int32_t element,const DotServices16* services,bool wrapper){
 if(!valid(out,attack,actor,services)||!aligned(combat,8))return -1;
 const void* p[]={out,attack,actor,services,actor->properties};const std::size_t z[]={sizeof(*out),sizeof(*attack),sizeof(*actor),sizeof(*services),sizeof(*actor->properties)};
 for(unsigned i=0;i<5;i++)if(overlap(combat,sizeof(*combat),p[i],z[i]))return -1;
 const auto& v=*actor->properties;const void* sheets[]={v.defaults,v.types,v.base,v.saved,v.gear,v.resolved};for(auto sheet:sheets)if(overlap(combat,sizeof(*combat),sheet,896))return -1;
 Run run{{},actor,attack,services};std::int32_t ignored;
 if((wrapper&&!run.debug("isTracingChar_Attack",ignored))||!run.call(dot_profile_begin,0,"Character::_F_CalculateResult"))return run.end(out,-2);
 *attack=CombatResult{};attack->mask=0x20080000u;attack->element=element;
 *combat={actor->identity,actor->identity,0,0,element,0,0,0,0};
 if(!run.debug("isTracingChar_Attack",ignored))return run.end(out,-2);
 dh2::data::CombatantView self{};self.properties=actor->properties->resolved;
 dh2::data::CombatRandom random{};
 dh2::data::CombatResultRequest request{&self,&self,&random,0x20080000u,-1,element,amount};
 if(dh2_combat_result(attack,&request))return run.end(out,-2);
 if(!run.call(dot_profile_end,0,"Character::_F_CalculateResult"))return run.end(out,-2);
 return run.end(out,1);
}
extern "C" int dh2_character_dot_calculate(DotResult24* o,CombatResult* r,DotCombatContext32* c,DotActor32* a,std::int32_t amount,std::int32_t element,const DotServices16* s){return calculate(o,r,c,a,amount,element,s,true);}
extern "C" int dh2_character_dot_calculate_result(DotResult24* o,CombatResult* r,DotCombatContext32* c,DotActor32* a,std::int32_t amount,std::int32_t element,const DotServices16* s){return calculate(o,r,c,a,amount,element,s,false);}
extern "C" int dh2_character_dot_apply(DotResult24* out,CombatResult* attack,DotActor32* actor,const DotServices16* services){
 if(!valid(out,attack,actor,services)||!source_dot(*attack))return -1;
 Run run{{},actor,attack,services};DotResponse8 response{};std::int32_t disabled=0,god=0;
 if(!run.call(dot_online,response))return run.end(out,-2);
 if(response.word)return run.end(out,-3);
 actor->combo_hits=std::uint16_t(actor->combo_hits+1u);
 if(!run.debug("NoDamages",disabled))return run.end(out,-2);
 if(!disabled){
  if(!run.debug("GOD",god))return run.end(out,-2);
  if(!god){if(!run.call(dot_application_switch,response,0,"GOD"))return run.end(out,-2);god=response.word;}
  if(god){
   if(!run.call(dot_is_player,response))return run.end(out,-2);
   if(response.word)return run.end(out,-3);
  }
  disabled=actor->god!=0;
 }
 if(!disabled&&attack->amount>0){
  if(!run.call(dot_party_count,response))return run.end(out,-2);
  if(response.word>1)return run.end(out,-3);
  volatile float per_damage=float(actor->properties->resolved[204])*0.00390625f;
  volatile float damage=float(attack->amount)*0.00390625f;
  volatile float threat=per_damage*damage;run.result.threat=threat;
  if(!run.call(dot_add_aggro,response,0,nullptr,threat))return run.end(out,-2);
  if(response.number>0&&!run.debug("isTracingThreatChange",god))return run.end(out,-2);
  actor->push_death=0;
  if(actor->network_id==-1){
   if(!run.debug("isTracingChar_Attack",god)||!run.call(dot_hit_for,attack->amount))return run.end(out,-2);
   run.result.hit_called=1;
  }
  if(!run.call(dot_is_dead,response))return run.end(out,-2);
 }
 if(!run.call(dot_regen_hp,attack->hp_leech)||!run.call(dot_regen_mp,attack->mp_leech)||!run.call(dot_is_dead,response))return run.end(out,-2);
 if(!response.word){if(!run.call(dot_is_player,response))return run.end(out,-2);if(response.word)return run.end(out,-3);}
 if(!run.call(dot_cancel_sneaking)||!run.call(dot_combat_text)||!run.call(dot_combat_sound))return run.end(out,-2);
 if(!run.call(dot_is_player,response))return run.end(out,-2);
 if(response.word)return run.end(out,-3);
 if(!run.call(dot_is_player,response))return run.end(out,-2);
 if(response.word)return run.end(out,-3);
 return run.end(out,1);
}
