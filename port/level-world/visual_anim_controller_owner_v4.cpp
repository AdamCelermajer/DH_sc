#include "visual_anim_controller_owner_v4.hpp"
namespace dh2::world {
namespace {std::uint8_t source_scaling_enabled_v99=1;}
std::uint8_t source_animation_scaling_enabled_v99() noexcept{return source_scaling_enabled_v99;}
void source_store_animation_scaling_v99(std::uint8_t value) noexcept{source_scaling_enabled_v99=value;}
bool VisualAnimControllerOwnerV4::construct(const std::shared_ptr<void>& root,std::uintptr_t identity,
 VisualAnimControllerRootServicesV4 services,bool remove,std::string& error){
 if(attempted_){error="AnimController constructor already attempted";return false;}attempted_=true;
 if(!root||!identity||root.get()!=reinterpret_cast<void*>(identity)){
  error="Required same actual RootSceneNode allocation for AnimController";return false;
 }
 // Source474d30 stores root4 then grabs BEFORE remove/callback continuation.
 root_identity_=identity;root_=root;services_=services;
 if(remove){if(!services_.remove_animators){error="Required actual root removeAnimators";return false;}
  return services_.remove_animators(services_.context,error);
 }
 return set_callbacks_on_all(do_nothing_timeline,this,do_nothing_event,this,error);
}
bool VisualAnimControllerOwnerV4::set_callbacks_on_all(VisualTimelineCallbackV4 complete,void* complete_user,
 VisualEventCallbackV4 event,void* event_user,std::string& error){
 if(!root_||!services_.get_animators){error="Required actual root getAnimators";return false;}
 const std::vector<VisualAnimatorBorrowV4>* animators{};
 if(!services_.get_animators(services_.context,animators,error))return false;
 if(!animators){error="Required same root animator list";return false;}
 for(const auto& animator:*animators){
  // Whole SetCallbacks474c78 ignores NULL receivers, then invokes the actual
  // AnimApplicator successor366330. An unknown nonnull owner is required.
  if(!animator.identity)continue;
  if(!animator.set_callbacks){error="Required actual animator SetCallbacks";return false;}
  if(!animator.set_callbacks(animator.context,complete,complete_user,event,event_user,error))return false;
 }
 return true;
}
}
