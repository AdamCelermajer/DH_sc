#include "../character_skill_runtime_binding_v48.hpp"
#include <cassert>
#include <vector>
using namespace dh2::character;using namespace dh2::character::skills;
struct Fixture {
 State machine{};TargetOwner16 owner{100,0,0,0};TargetState48 ai{200,&owner,0,11,7,1,1,0,0,0};
 SkillStateV4 fields{&machine,100,0,99,98,0,0,0,{0,0}};std::uintptr_t physical=111;
 unsigned change{UINT32_MAX},fail{};std::uintptr_t replacement{};std::vector<std::uintptr_t> pins,unpins;
 static int call(void* raw,SkillStateV4*,const SkillStateRequest32V4* q,SkillStateResponse16V4* out){
  auto& f=*static_cast<Fixture*>(raw);
  if(q->operation==skill_state_debug_construct_v4)out->identity=300;
  if(q->operation==skill_state_monster_v4)out->word=0;
  if(q->operation==f.change)f.physical=f.replacement;
  if(q->operation==skill_state_pin_v4)f.pins.push_back(q->subject);
  if(q->operation==skill_state_unpin_v4)f.unpins.push_back(q->subject);
  return f.fail==q->operation+1?-1:0;
 }
 int run(unsigned op){SkillStateServices16V4 cb{this,call};return character_skill_runtime_bound_v48(fields,ai,&physical,op,0,0,0,0,cb);}
};
int main(){unsigned checks=0;
 {Fixture f;assert(!f.run(skill_state_focus_v4)&&f.unpins==std::vector<std::uintptr_t>{111});++checks;}
 {Fixture f;f.physical=0;assert(!f.run(skill_state_focus_v4)&&f.unpins.empty());++checks;}
 {Fixture f;f.change=skill_state_animation_v4;f.replacement=222;assert(!f.run(skill_state_focus_v4)&&f.unpins==std::vector<std::uintptr_t>{222});assert(f.physical==222);++checks;}
 {Fixture f;f.change=skill_state_cancel_sneaking_v4;f.replacement=0;assert(!f.run(skill_state_focus_v4)&&f.unpins.empty());++checks;}
 {Fixture f;f.change=skill_state_raise_v4;f.replacement=444;assert(!f.run(skill_state_blur_v4)&&f.pins==std::vector<std::uintptr_t>{444});assert(f.ai.last_target==11);++checks;}
 {Fixture f;f.machine.attack_gate=0x100;assert(!f.run(skill_state_blur_v4)&&f.pins.empty());++checks;}
 {Fixture f;f.fail=skill_state_animation_v4+1;assert(f.run(skill_state_focus_v4)==-2&&f.unpins.empty()&&f.machine.flags==0x6341);++checks;}
 {Fixture f;f.owner.identity=999;assert(f.run(skill_state_focus_v4)==-1&&!f.fields.physical&&f.machine.flags==0);++checks;}
 {Fixture f;SkillStateServices16V4 cb{&f,Fixture::call};assert(character_skill_runtime_bound_v48(f.fields,f.ai,nullptr,skill_state_focus_v4,0,0,0,0,cb)==-1&&!f.fields.physical);++checks;}
 assert(checks==9);
}
