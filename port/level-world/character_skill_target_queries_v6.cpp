#include "character_skill_target_queries_v6.hpp"
using dh2::character::skills::SkillTargetCharacterV6;
#include <cstring>
namespace dh2::character::skills {using namespace dh2::target_providers;namespace {
bool aligned(const void* p,std::uintptr_t n){return p&&!(reinterpret_cast<std::uintptr_t>(p)&(n-1));}
bool overlap(const void*a,std::uintptr_t an,const void*b,std::uintptr_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<an:x-y<bn;}
bool state(const SkillTargetCharacterV6* p){return aligned(p,8)&&p->identity&&aligned(p->resolved,4);}
bool tables(const Types16*t){return aligned(t,8)&&t->count>=9&&t->count<=65536&&aligned(t->types,4)&&!t->reserved;}
bool service(const Services16*s){return aligned(s,8)&&s->invoke;}
int type(const SkillTargetCharacterV6*c,const Types16*t){auto id=c->resolved[1];return t->types[id>=0&&static_cast<std::uint32_t>(id)<t->count?id:8];}
bool call(const Services16*s,Service op,std::uintptr_t a,std::uintptr_t b,std::uintptr_t&v){v=0;Request24 q{op,0,a,b};return s->invoke(s->context,&q,&v)==0;}
}
static int target_query(std::int32_t*out,std::uint32_t op,SkillTargetCharacterV6*c,SkillTargetCharacterV6*other,const Types16*t,const Services16*s,bool live_flags){
 if(!aligned(out,4)||!state(c)||!tables(t)||op<1||op>10||overlap(out,4,c,sizeof(*c))||overlap(out,4,c->resolved,896))return 1;
 if(other&&!state(other))return 1;
 if(overlap(out,4,t,sizeof(*t))||overlap(out,4,t->types,t->count*4u)||(other&&(overlap(out,4,other,sizeof(*other))||overlap(out,4,other->resolved,896))))return 1;
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
   if(!v){auto flags=c->flags520;if(live_flags){if(!call(s,live_state_flags,c->identity,0,v))return 2;flags=std::uint32_t(v);}if(flags&0x2000)result=c->interactive415;}
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
extern "C" int dh2_character_skill_target_query_v6(std::int32_t*out,std::uint32_t op,SkillTargetCharacterV6*c,SkillTargetCharacterV6*other,const Types16*t,const Services16*s){return target_query(out,op,c,other,t,s,false);}
extern "C" int dh2_character_skill_target_query_live_flags_v6(std::int32_t*out,std::uint32_t op,SkillTargetCharacterV6*c,SkillTargetCharacterV6*other,const Types16*t,const Services16*s){return target_query(out,op,c,other,t,s,true);}
}
