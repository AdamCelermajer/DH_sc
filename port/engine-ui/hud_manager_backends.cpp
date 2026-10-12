#include "hud_manager_backends.hpp"
#include <cstddef>
namespace {
template<class T>bool aligned(const T*p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
}
extern "C" int dh2_ui_hud_num_potions(const std::int16_t*p,std::int32_t*out)noexcept{
 if(!aligned(out)||(p&&!aligned(p)))return -1;*out=p?*p:0;return 0;
}
extern "C" int dh2_ui_hud_skill_slot(const dh2::ui::HudSkillSlotTree*tree,std::int32_t slot,std::int32_t*out)noexcept{
 if(!aligned(out))return -1;if(!tree){*out=-1;return 0;}if(!aligned(tree)||tree->reserved||tree->maximum_nodes>65536)return -1;
 auto*p=tree->root;const dh2::ui::HudSkillSlotNode* candidate=nullptr;std::uint32_t visits=0;
 while(p){if(!aligned(p)||++visits>tree->maximum_nodes)return -1;if(slot>p->key)p=p->right;else {candidate=p;p=p->left;}}
 *out=candidate&&slot>=candidate->key?candidate->value:-1;return 0;
}
extern "C" int dh2_ui_hud_skill_level(const dh2::ui::HudSavedSkillLevels*p,std::uint32_t i,std::int32_t*out)noexcept{
 if(!aligned(out))return -1;if(!p){*out=-1;return 0;}if(!aligned(p)||p->reserved||p->count>65536)return -1;
 if(!p->rows){*out=-1;return 0;}if(!aligned(p->rows)||i>=p->count)return -1;*out=p->rows[i].level;return 0;
}
extern "C" int dh2_ui_hud_cooldown(const dh2::ui::HudCooldown*p,float*out)noexcept{
 if(!aligned(p)||!aligned(out)||p->reserved)return -1;if(p->timer_id==-1){*out=0.f;return 0;}
 std::uint32_t elapsed=0,duration=0;auto rc=dh2_character_timer_time_left(&elapsed,&duration,p->timers,static_cast<std::uint32_t>(p->timer_id));if(rc<0)return -1;
 if(!rc){*out=0.f;return 0;}volatile float a=static_cast<float>(elapsed),b=static_cast<float>(duration),ratio=a/b;*out=1.f-ratio;return 0;
}
extern "C" int dh2_ui_hud_skill_query(dh2::ui::HudSkillAI*ai,std::uint32_t op,std::uint32_t index,std::int32_t level,dh2::ui::HudSkillResponse*out,const dh2::ui::HudSkillServices*services)noexcept{
 using namespace dh2::ui;if(!aligned(ai)||!aligned(out)||!aligned(services)||!services->invoke||op>3||ai->reserved||ai->reserved1||ai->reserved2)return -1;
 int failure=0;auto call=[&](HudSkillOperation operation,std::uintptr_t subject,std::uint32_t i=0,std::int32_t l=0){HudSkillResponse r=operation==HudSkillOperation::script_info?*out:HudSkillResponse{};HudSkillRequest q{operation,i,l,0,subject};if(!failure&&services->invoke(services->context,ai,&q,&r)!=1)failure=-2;return r;};
 auto owner=[&](){if(!aligned(ai->owner))failure=-1;return failure?std::uintptr_t(0):reinterpret_cast<std::uintptr_t>(ai->owner);};
 bool spell=(op&1u)!=0;
 if(op<2){auto p=owner();if(failure)return failure;auto current=call(HudSkillOperation::current_state,p).value;if(failure)return failure;
  if(current==6){if(spell){out->value=0;return 0;}p=owner();if(failure)return failure;if(!(ai->owner->flags&0x8000u)){out->value=0;return 0;}}
  p=owner();if(failure)return failure;current=call(HudSkillOperation::current_state,p).value;if(failure)return failure;
  if(current==7||ai->script_stage<=6){out->value=0;return 0;}
 }
 if(spell){auto p=owner();if(failure)return failure;index=static_cast<std::uint32_t>(call(HudSkillOperation::selected_spell,p,UINT32_MAX).value);if(failure)return failure;}
 auto rows=spell?ai->spells:ai->skills;auto count=spell?ai->spell_count:ai->skill_count;
 if(count>65536||index>=count||!aligned(rows))return -1;auto script=rows[index];
 if(!script){if(op<2)out->value=0;else out->fraction=0.f;return 0;}
 auto r=call(op<2?HudSkillOperation::script_usable:HudSkillOperation::script_info,script,index,op<2||spell?0:level);if(failure)return failure;
 if(op<2)out->value=r.value;else out->fraction=r.fraction;return 0;
}
