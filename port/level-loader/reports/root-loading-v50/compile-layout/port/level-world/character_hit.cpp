#include "character_hit.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
namespace tp=dh2::target_providers;
bool aligned(const void* p,unsigned n){return p&&!(reinterpret_cast<std::uintptr_t>(p)&(n-1));}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){if(!n||!m)return false;auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
std::int32_t signed_bits(std::uint32_t x){std::int32_t r;std::memcpy(&r,&x,4);return r;}
bool valid(HitResult40* o,HitActor32* a,const HitAttacker24* t,const HitServices16* s){
 if(!aligned(o,4)||!aligned(a,8)||!a->identity||a->reserved||!aligned(a->properties,8)||!aligned(t,8)||!aligned(s,8)||!s->invoke)return false;
 const auto& v=*a->properties;const void* sheets[]={v.defaults,v.types,v.base,v.saved,v.gear,v.resolved};
 for(auto p:sheets)if(!aligned(p,4))return false;
 if(v.group_count>10000||(v.group_count&&!aligned(v.groups,8)))return false;
 for(unsigned i=0;i<v.group_count;i++){const auto& g=v.groups[i];if(g.count>10000||(g.count&&!aligned(g.sheets,8)))return false;for(unsigned j=0;j<g.count;j++)if(!aligned(g.sheets[j],4))return false;}
 if(dh2_property_validate(a->properties))return false;
 if(t->identity){
  if(!aligned(t->shared_handle,8)||!aligned(t->registry,8))return false;
  const auto& r=*t->registry;if(!aligned(r.records,8)||r.count>r.capacity||r.capacity>65536||r.reserved)return false;
  for(unsigned i=0;i<r.count;i++)if(r.records[i].reserved||(i&&r.records[i-1].key>=r.records[i].key))return false;
 }else if(t->shared_handle||t->registry)return false;
 const void* p[]={o,a,t,s,a->properties,t->shared_handle,t->registry,t->registry?t->registry->records:nullptr};
 const std::size_t z[]={sizeof(*o),sizeof(*a),sizeof(*t),sizeof(*s),sizeof(v),t->identity?sizeof(tp::Handle16):0,t->identity?sizeof(tp::Registry24):0,t->identity?t->registry->capacity*sizeof(tp::Record16):0};
 for(unsigned i=0;i<8;i++){for(unsigned j=i+1;j<8;j++)if(overlap(p[i],z[i],p[j],z[j]))return false;for(auto sheet:sheets)if(overlap(p[i],z[i],sheet,896))return false;}
 for(unsigned i=0;i<8;i++){
  if(overlap(p[i],z[i],v.groups,v.group_count*sizeof(*v.groups)))return false;
  for(unsigned j=0;j<v.group_count;j++){const auto& g=v.groups[j];if(overlap(p[i],z[i],g.sheets,g.count*sizeof(*g.sheets)))return false;for(unsigned k=0;k<g.count;k++)if(overlap(p[i],z[i],g.sheets[k],896))return false;}
 }
 return true;
}
struct Run {
 HitResult40 result{};HitActor32* actor;const HitAttacker24* attacker;const HitServices16* services;
 bool call(HitService op,std::uintptr_t& value,std::uintptr_t subject,const char* name=nullptr){
  result.phase=op+1;++result.calls;HitRequest32 q{op,0,subject,op==hit_controller_kill?attacker->identity:0,name};value=0;
  return services->invoke(services->context,actor,&q,&value)==0;
 }
 bool debug(const char* name,std::uintptr_t& value){return call(hit_debug_load,value,0)&&call(hit_debug_query,value,0,name);}
 int end(HitResult40* out,int status){result.status=status;*out=result;return status;}
 static int classify(void* context,const tp::Request24* q,std::uintptr_t* out){auto& r=*static_cast<Run*>(context);return r.call(hit_is_character,*out,q->subject)?0:1;}
};
}
extern "C" int dh2_character_hit_for(HitResult40* out,HitActor32* actor,std::uint32_t damage,const HitAttacker24* attacker,const HitServices16* services){
 if(!valid(out,actor,attacker,services))return -1;
 Run run{{},actor,attacker,services};std::uintptr_t value=0,main_player=0;
 if(!run.call(hit_is_dead,value,actor->identity))return run.end(out,-2);
 if(value)return run.end(out,1);
 auto& r=run.result;r.before_hp=actor->properties->resolved[36];r.maximum_hp=actor->properties->resolved[38];
 if(!run.call(hit_main_player,main_player,0)||!run.debug("GOD_Monster",value))return run.end(out,-2);
 bool suppress=false;
 if(value){if(!run.call(hit_is_monster,value,actor->identity))return run.end(out,-2);suppress=value!=0;}
 if(!suppress){
  if(!run.call(hit_online,value,0))return run.end(out,-2);
  if(value)return run.end(out,-3);
  suppress=!main_player;
  if(main_player){if(!run.call(hit_is_dead,value,main_player))return run.end(out,-2);suppress=value!=0;}
 }
 if(!suppress){r.raw_add=signed_bits(0u-damage);r.credited_damage=signed_bits((damage>>8)|((damage&0x80000000u)?0xff000000u:0));}
 if(dh2_property_add(actor->properties,36,r.raw_add))return run.end(out,-2);
 if(!run.call(hit_application_switch,value,0,"OneShotKill"))return run.end(out,-2);
 if(!value&&!run.debug("OneShotKill",value))return run.end(out,-2);
 if(value){
  if(!run.call(hit_is_player,value,actor->identity))return run.end(out,-2);
  if(value)return run.end(out,-3);
  if(dh2_property_set(actor->properties,36,0))return run.end(out,-2);
 }
 r.after_hp=actor->properties->resolved[36];
 if(r.after_hp<=0){
  if(dh2_property_set(actor->properties,36,0))return run.end(out,-2);
  if(!run.call(hit_controller_kill,value,actor->controller))return run.end(out,-2);
  r.kill_called=1;
  if(!run.call(hit_is_remotely_updated,value,actor->identity))return run.end(out,-2);
  if(!value){actor->lifecycle=3;r.lifecycle_written=1;}
 }
 if(!run.call(hit_is_player,value,actor->identity))return run.end(out,-2);
 if(value)return run.end(out,-3);
 if(!run.call(hit_is_player,value,actor->identity))return run.end(out,-2);
 if(value)return run.end(out,-3);
 if(!attacker->identity)return run.end(out,-3);
 tp::Handle16 local{};std::uintptr_t resolved=0;tp::Services16 classify{&run,Run::classify};
 if(tp::dh2_target_handle_character(&resolved,&local,attacker->shared_handle,attacker->registry,&classify))return run.end(out,-2);
 if(!resolved)return run.end(out,1);
 if(!run.call(hit_is_player,value,resolved))return run.end(out,-2);
 return run.end(out,value&&resolved!=actor->identity?-3:1);
}
