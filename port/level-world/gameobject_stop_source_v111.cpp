#include "gameobject_stop_source_v111.hpp"
#include <cstring>
namespace dh2::world {
bool gameobject_stop_source_v111(CanonicalGameObjectBaseOwnerV1& base,
 const GameObjectStopServicesV111& services,std::string& e){
 auto& runtime=base.runtime();
 // The same retained PFObject path owns its original temporary edge and
 // DropPath storage. Failure retains its reached prefix rather than resetting.
 if(dh2_nav_drop_path(&runtime.path)){e="Required actual PFObject DropPath in GameObject.Stop";return false;}
 const auto position=base.vector3(0x160),destination=base.vector3(0x1a8);
 auto requested=base.byte(0x1b4),validating=base.byte(0x1b5);
 if(!position||!destination||!requested||!validating){e="Required SAME GameObject.Stop source fields";return false;}
 std::memcpy(destination,position,12);
 *requested=0;*validating=0;
 for(auto& value:runtime.controller.heading.direction)value=0.f; //actual Point3D::zero
 // Native projections consumed by the existing path/update backend.
 runtime.controller.path_requested=0;runtime.controller.validate_boundary=0;
 runtime.controller.heading.active=0;runtime.subobjects.path_count=runtime.path.count;
 std::memcpy(runtime.subobjects.destination,destination,12);
 auto physical_slot=base.pointer(0x2dc);
 if(!physical_slot){e="Required actual GameObject physical2dc cell";return false;}
 if(!*physical_slot){e.clear();return true;}
 bool updating{};
 if(!services.updating_position_from_physics64||
    !services.updating_position_from_physics64(updating,e))return false;
 if(!updating){e.clear();return true;}
 std::shared_ptr<void> pin;physical::NativeBody* body{};
 if(!services.physical||!services.physical(pin,body,e)||!pin||!body||!body->body){
  if(e.empty())e="Required SAME physical receiver for GameObject.Stop";return false;
 }
 // Source linear0→angular0→SetPosition(currentXY)→PutToSleep. The existing
 // native Stop preserves the ignored SetXForm result and clears real forces.
 if(dh2_native_body_stop(body,position)){e="GameObject.Stop native physical delivery failed";return false;}
 if(dh2_native_body_refresh_view(&runtime.body,body)){e="GameObject.Stop native body observation failed";return false;}
 dh2_physical_stop_finish(&runtime.body);e.clear();return true;
}
}
