#include "character_skill_cooldown_v3.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dh2::character::skills {namespace {
int failure(char* e,std::size_t n,const char* s){if(e&&n)std::snprintf(e,n,"%s",s);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
std::uint32_t u32(float f){std::uint32_t b;std::memcpy(&b,&f,4);const auto e=(b>>23)&255,frac=b&0x7fffff;if((b>>31)||e<127||(e==255&&frac))return 0;if(e>158)return UINT32_MAX;return ((frac<<8)|0x80000000u)>>(158-e);}
std::int32_t i32(float f){if(std::isnan(f))return 0;if(f>=2147483648.f)return INT32_MAX;if(f< -2147483648.f)return INT32_MIN;return std::int32_t(f);}
std::int32_t bits(std::uint32_t value){std::int32_t out;std::memcpy(&out,&value,4);return out;}
bool number(SkillCooldownBindingsV3& b,const dh2_script_value& v,float& f){if(v.type==0)f=0;else if(v.type==1)f=float(v.boolean!=0);else if(v.type==3)f=v.number;else return b.number&&b.number(b.context,&v,&f)==0;return true;}
}
int skill_set_cooldown_v3(void* p,const dh2_script_value* a,std::uint32_t count,
 dh2_script_value*,std::uint32_t,std::uint32_t* result,char* error,std::size_t size){
 if(!result||(count&&!a))return -1;*result=0;if(count<2)return 0;
 auto* b=static_cast<SkillCooldownBindingsV3*>(p);if(!b||!b->owner)return failure(error,size,"Skill cooldown owner unavailable");float converted;
 if(a[0].type!=3){if(!number(*b,a[0],converted))return failure(error,size,"Skill cooldown index conversion unavailable");std::uint32_t list_count;if(b->owner->list_count(0,&list_count))return failure(error,size,"Skill cooldown live list unavailable");if(u32(converted)>=list_count)return 0;}
 if(a[1].type!=0&&a[1].type!=3)return 0;
 // Source repeats conversion after the nonnumber guard. Numeric indices have
 // no source range guard; reject its assertion-unsafe pointer domain explicitly.
 if(!number(*b,a[0],converted))return failure(error,size,"Skill cooldown second conversion unavailable");const auto index=i32(converted);
 const auto& slots=b->owner->state().skills;if(index<0||std::uint32_t(index)>=slots.count)return failure(error,size,"Skill cooldown source index outside owned slots");
 if(!slots.items[index])return 0;const auto timer=a[1].type==0?-1:bits(u32(a[1].number));
 return b->owner->set_cooldown(0,std::uint32_t(index),timer)<0?failure(error,size,"Skill cooldown owned write failed"):0;
}
int spell_set_cooldown_v3(void* p,const dh2_script_value* a,std::uint32_t count,
 dh2_script_value*,std::uint32_t,std::uint32_t* result,char* error,std::size_t size){
 if(!result||(count&&!a))return -1;*result=0;if(!count||(a[0].type!=0&&a[0].type!=3))return 0;
 auto* b=static_cast<SkillCooldownBindingsV3*>(p);if(!b||!b->owner)return failure(error,size,"Spell cooldown owner unavailable");
 const auto timer=a[0].type==0?-1:bits(u32(a[0].number));std::uint32_t count_spells;if(b->owner->list_count(1,&count_spells))return failure(error,size,"Spell cooldown live list unavailable");
 for(unsigned n=0;n<count_spells;++n)if(b->owner->set_cooldown(1,n,timer)<0)return failure(error,size,"Spell cooldown owned write failed");return 0;
}
}
