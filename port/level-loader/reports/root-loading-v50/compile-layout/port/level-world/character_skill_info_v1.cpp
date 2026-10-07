#include "character_skill_info_v1.hpp"
#include <cmath>
#include <cstdint>
#include <limits>
namespace dh2::character::skills {namespace {
bool aligned(const void* p,std::size_t alignment){return !p||reinterpret_cast<std::uintptr_t>(p)%alignment==0;}
int query(const SkillInfoServicesV1* s,const SkillInfoRequestV1& q,SkillInfoResponseV1& r){if(!s||!s->invoke)return -2;r={};return s->invoke(s->context,&q,&r)==0&&!r.reserved?0:-2;}
std::int32_t integer(float f){if(std::isnan(f))return 0;if(f>=2147483648.0f)return INT32_MAX;if(f< -2147483648.0f)return INT32_MIN;return static_cast<std::int32_t>(f);}
}
int character_skill_info_v1(State40* state,std::uint32_t index,std::uint32_t level,float* fraction,const SkillInfoServicesV1* svc){
 if(!state||!aligned(state,alignof(State40))||state->skills.reserved||index>=state->skills.count||!state->skills.items||!aligned(state->skills.items,alignof(Instance32*))||!aligned(fraction,alignof(float))||!aligned(svc,alignof(SkillInfoServicesV1)))return -1;
 const Instance32* instance=state->skills.items[index];if(!instance){if(!fraction)return -1;*fraction=0;return 0;}
 if(!aligned(instance,alignof(Instance32))||instance->reserved||!instance->owner||!instance->script)return -1;
 SkillInfoResponseV1 r;SkillInfoRequestV1 q{skill_info_active_v1,0,instance->owner,0,instance,nullptr,0,0};int code=query(svc,q,r);if(code)return code;
 if(!r.active){if(fraction)*fraction=0;return 0;}
 q={skill_info_set_v1,0,instance->owner,r.active,instance,"SetSkill",0,0};if((code=query(svc,q,r)))return code;if(r.source_error)return 0;
 q={skill_info_active_v1,0,instance->owner,0,instance,nullptr,0,0};if((code=query(svc,q,r)))return code;if(!r.active)return -2; // original second receiver has no null guard
 q={skill_info_call_v1,level,instance->owner,r.active,instance,"OnSkillInfo",0,0};if((code=query(svc,q,r)))return code;if(r.source_error||!fraction)return 0;
 *fraction=0;if(!r.return_count||r.first_type!=3)return 0;
 q={skill_info_timer_v1,0,instance->owner,0,instance,nullptr,integer(r.first_number),0};if((code=query(svc,q,r)))return code;if(r.timer_found)*fraction=1.0f-static_cast<float>(r.elapsed)/static_cast<float>(r.duration);return 0;
}
}
extern "C" int dh2_character_skill_info_v1(dh2::character::skills::State40* state,std::uint32_t index,std::uint32_t level,float* fraction,const dh2::character::skills::SkillInfoServicesV1* services){return dh2::character::skills::character_skill_info_v1(state,index,level,fraction,services);}
