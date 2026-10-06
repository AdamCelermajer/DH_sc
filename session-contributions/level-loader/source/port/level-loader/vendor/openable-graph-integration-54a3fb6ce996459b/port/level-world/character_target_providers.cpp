#include "character_target_providers.hpp"
#include <cstring>
namespace dh2::target_providers {namespace {
bool aligned(const void* p,std::uintptr_t n){return p&&!(reinterpret_cast<std::uintptr_t>(p)&(n-1));}
bool overlap(const void*a,std::uintptr_t an,const void*b,std::uintptr_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<an:x-y<bn;}
bool state(const Character32* p){return aligned(p,8)&&p->identity&&aligned(p->properties,4);}
bool tables(const Types16*t){return aligned(t,8)&&t->count>=9&&t->count<=65536&&aligned(t->types,4)&&!t->reserved;}
bool service(const Services16*s){return aligned(s,8)&&s->invoke;}
int type(const Character32*c,const Types16*t){auto id=c->properties->words[1];return t->types[id>=0&&static_cast<std::uint32_t>(id)<t->count?id:8];}
bool call(const Services16*s,Service op,std::uintptr_t a,std::uintptr_t b,std::uintptr_t&v){v=0;Request24 q{op,0,a,b};return s->invoke(s->context,&q,&v)==0;}
}
extern "C" int dh2_character_target_query(std::int32_t*out,std::uint32_t op,Character32*c,Character32*other,const Types16*t,const Services16*s){
 if(!aligned(out,4)||!state(c)||!tables(t)||op<1||op>10||overlap(out,4,c,sizeof(*c))||overlap(out,4,c->properties,sizeof(*c->properties)))return 1;
 if(other&&!state(other))return 1;
 if(overlap(out,4,t,sizeof(*t))||overlap(out,4,t->types,t->count*4u)||(other&&(overlap(out,4,other,sizeof(*other))||overlap(out,4,other->properties,sizeof(*other->properties)))))return 1;
 if(op>=6&&op<=8&&!service(s))return 1;
 if(op==5&&type(c,t)==0&&!c->name)return 1;
 std::uintptr_t v=0;int result=0;
 switch(op){
  case char_type:result=type(c,t);break;
  case is_monster:result=type(c,t)==4;break;
  case is_faerie:result=type(c,t)==3;break;
  case is_summoned:result=type(c,t)==5;break;
  case is_player:{auto k=type(c,t);result=k?k==1:std::strncmp(c->name,"PlayerCharacter",15)==0;break;}
  case is_interactive:
   if(!call(s,virtual_dead,c->identity,0,v))return 2;
   if(v&&other){if(!call(s,ai_friend,c->identity,other->identity,v))return 2;if(v&&type(c,t)!=4){result=1;break;}}
   if(c->disabled81||!c->visible8a||type(c,t)==3||type(c,t)==5)break;
   if(!call(s,virtual_dead,c->identity,0,v))return 2;
   if(!v&&(c->flags520&0x2000))result=c->interactive415;
   break;
  case interaction_type:
   if(other){if(!call(s,ai_enemy,c->identity,other->identity,v))return 2;if(v){if(!call(s,virtual_player,other->identity,0,v))return 2;if(!v){result=8;break;}}}
   if(type(c,t)==5){result=-1;break;}
   if(!call(s,virtual_dead,c->identity,0,v))return 2;
   if(v){result=-1;break;}result=type(c,t)==4?8:3;break;
  case is_zonable:
   if(!call(s,virtual_player,c->identity,0,v))return 2;
   result=!v&&type(c,t)!=3; // actual direct GameObject::MeetCondition is 1
   break;
  case is_character:result=1;break;
  case is_dead:result=c->dead1449;break;
 }
 *out=result;return 0;
}
extern "C" int dh2_character_sneak_fields(std::int32_t*out,const character::CombatProperties896*p){if(!aligned(out,4)||!aligned(p,4)||overlap(out,8,p,sizeof(*p)))return 1;out[0]=p->words[198];out[1]=p->words[199];return 0;}
extern "C" int dh2_gameobject_interaction_radius(float*out,const float*aabb){
 if(!aligned(out,4)||!aligned(aabb,4)||overlap(out,4,aabb,24))return 1;
 volatile float x=aabb[3]-aabb[0],y=aabb[4]-aabb[1];float chosen=x;if(x>y)chosen=y;volatile float radius=chosen*0.5f;*out=radius;return 0;
}
extern "C" int dh2_gameobject_target_query(std::int32_t*out,std::uint32_t op,std::uint8_t b){if(!aligned(out,4)||op<1||op>4)return 1;*out=op==2?-1:op==4?b^1:0;return 0;}
extern "C" int dh2_target_handle_character(std::uintptr_t*out,Handle16*local,Handle16*shared,Registry24*registry,const Services16*s){
 if(!aligned(out,8)||!aligned(local,8)||!aligned(shared,8)||!aligned(registry,8)||!service(s)||registry->count>registry->capacity||registry->capacity>65536||!aligned(registry->records,8)||registry->reserved||overlap(local,sizeof(*local),shared,sizeof(*shared))||overlap(out,8,local,sizeof(*local))||overlap(out,8,shared,sizeof(*shared))||overlap(out,8,registry,sizeof(*registry)))return 1;
 const auto span=registry->capacity*sizeof(Record16);
 if(overlap(local,sizeof(*local),registry,sizeof(*registry))||overlap(shared,sizeof(*shared),registry,sizeof(*registry))||overlap(out,8,registry->records,span)||overlap(local,sizeof(*local),registry->records,span)||overlap(shared,sizeof(*shared),registry->records,span)||overlap(registry,sizeof(*registry),registry->records,span))return 1;
 for(std::uint32_t i=0;i<registry->count;++i)if(registry->records[i].reserved||(i&&registry->records[i-1].key>=registry->records[i].key))return 1;
 shared->frame=registry->frame;*local=*shared;
 if(local->key){auto frame=registry->frame;if(!local->cached||local->frame!=frame){
  std::uint32_t lo=0,hi=registry->count;while(lo<hi){auto mid=lo+(hi-lo)/2;if(registry->records[mid].key<local->key)lo=mid+1;else hi=mid;}
  if(lo==registry->count||registry->records[lo].key!=local->key){if(registry->count==registry->capacity)return 2;for(auto i=registry->count;i>lo;--i)registry->records[i]=registry->records[i-1];registry->records[lo]={local->key,0,0};++registry->count;}
  local->cached=registry->records[lo].object;local->frame=frame;
 }}
 std::uintptr_t result=local->key?local->cached:0;
 if(result){std::uintptr_t classified;if(!call(s,virtual_character,result,0,classified))return 2;if(!classified)result=0;}
 *out=result;return 0;
}
}
