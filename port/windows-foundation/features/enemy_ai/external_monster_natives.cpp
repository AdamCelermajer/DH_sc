#include "external_monster_natives.hpp"
#include <cstdio>
namespace dh::foundation::enemy_ai {
int external_monster_do_skill(void* context,const dh2_script_value* a,std::uint32_t n,
 dh2_script_value*,std::uint32_t,std::uint32_t* returned,char* error,std::size_t size){
 if(!returned||(n&&!a))return -1;*returned=0;
 // The source argument guard precedes receiver/SkillOwner access.
 if(!n||a[0].type!=DH2_SCRIPT_NUMBER)return 0;
 auto* b=static_cast<ExternalMonsterDoSkill*>(context);std::string e;
 auto fail=[&](const char* missing){if(error&&size)std::snprintf(error,size,"%s",e.empty()?missing:e.c_str());return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;};
 if(!b||!b->character||!b->receiver_lease)return fail("Required retained Character DoSkill receiver");
 std::uint32_t index{},count{};
 if(!b->get_unsigned||!b->get_unsigned(a[0],index,e))return fail("Required original Value.getUnsigned conversion");
 if(!b->skill_count||!b->skill_count(count,e))return fail("Required SAME Character.GetSkills native vector count");
 if(index>=count)return 0;
 std::int32_t native_index{};
 if(!b->get_number_integer||!b->get_number_integer(a[0],native_index,e))return fail("Required original Value.getNumber signed conversion");
 if(!b->use_skill||!b->use_skill(native_index,e))return fail("Required SAME NPC CharAI.AI_UseSkill and genuine Check/FSM/SkillAI providers");
 return 0;
}
int select_external_monster_do_skill(void* context,std::uint32_t address,dh2_script_function* fn,void** out){
 if(!fn||!out)return -1;
 if(address!=0x3b8bd8)return 0;
 auto* b=static_cast<ExternalMonsterDoSkill*>(context);
 if(!b||!b->character||!b->receiver_lease)return -1;
 *fn=external_monster_do_skill;*out=b;return 1;
}
}
