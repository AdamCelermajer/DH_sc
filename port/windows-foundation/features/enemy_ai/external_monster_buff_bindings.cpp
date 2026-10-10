#include "external_monster_buff_bindings.hpp"
namespace dh::foundation::enemy_ai {
int select_external_monster_buff(void* raw,std::uint32_t address,dh2_script_function* fn,void** context){
 if(!fn||!context)return -1;
 if(address!=0x3b86a8&&address!=0x3b842c&&address!=0x3babdc)return 0;
 auto* b=static_cast<ExternalMonsterBuffBindings*>(raw);
 if(!b||!b->receiver_lease||!b->buffs||!b->buffs->owner||!b->properties||
    !b->same_buff_properties||b->properties->owner!=b->same_buff_properties)return -1;
 using namespace dh2::character::skills;
 if(address==0x3b86a8){*fn=skill_create_buff_v3;*context=b->buffs;}
 else if(address==0x3b842c){*fn=skill_remove_buff_v3;*context=b->buffs;}
 else {*fn=skill_apply_class_v1;*context=b->properties;}
 return 1;
}
}
