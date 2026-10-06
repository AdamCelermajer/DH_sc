#include "character_animation_step_fx_v2.hpp"
#include "canonical_point3d_globals_v1.hpp"
namespace dh2::character {
int character_animation_step_fx_v2(const data::AnimationStep& step,const AnimationStepFxServicesV2& s,std::string& error){
 error.clear();auto fail=[&](const char* name){error=std::string("Required source animation step FX ")+name;return -1;};
 bool gate=true;
 if(step.swoosh&&(!s.swoosh_fx_gate||s.swoosh_fx_gate(s.context,step,gate)))return fail("Swoosh/equipment gate");
 if(!gate||step.fx==-1)return 0;
 std::uintptr_t owner=0;if(!s.owner||s.owner(s.context,owner)||!owner)return fail("same Character owner");
 if(!s.play)return fail("same VisualFXManager");
 if(step.anchor_fx){
  if(s.play(s.context,step.fx,world::canonical_vec3_origin_v1().data(),nullptr,owner))return fail("anchored PlayAnimFXSet");
 }else{
  float position[3],rotation[3];
  if(!s.target_position||s.target_position(s.context,owner,position))return fail("GetTargetPosition");
  if(!s.rotation||s.rotation(s.context,owner,rotation))return fail("actual rotation16c");
  if(s.play(s.context,step.fx,position,rotation,0))return fail("unanchored PlayAnimFXSet");
 }
 return 0;
}
}
