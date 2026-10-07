#include "character_skill_target_binding_v46.hpp"
namespace dh2::character::skills { namespace {
struct Bridge {
 SkillStateV4& state;TargetState48& target;const SkillStateServices16V4& services;
 bool blur,synced{};
 static int invoke(void* raw,SkillStateV4* fields,const SkillStateRequest32V4* q,SkillStateResponse16V4* out){
  auto& b=*static_cast<Bridge*>(raw);
  if(fields!=&b.state||!q||!out)return -1;
  // CSSkill::OnBlur 3c43ac calls AI_SyncLastTarget3d49c4 after
  // Debug/CString destruction and immediately before GameObject::Stop.
  // Refresh here, rather than before debug or after reentrant Stop/Post.
  if(b.blur&&!b.synced&&q->operation==skill_state_stop_v4){
   if(dh2_character_ai_sync_last_target(&b.target))return -1;
   b.state.target=b.target.target;b.state.last_target=b.target.last_target;
   b.synced=true;
  }
  return b.services.invoke(b.services.context,fields,q,out);
 }
}; }
int character_skill_state_target_bound_v46(SkillStateV4& state,TargetState48& target,
 std::uint32_t operation,std::uint32_t index,std::uint32_t moving,
 std::uintptr_t payload,std::uint32_t force,const SkillStateServices16V4& services){
 if(!services.invoke||!target.owner||target.owner->identity!=state.character)return -1;
 state.target=target.target;state.last_target=target.last_target;
 Bridge bridge{state,target,services,operation==skill_state_blur_v4};
 const SkillStateServices16V4 bound{&bridge,Bridge::invoke};
 return dh2_character_skill_state_v4(&state,operation,index,moving,payload,force,&bound);
}
}
