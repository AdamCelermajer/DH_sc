#include "loot_item_frame_source_v47.hpp"
#include <algorithm>
namespace dh2::character {
bool LootItemFrameSourceBridgeV47::bind(WorldItemFrameServicesV5 actual,WorldItemFrameServicesV5& out,std::string& e){
 auto& item=graph_.receiver_v4();auto& base=item.base();
 const auto* auxiliary=base.pointer(0x2e0);const auto* boundary=base.byte(0x1c4);
 if(!actual.geometry||!actual.paths||!actual.obstacles||!actual.motion||!actual.workspace||!actual.roots||
    !auxiliary||!boundary){e="Required SAME Item floor/PF/path/workspace/scene/source fields";return false;}
 // Item InitOnce/InitAgain never attach this pointer. A positive attachment
 // must borrow its type/mode/Update receiver; never treat it as absent.
 if(*auxiliary||*boundary||item.runtime().controller.validate_boundary){
  if(!actual.actual_runtime_policy){e="Required positive Item auxiliary/boundary source policy";return false;}
  policy_=*actual.actual_runtime_policy;
 }else{
  // All virtual fields are overwritten by item_frame_virtual_policy_v4.
  // These unused auxiliary/boundary facts are the actual NULL/zero source
  // constructor fields, not a missing camera/global service fallback.
  policy_={};policy_.virtual_speed=item.fields().speed3b0;
 }
 preceding_=actual.actual_world;actual.actual_world={this,world};
 actual.actual_runtime_policy=&policy_;out=std::move(actual);return true;
}
std::uint32_t LootItemFrameSourceBridgeV47::world(void* raw,std::uint32_t event,float* payload){
 auto& self=*static_cast<LootItemFrameSourceBridgeV47*>(raw);auto& item=self.graph_.receiver_v4();
 if(event==subobjects::get_speed){
  if(!payload){self.error_="Required Item GetSpeed output";return ~0u;}
  *payload=item.fields().speed3b0;return 1;
 }
 if(event==subobjects::visual_sync_scaling){
  const auto* field=item.base().pointer(0x2d8);
  if(!field){self.error_="Required Item visual2d8 source field";return ~0u;}
  if(!*field)return 1; // actual NULL VisualObject, source call skipped
  auto visual=self.graph_.visual().visual();const auto* scale=item.base().vector3(0x120);
  if(!visual||!scale||visual->root_identity()==0){
   self.error_="Required SAME Item VisualObject/root/scale120";return ~0u;
  }
  if(*field!=reinterpret_cast<std::uintptr_t>(visual.get())){
   self.error_="Item visual2d8 differs from retained VisualObject";return ~0u;
  }
  std::copy_n(scale,3,visual->binding().root.scale);
  return visual->binding().update_world(visual->scene(),self.error_)?1u:~0u;
 }
 if(!self.preceding_.invoke){
  self.error_="Required reached source Item world event "+std::to_string(event);return ~0u;
 }
 return self.preceding_.invoke(self.preceding_.context,event,payload);
}
}

