#include "../character_skill_target_binding_v46.hpp"
#include <cassert>
#include <vector>
using namespace dh2::character;using namespace dh2::character::skills;
struct Fixture {
 State machine{};TargetOwner16 owner{100,0,0,0};TargetState48 ai{200,&owner,0,11,7,1,1,0,0,0};
 SkillStateV4 fields{&machine,100,0,99,98,0,0,0,{0,0}};
 unsigned fail{},checks{};bool retarget_debug{},retarget_stop{};std::vector<unsigned> order;
 static int call(void* raw,SkillStateV4*,const SkillStateRequest32V4* q,SkillStateResponse16V4* out){
  auto& f=*static_cast<Fixture*>(raw);f.order.push_back(q->operation);
  if(q->operation==skill_state_debug_construct_v4)out->identity=300;
  if(q->operation==skill_state_debug_destroy_v4&&f.retarget_debug)f.ai.target=22;
  if(q->operation==skill_state_stop_v4){assert(f.ai.last_target==f.ai.target);++f.checks;if(f.retarget_stop)f.ai.target=33;}
  if(q->operation==skill_state_monster_v4)out->word=0;
  return f.fail==q->operation+1?-1:0;
 }
 int run(){SkillStateServices16V4 cb{this,call};return character_skill_state_target_bound_v46(fields,ai,skill_state_blur_v4,0,0,0,0,cb);}
};
int main(){
 unsigned checks=0;
 {Fixture f;assert(!f.run());assert(f.ai.target==11&&f.ai.last_target==11&&f.fields.last_target==11&&f.checks==1);++checks;}
 {Fixture f;f.retarget_debug=true;assert(!f.run());assert(f.ai.target==22&&f.ai.last_target==22);++checks;}
 {Fixture f;f.retarget_stop=true;assert(!f.run());assert(f.ai.target==33&&f.ai.last_target==11);++checks;}
 {Fixture f;f.ai.target=0;assert(!f.run());assert(!f.ai.target&&!f.ai.last_target);++checks;}
 {Fixture f;f.fail=skill_state_debug_get_v4+1;assert(f.run()==-2);assert(f.ai.last_target==7&&!f.checks);++checks;}
 {Fixture f;f.fail=skill_state_stop_v4+1;assert(f.run()==-2);assert(f.ai.last_target==11&&f.checks==1);++checks;}
 {Fixture f;f.owner.identity=999;assert(f.run()==-1&&f.order.empty()&&f.ai.last_target==7);++checks;}
 assert(checks==7);
}
