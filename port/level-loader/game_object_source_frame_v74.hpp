#pragma once
#include <canonical_gameobject_base_owner_v1.hpp>
#include <gameobject_update_prefix_v23.hpp>
#include <gameobject_online_update_v5.hpp>
namespace dh2::loader {
struct GameObjectRuntimeBorrowV74 {
 // Read-only scoped lending of already existing native resources/policies.
 // This pin retains their real World/scene/physics/clock storage for the call.
 std::shared_ptr<void> actual_scope;
 actor::GenericRuntimeRequestV4 request;
};
struct GameObjectSourceFrameServicesV74 {
 std::shared_ptr<void> owner; // independent source service authority
 world::GameObjectUpdatePrefixServicesV23 prefix;
 std::function<bool(world::CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,std::string&)> collision_interact;
 // Supplies actual virtual policy, physics/native-body, generic visual root,
 // current dt, navigation/workspace, target node position and source services.
 // No C1, frame execution, world Step or scene OnAnimate occurs in this lender.
 std::function<bool(world::CanonicalGameObjectBaseOwnerV1&,GameObjectRuntimeBorrowV74&,std::string&)> lend_runtime;
 world::GameObjectOnlineUpdateServicesV5 online;
 std::function<bool(world::CanonicalGameObjectBaseOwnerV1&,std::int16_t,std::string&)> idle_sound;
};
// Whole GameObject.Update38cbe8 source orchestration on THIS canonical base and
// its SAME RuntimeState, consuming existing recovered generic actor kernels.
// Animation time belongs to Main's actual Scene OnAnimate. Runtime source
// visual_update must NOT call RetainedGenericAnimator.frame/container.frame or
// legacy loot animation timing a second time.
inline bool game_object_source_frame_v74(world::CanonicalGameObjectBaseOwnerV1& base,
 const GameObjectSourceFrameServicesV74& services,std::string& e){
 if(!services.owner){e="Required actual GameObject frame service authority";return false;}
 if(!world::gameobject_update_begin_v23(services.prefix,e))return false;
 auto* collision=base.pointer(0x2e4);
 if(!collision){e="Required SAME GameObject collision target2e4 source cell";return false;}
 if(*collision){
  if(!services.collision_interact){e="Required actual GameObject Interact virtual98";return false;}
  if(!services.collision_interact(base,*collision,e))return false;
  *collision=0;
 }
 GameObjectRuntimeBorrowV74 runtime;
 if(!services.lend_runtime||!services.lend_runtime(base,runtime,e)){
  if(e.empty())e="Required actual scoped generic GameObject runtime resources";return false;
 }
 auto& request=runtime.request;auto& frame=request.actor;
 if(!runtime.actual_scope||(frame.state&&frame.state!=&base.runtime())||
    (frame.key&&frame.key!=base.identity())||frame.binding||frame.scene){
  e="Generic frame replaced SAME canonical runtime/identity or borrowed Character scene";return false;
 }
 const auto* physical=base.pointer(0x2dc);
 if(!physical||bool(*physical)!=bool(frame.native_body)){
  e="Required actual native-body facet for SAME GameObject physical2dc presence";return false;
 }
 // The caller supplies genuine engine views; only loader-owned source
 // receiver identity and runtime are assigned here. No virtual values, time,
 // geometry, destination or field policy are fabricated from missing inputs.
 frame.state=&base.runtime();frame.key=base.identity();
 actor::RuntimeResult result{};
 const auto status=actor::update_gameobject_v4(result,request,e);
 if(status){if(e.empty())e="Actual generic GameObject source frame failed at phase "+std::to_string(result.phase);return false;}
 if(!world::gameobject_require_online_update_v5(base,services.online,e))return false;
 const auto* value=base.integer(0x370);
 if(!value){e="Required SAME GameObject signed-short idle sound370";return false;}
 const auto sound=static_cast<std::int16_t>(static_cast<std::uint16_t>(*value));
 if(sound>=0){
  if(!services.idle_sound){e="Required actual GameObject PlaySound38ae2c";return false;}
  if(!services.idle_sound(base,sound,e))return false;
 }
 return world::gameobject_update_end_v23(e); // original profiling Pop is BX LR
}
}
