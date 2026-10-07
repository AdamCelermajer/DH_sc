#include "character_skill_runtime_binding_v48.hpp"
namespace dh2::character::skills {namespace {
struct PhysicalProjection {
 SkillStateV4& fields;const std::uintptr_t* slot;const SkillStateServices16V4& services;
 static int invoke(void* raw,SkillStateV4* fields,const SkillStateRequest32V4* q,SkillStateResponse16V4* out){
  auto& b=*static_cast<PhysicalProjection*>(raw);
  if(fields!=&b.fields)return -1;
  const int result=b.services.invoke(b.services.context,fields,q,out);
  // Focus reads2dc after Pre/SetAnim/CancelSneak; Blur reads after Stop/Post.
  // Reload the borrowed projection after each completed synchronous callback,
  // preserving those original late reads without writing the actual slot.
  if(!result)b.fields.physical=*b.slot;
  return result;
 }
};}
int character_skill_runtime_bound_v48(SkillStateV4& fields,TargetState48& target,
 const std::uintptr_t* physical,std::uint32_t operation,std::uint32_t index,
 std::uint32_t moving,std::uintptr_t payload,std::uint32_t force,const SkillStateServices16V4& services){
 if(!physical||reinterpret_cast<std::uintptr_t>(physical)%alignof(std::uintptr_t)||
  !services.invoke||!target.owner||target.owner->identity!=fields.character)return -1;
 fields.physical=*physical;
 PhysicalProjection bridge{fields,physical,services};const SkillStateServices16V4 bound{&bridge,PhysicalProjection::invoke};
 return character_skill_state_target_bound_v46(fields,target,operation,index,moving,payload,force,bound);
}
}
