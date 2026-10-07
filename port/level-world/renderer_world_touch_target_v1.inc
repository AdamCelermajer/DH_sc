// Include after world_click_target_v1.hpp/world_touch_projection_v1.hpp. Root
// supplies real input/controller/viewport services and canonical fields.
namespace dh2::world {
struct RendererWorldTouchTargetV1 {
 WorldClickFieldsV1 fields;
 WorldClickServicesV1 click;
 WorldTouchProjectionServicesV1 projection;
 // Whole source Cmd_Click4051a8 gate over actual controller override9,
 // locked8 and Application controller suspension flag. accepted=false skips.
 std::function<bool(bool&,std::string&)> controller_click_allowed;
 bool dispatch(float scene_x,float scene_y,bool released,bool& floor_hit,std::string& error){
  std::array<float,3> point{};
  if(!world_touch_projection_v1(scene_x,scene_y,projection,floor_hit,point,error))return false;
  if(!floor_hit)return true;
  if(!controller_click_allowed){error="required actual Cmd_Click controller gate";return false;}
  bool allowed=false;if(!controller_click_allowed(allowed,error))return false;if(!allowed)return true;
  return world_click_target_v1(fields,point,released,click,error);
 }
};
}
