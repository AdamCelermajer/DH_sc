#pragma once
#include <retained_gameobject_visual_v1.hpp>
#include <canonical_door_v27.hpp>
#include <canonical_trigger_object_v28.hpp>
namespace model_renderer {
// Original AnimController.PlayClip4749f4(name,bool loop,int priority,uint group).
// Recovered base body never reads priority; GetAnim4748b8 selects the actual
// animator list by group ordinal. This retained generic domain has one root
// animator, as its actual constructor/identity method already exposes.
inline bool retained_idle_animation_source_v86(dh2::world::RetainedGameObjectVisualV1& visual,
 const char* name,bool loop,std::int32_t /* source_unused_priority */,
 std::uint32_t group,std::string& e){
 if(!name){e="Required original named idle CString";return false;}
 std::uintptr_t animator{};bool present{};
 if(!visual.generic_animator_identity_v21(animator,present,e))return false;
 if(group>=std::uint32_t(present?1:0)){e.clear();return true;}
 // Native startup ignores accepted PlayClip return. Delivery still executes
 // the SAME source kernel/timeline/clip selection/NewAnim/flags200 stores.
 // It performs no frame sample, OnAnimate, physics or second animation tick.
 bool accepted{};return visual.play(name,loop,accepted,e);
}
template<class Self,class Startup>
void bind_startup_idle_animation_v86(std::weak_ptr<Self> weak,Startup& output){
 if(output.play_idle_animation)return; // preserve genuine existing service
 output.play_idle_animation=[weak](auto& receiver,std::uintptr_t visual_id,const char* name,
  bool loop,std::int32_t priority,std::uint32_t group,std::string& e){
  auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!self||!self->borrow(receiver.base().identity(),pin,base,e)||base!=&receiver.base())return false;
  const auto slot=base->pointer(0x2d8);if(!slot||*slot!=visual_id){e="Required SAME startup VisualObject2d8";return false;}
  std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
  if(!self->actual_visual(visual_id,visual,e))return false;
  return retained_idle_animation_source_v86(*visual,name,loop,priority,group,e);
 };
}
}
