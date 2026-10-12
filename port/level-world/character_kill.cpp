#include "character_kill.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
bool aligned(const void* p,unsigned n){return p&&!(reinterpret_cast<std::uintptr_t>(p)&(n-1));}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){if(!n||!m)return false;auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
int bits(unsigned x){int n;std::memcpy(&n,&x,4);return n;}
int get_int(const dh2::data::PropertyView* p,unsigned i){auto x=std::uint32_t(p->resolved[i]);return bits((x>>8)|((x&0x80000000u)?0xff000000u:0));}
bool actor_valid(const KillActor56* a){
 if(!aligned(a,8)||!a->identity||!aligned(a->properties,8))return false;
 for(auto b:a->reserved)if(b)return false;
 const auto& v=*a->properties;const void* sheets[]={v.defaults,v.types,v.base,v.saved,v.gear,v.resolved};for(auto p:sheets)if(!aligned(p,4))return false;
 if(v.group_count>10000||(v.group_count&&!aligned(v.groups,8)))return false;
 for(unsigned i=0;i<v.group_count;i++){const auto& g=v.groups[i];if(g.count>10000||(g.count&&!aligned(g.sheets,8)))return false;for(unsigned j=0;j<g.count;j++)if(!aligned(g.sheets[j],4))return false;}
 return dh2_property_validate(a->properties)==0;
}
bool valid(KillResult24* o,KillActor56* a,KillWorld16* w,const KillServices16* s){
 if(!aligned(o,4)||!actor_valid(a)||!aligned(w,8)||!aligned(s,8)||!s->invoke)return false;
 const auto& v=*a->properties;const void* p[]={o,a,w,s,a->properties};const std::size_t z[]={sizeof(*o),sizeof(*a),sizeof(*w),sizeof(*s),sizeof(v)};const void* sheets[]={v.defaults,v.types,v.base,v.saved,v.gear,v.resolved};
 for(unsigned i=0;i<5;i++){
  for(unsigned j=i+1;j<5;j++)if(overlap(p[i],z[i],p[j],z[j]))return false;
  for(auto sheet:sheets)if(overlap(p[i],z[i],sheet,896))return false;
  if(overlap(p[i],z[i],v.groups,v.group_count*sizeof(*v.groups)))return false;
  for(unsigned j=0;j<v.group_count;j++){const auto& g=v.groups[j];if(overlap(p[i],z[i],g.sheets,g.count*sizeof(*g.sheets)))return false;for(unsigned k=0;k<g.count;k++)if(overlap(p[i],z[i],g.sheets[k],896))return false;}
 }return true;
}
struct Run {
 KillResult24 result{};KillActor56* actor;std::uintptr_t attacker;KillWorld16* world;const KillServices16* services;
 bool call(KillService op,KillResponse16& response,std::uintptr_t subject=0,std::uintptr_t target=0,int argument=0,const char* name=nullptr,const char* key=nullptr,const KillQuest48* event=nullptr,int index=0){
  result.phase=op+1;++result.calls;KillRequest56 q{op,argument,subject,target,name,key,event,index,0};response={};return services->invoke(services->context,actor,&q,&response)==0&&!response.reserved;
 }
 bool event(std::uintptr_t subject,int number,std::uintptr_t target){KillResponse16 r;if(!call(kill_raise_event,r,subject,target,number))return false;++result.events_raised;return true;}
 bool trophy(const char* name,std::uintptr_t captured){KillResponse16 r;if(!call(kill_trophy_id,r,0,0,0,name))return false;if(!call(kill_unlock_trophy,r,captured,0,r.word))return false;++result.trophies_unlocked;return true;}
 int end(KillResult24* out,int status){result.status=status;*out=result;return status;}
 int body(std::uint32_t force){
  KillResponse16 r;
  if(!call(kill_is_dead,r,actor->identity))return -2;
  if(r.word)return 1;
  actor->dead=1;result.dead_written=1;
  if(dh2_property_set(actor->properties,36,0))return -2;
  if(!call(kill_is_player,r,actor->identity))return -2;
  if(r.word&&!force){
   if(dh2_property_add(actor->properties,25,256))return -2;
   if(!call(kill_is_local_player,r,actor->identity))return -2;
   if(r.word){auto manager=world->trophy_manager;const int threshold[]={10,50,100};const char* names[]={"quest_died_10_times","quest_died_50_times","quest_died_100_times"};for(unsigned i=0;i<3;i++)if(get_int(actor->properties,25)==threshold[i]){if(!trophy(names[i],manager))return -2;break;}}
   if(!call(kill_online,r))return -2;
   if(r.word&&!call(kill_get_local_player,r,0,0,1,nullptr,nullptr,nullptr,0))return -2;
   return 1;
  }
  if(!call(kill_current_level,r))return -2;
  auto* level=reinterpret_cast<KillLevel16*>(r.pointer);
  if(!level)return -3;
  if(!aligned(level,8)||!level->identity||level->reserved)return -2;
  if(!level->loot_gate150&&!call(kill_drop_loot,r,actor->identity,attacker))return -2;
  if(force)return 1;
  std::uintptr_t credited=0;
  if(attacker){
   actor->killer=attacker;credited=attacker==actor->identity;
   for(int i=0;;++i){
    if(!call(kill_aggro_count,r,actor->identity))return -2;
    if(i>=r.word)break;
    if(!call(kill_aggro_entry,r,actor->identity,0,0,nullptr,nullptr,nullptr,i))return -2;
    auto* original=reinterpret_cast<KillActor56*>(r.pointer);
    if(!original)continue;
    if(!actor_valid(original))return -2;
    auto* selected=original->owner?original->owner:original;
    if(!actor_valid(selected))return -2;
    if(!event(selected->identity,4,actor->identity))return -2;
    if(!call(kill_is_player,r,selected->identity))return -2;
    if(!r.word)continue;
    if(selected->tracked_target==actor->identity)selected->tracked_target=0;
    if(dh2_property_add(selected->properties,23,256)||dh2_property_add(selected->properties,24,256))return -2;
    if(attacker==original->identity)credited=1;
    if(!call(kill_is_local_player,r,selected->identity))return -2;
    if(r.word){auto manager=world->trophy_manager;const int threshold[]={100,500,1000,2000};const char* names[]={"killed_100","killed_500","killed_1000","killed_2000"};for(unsigned j=0;j<4;j++)if(get_int(selected->properties,23)==threshold[j]){if(!trophy(names[j],manager))return -2;break;}}
   }
   if(credited){
    if(!call(kill_is_character,r,actor->killer))return -2;
    std::uintptr_t character=0;
    if(r.word){if(!call(kill_handle_character,r,actor->killer))return -2;character=r.pointer;}
    if(!call(kill_distribute_xp,r,character,actor->identity))return -2;
   }
  }
  if(!call(kill_is_remotely_updated,r,actor->identity))return -2;
  if(r.word||actor->suppress_quest)return 1;
  if(!call(kill_current_level,r))return -2;
  level=reinterpret_cast<KillLevel16*>(r.pointer);
  if(!level)return -3;
  if(!aligned(level,8)||!level->identity||level->reserved)return -2;
  const auto captured_level=level->identity;
  const char* keys[]={"KillXEnemies","ClearEnemies","KillEnemyTemplate","ClearEnemyTemplate"};
  for(unsigned i=0;i<4;i++){
   if(i==2&&actor->template_id==-1)break;
   // The source captures OID/subject before each constant query, then queues
   // that event. Mutations during query/queue affect only subsequent captures.
   const int subject=i<2?actor->property_id:actor->template_id;
   const int oid=actor->oid;
   if(!call(kill_constant,r,world->constants,0,0,"v2QuestObjectiveType",keys[i]))return -2;
   KillQuest48 event{i+1,std::uint32_t(r.word),attacker,oid,-1,subject,0,0,0,captured_level,0};
   if(!call(kill_raise_async,r,captured_level,0,0,nullptr,nullptr,&event))return -2;
  }
  return 1;
 }
};
}
extern "C" int dh2_character_kill(KillResult24* o,KillActor56* a,std::uintptr_t attacker,std::uint32_t force,KillWorld16* w,const KillServices16* s){if(!valid(o,a,w,s))return -1;Run r{{},a,attacker,w,s};return r.end(o,r.body(force));}
extern "C" int dh2_character_ctrl_kill(KillResult24* o,KillActor56* a,std::uintptr_t attacker,std::uint32_t force,KillWorld16* w,const KillServices16* s){
 if(!valid(o,a,w,s))return -1;
 Run r{{},a,attacker,w,s};KillResponse16 q;
 if(!r.call(kill_is_dead,q,a->identity))return r.end(o,-2);
 if(q.word)return r.end(o,1);
 const auto status=r.body(force);if(status!=1)return r.end(o,status);
 if(!r.event(a->identity,2,attacker))return r.end(o,-2);
 return r.end(o,1);
}
