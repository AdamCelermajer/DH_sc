#include "original_actor_subobjects.hpp"
#include "../level-world/move_state.hpp"
#include "../level-world/native_body.hpp"
#include <cstring>
#include <cmath>
#include <stdexcept>
namespace dh::foundation {
namespace {
struct Failure : std::runtime_error {using std::runtime_error::runtime_error;};
struct Bridge {
 const OriginalActorSubobjectsInput& input;OriginalActorSubobjectsResult& result;std::string& error;
 dh2::subobjects::State state{};dh2::subobjects::Policy policy{};
 dh2::physical::BodyState body{};dh2::physical::TransformRequest transform{};
 dh2::physical::NativeSubobjectsBridge native{};
 std::uintptr_t physical_identity;
 Bridge(const OriginalActorSubobjectsInput& i,OriginalActorSubobjectsResult& r,std::string& e):input(i),result(r),error(e),physical_identity(*i.owner.physical2dc){native={i.owner.native,&body,&transform,nullptr,nullptr};}
 [[noreturn]] void fail(std::uint32_t event,const std::string& reason){result.failed_event=event;error=reason;throw Failure(reason);}
 void publish(){std::memcpy(input.owner.position160,state.position,12);input.actor->transform.rotation[2]=state.rotation;std::memcpy(input.owner.destination1a8,state.destination,12);std::memcpy(input.owner.absolute12c,state.absolute_bounds,24);}
 void reload(){
  if(*input.owner.physical2dc!=physical_identity)fail(result.failed_event,"Source physical assignment changed during bounded SubObjects invocation");
  std::memcpy(state.position,input.owner.position160,12);std::memcpy(state.destination,input.owner.destination1a8,12);state.rotation=input.actor->transform.rotation[2];std::memcpy(state.local_bounds,input.owner.relative144,24);std::memcpy(state.absolute_bounds,input.owner.absolute12c,24);std::memcpy(state.previous_position,input.pf->motion.position,12);state.path_count=input.path->count;std::memcpy(state.path_target,input.path->target,12);
 }
 OriginalActorSubobjectsVisual visual(std::uint32_t event){
  const auto id=*input.owner.visual2d8;OriginalActorSubobjectsVisual value;
  if(!id||!input.visual||!input.visual(id,value,error)||!value.receiver||value.identity!=id||!value.position_ac)fail(event,error.empty()?"Required actual leased SAME visual root position/identity":error);
  return value;
 }
 void absolute(OriginalActorSubobjectsVisual& value,std::uint32_t event){if(!value.update_absolute||!value.update_absolute(error))fail(event,error.empty()?"Required actual visual absolute-transform publication":error);}
 std::uint32_t invoke(std::uint32_t event,float* payload){
  using namespace dh2::subobjects;publish();result.failed_event=event;std::uint32_t value=1;
  switch(event){
   case physical_update:case physical_set_velocity:case physical_wake:case apply_body_transform:{
    if(!input.owner.native||!input.owner.native->body)fail(event,"Reached native service without SAME assigned native body");
    if(payload)for(unsigned k=0;k<(event==apply_body_transform?3u:2u);++k)if(!std::isfinite(payload[k]))fail(event,"Nonfinite native subobject service payload");
    value=dh2_native_body_subobject_service(&native,event,payload);
    if(event!=apply_body_transform&&!value)fail(event,"Actual native SubObjects backend rejected service");
    // Source SetXForm callers ignore frozen/outside-world false; no success
    // substituted for that backend result and no solver/Step invoked.
    break;
   }
   case validate_position:{
    if(!payload||!input.floor_lease||!input.obstacle_lease||!input.floors||!input.floors->sewn||!input.obstacles)fail(event,"Reached floor validation without SAME actual floor/PF/registry leases");
    dh2::navigation::PositionResult admission{};const dh2::navigation::ObjectPositionRequest request{&input.floors->collision_world,input.obstacles,input.pf,input.owner.identity,payload,&input.floors->source_motion_policy_v95};
    const auto status=dh2_nav_validate_object_position(&admission,&request);if(status)fail(event,"Actual source object floor validation failed: "+std::to_string(status));
    // payload aliases kernel state.position; publish accepted OR clamped words.
    std::memcpy(input.owner.position160,payload,12);value=admission.valid;break;
   }
   case visual_update:{auto v=visual(event);if(!v.update||!v.update(error))fail(event,error.empty()?"Reached actual VisualObject.Update service unbound":error);break;}
   case visual_apply_position:{auto v=visual(event);if(!input.set_position)fail(event,"Required SAME source GameObject.SetPosition for Visual.ApplyPosition");std::array<float,3> point;std::memcpy(point.data(),v.position_ac,12);if(!input.set_position(point,false,error))fail(event,error.empty()?"Actual visual ApplyPosition source setter failed":error);break;}
   case visual_sync_position:{auto v=visual(event);std::memcpy(v.position_ac,input.owner.position160,12);absolute(v,event);break;}
   case visual_apply_rotation:{auto v=visual(event);if(!v.apply_rotation||!v.apply_rotation(*input.actor,error))fail(event,error.empty()?"Reached actual visual rotation-apply service unbound":error);break;}
   case visual_sync_rotation:{auto v=visual(event);if(!v.sync_rotation||!v.sync_rotation(*input.actor,error))fail(event,error.empty()?"Reached actual visual rotation synchronization unbound":error);break;}
   case visual_sync_scaling:{auto v=visual(event);if(!v.sync_scaling||!v.sync_scaling(*input.actor,error))fail(event,error.empty()?"Reached actual visual scaling synchronization unbound":error);break;}
   case auxiliary_update:{const auto id=*input.owner.attached2e0;if(!id||!input.auxiliary_update||!input.auxiliary_update(id,error))fail(event,error.empty()?"Reached actual attached auxiliary update unbound":error);break;}
   case camera_get:case camera_can_move:case camera_position:case camera_set_free:{if(!input.camera||!input.camera(static_cast<Event>(event),payload,value,error))fail(event,error.empty()?"Reached actual camera service unbound":error);break;}
   case get_speed:{if(!input.virtual_speed)fail(event,"Reached virtual GetSpeed fact unbound");if(payload)*payload=*input.virtual_speed;break;}
   default:fail(event,"Unknown source SubObjects event");
  }
  ++result.completed_events;reload();result.failed_event=0;return value;
 }
 static std::uint32_t service(void* context,std::uint32_t event,float* payload){return static_cast<Bridge*>(context)->invoke(event,payload);}
 static int auxiliary(void* context,dh2::subobjects::AuxiliaryBranchV69* out){auto& self=*static_cast<Bridge*>(context);const auto id=*self.input.owner.attached2e0;if(!out||!id||!self.input.auxiliary_branch||!self.input.auxiliary_branch(id,*out,self.error))self.fail(dh2::subobjects::auxiliary_update,self.error.empty()?"Required actual auxiliary type/mode/position borrow":self.error);return 0;}
};
}
bool update_original_actor_subobjects(const OriginalActorSubobjectsInput& input,OriginalActorSubobjectsResult& output,std::string& error){
 const auto& owner=input.owner;
 if(!input.actor||!owner.owner_lease||!owner.identity||input.actor->id!=owner.identity||owner.position160!=input.actor->transform.position.data()||!owner.destination1a8||!owner.relative144||!owner.absolute12c||!owner.attached2e0||!owner.visual2d8||!owner.physical2dc||!input.navigation_lease||!input.path_lease||!input.pf||!input.path||bool(*owner.physical2dc)!=bool(owner.native)||(owner.native&&!owner.native->body)){error="SubObjects requires SAME actual actor/body slots/bounds/PF/path borrows";return false;}
 if(!input.actor->source_flags520){error="Actual Character.flags520 source policy unknown";return false;}
 dh2::move::Policy decoded{};const auto flags=*input.actor->source_flags520;if(dh2_move_policy(&decoded,&flags)){error="Actual Character SubObjects policy rejected";return false;}
 if(!decoded.validate_floor&&!input.validating_camera){error="Reached trailing floor branch requires actual IsValidatingCamera predicate";return false;}
 if(input.virtual_speed&&!std::isfinite(*input.virtual_speed)){error="Nonfinite actual virtual speed fact";return false;}
 output={};error.clear();Bridge bridge(input,output,error);bridge.policy={decoded.position_from_visual,decoded.position_from_physics,decoded.rotation_from_visual,decoded.rotation_from_physics,decoded.visual_with_rotation,decoded.validate_floor,unsigned(input.validating_camera.value_or(false)),unsigned(*owner.visual2d8!=0),unsigned(*owner.attached2e0!=0),0,0,0,input.virtual_speed.value_or(0)};
 try {
  bridge.reload();if(owner.native&&dh2_native_body_refresh_view(&bridge.body,owner.native))bridge.fail(dh2::subobjects::physical_update,"Actual native body observation failed");
  const dh2::subobjects::Services services{&bridge,Bridge::service};const dh2::subobjects::AuxiliaryBranchServicesV69 auxiliary{&bridge,Bridge::auxiliary};const dh2::subobjects::Request request{&bridge.state,owner.native?&bridge.body:nullptr,owner.native?&bridge.transform:nullptr,&bridge.policy,&services};
  const auto status=dh2_subobjects_update_source_v69(&output.source,&request,&auxiliary);if(status)bridge.fail(0,"Original SubObjects kernel rejected request: "+std::to_string(status));
  bridge.publish();output.completed=true;error.clear();return true;
 }catch(const std::exception& failure){if(error.empty())error=failure.what();return false;}
}
}
