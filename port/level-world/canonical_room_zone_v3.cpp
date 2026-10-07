#include "canonical_room_zone_v3.hpp"
#include <exception>
#include <algorithm>
namespace dh2::world {
CanonicalRoomZoneV3::CanonicalRoomZoneV3(std::shared_ptr<void> pin,actor::RuntimeState& runtime,RoomZoneServicesV3 services):
 base_(reinterpret_cast<std::uintptr_t>(this),11,std::move(pin),runtime),services_(std::move(services)),initialization_(base_,services_.game_object){
 base_.lifecycle().static84=1;
 // Modern safety repair, not original constructor parity. RoomZone's empty
 // DeclareProperties never supplies ObjectBase byte87, although the manager
 // reads it during room lookup. Initialize only this receiver room-local;
 // use the same base field producer, leaving source Add/default order intact.
 std::string initialization_error;
 base_.store_byte(0x87,0,initialization_error);
}
bool CanonicalRoomZoneV3::init_post(std::string& error){
 error.clear();const auto& g=services_.game_object;
 if(!g.owner||!g.check_spawn_probability){error="Required actual RoomZone CheckSpawnProbability";return false;}
 std::int32_t roll;if(!g.check_spawn_probability(roll,error))return false;
 if(roll>=*base_.integer(0x274))return true;
 bool eligible=false;if(!initialization_.init_post(eligible,error))return false;
 // Original Zone continues after the void base call, with no extra gate.
 const auto* scale=base_.vector3(0x120);
 for(unsigned i=0;i<3;++i){volatile float scaled=dimensions_[i]*scale[i];dimensions_[i]=scaled;volatile float half=scaled*.5f;base_.relative_aabb144()[i]=-half;base_.relative_aabb144()[i+3]=half;}
 if(!game_object_relative_box_v3(base_,base_.relative_aabb144(),g.update_pf_object,error))return false;
 if(physical380_){error="Required actual physical Zone InitPost continuation";return false;}
 if(node384_)return true;
 if(!*base_.pointer(0x2d8))return true;
 error="Required actual Zone visual trigger-node lookup/mesh continuation";return false;
}
bool CanonicalRoomZoneV3::init_bounds(const std::array<float,6>& box,std::uintptr_t module,std::string& error){
 error.clear();for(unsigned i=0;i<3;++i){volatile float size=box[i+3]-box[i];dimensions_[i]=size;}
 std::copy(box.begin(),box.end(),base_.relative_aabb144());
 float center[3];for(unsigned i=0;i<3;++i){volatile float sum=box[i]+box[i+3];center[i]=sum*.5f;}
 if(!game_object_set_position_v2(base_,center,true,services_.position,error))return false;
 if(physical380_){error="Required actual physical Zone InitBounds continuation";return false;}
 // Module::InitPost writes module38c only after successful InitBounds.
 module38c_=module;return true;
}
bool CanonicalRoomZoneV3::bind_runtime_v104(RoomZoneRuntimeServicesV104 services,std::string& e){
 if(runtime_v104_.owner||!services.owner){e="Required once-only native RoomZone runtime authority";return false;}
 runtime_v104_=std::move(services);e.clear();return true;
}
bool CanonicalRoomZoneV3::source_add_object_v104(std::uintptr_t id,std::string& e){
 //39672c: uniqueness is local to this list, including a genuine NULL entry.
 if(std::find(occupants_.begin(),occupants_.end(),id)==occupants_.end())occupants_.push_back(id);
 e.clear();return true;
}
void CanonicalRoomZoneV3::source_remove_object_v104(std::uintptr_t id)noexcept{
 //3968cc removes only the first matching list node.
 auto i=std::find(occupants_.begin(),occupants_.end(),id);if(i!=occupants_.end())occupants_.erase(i);
}
bool CanonicalRoomZoneV3::source_has_inside_v104(const std::array<float,3>& p)const noexcept{
 const auto* b=base_.absolute_aabb12c();
 return b[0]<=p[0]&&p[0]<=b[3]&&b[1]<=p[1]&&p[1]<=b[4]; //3964cc, inclusive XY only
}
bool CanonicalRoomZoneV3::source_add_initial_object_v104(std::uintptr_t id,bool& accepted,std::string& e){
 accepted=false;if(!id){e.clear();return true;}
 const auto& s=runtime_v104_;RoomObjectBorrowV104 object;
 if(!s.owner||!s.object||!s.object(id,object,e)||!object.owner||object.identity!=id||!object.is_zonable_c4){if(e.empty())e="Required actual initial RoomZone object";return false;}
 bool zonable{};if(!object.is_zonable_c4(zonable,e))return false;if(!zonable){e.clear();return true;}
 if(!object.position160){e="Required actual initial object position160";return false;}
 const std::array<float,3> position{object.position160[0],object.position160[1],object.position160[2]};
 if(!source_has_inside_v104(position)){e.clear();return true;}
 if(!object.initial_room2ef||!object.room2f4||!s.trace){e="Required actual initial room2ef/2f4 and Debug transport";return false;}
 const bool previously_assigned=*object.initial_room2ef!=0;
 if(!s.trace("isTracingRoomZoneInit",e))return false;
 if(previously_assigned&&*object.room2f4){
  if(!s.remove_from_room||!s.remove_from_room(*object.room2f4,id,e))return false;
 }
 *object.room2f4=base_.identity();*object.initial_room2ef=1;
 if(!s.zone_transition||!s.zone_transition(object,true,e))return false;
 //396bc0 appends unconditionally after ZoneEntered, unlike AddObject.
 occupants_.push_back(id);accepted=true;e.clear();return true;
}
bool CanonicalRoomZoneV3::source_activate_v104(bool active,std::string& e){
 //396d24/396918 operate on the live list; Debug calls remain per occupant.
 const auto& s=runtime_v104_;
 if(!s.owner||!s.trace||!s.trace("isTracingRoomZoneTransition",e))return false;
 for(auto i=occupants_.begin();i!=occupants_.end();++i){
  if(*i&&(!s.occupant_transition||!s.occupant_transition(*i,active,e)))return false;
  if(!s.trace("isTracingRoomZoneTransitionFull",e))return false;
 }
 if(!s.room_objects){e="Required actual ObjectManager Add/DelRoomObjects";return false;}
 return s.room_objects(occupants_,active,e);
}
bool CanonicalRoomZoneV3::source_init_object_list_v104(CanonicalObjectManagerV1& manager,
 const std::function<bool(const CanonicalObjectBorrowV1&,std::uintptr_t&,std::string&)>& game_object,std::string& e){
 if(local390_){e.clear();return true;}
 std::int32_t key{};const CanonicalObjectBorrowV1* object{};
 bool more=manager.source_ordered_begin_v38(key,object);
 while(more){
  std::uintptr_t id{};
  if(object&&object->identity){
   auto actual=*object; //pins the existing object through native handle/cast calls
   if(!game_object||!game_object(actual,id,e))return false;
  }
  bool accepted{};if(!source_add_initial_object_v104(id,accepted,e))return false;
  more=manager.source_ordered_next_v38(key,key,object);
 }
 local390_=1;e.clear();return true;
}
bool CanonicalRoomZoneV3::source_has_been_visited_v104(bool& visited,std::string& e){
 if(!module38c_){visited=true;e.clear();return true;}
 std::uint8_t* byte{};std::shared_ptr<void> pin;
 if(!runtime_v104_.module_visited||!runtime_v104_.module_visited(module38c_,byte,pin,e)||!pin||!byte){if(e.empty())e="Required actual Module3fc visited cell";return false;}
 visited=*byte!=0;e.clear();return true;
}
bool CanonicalRoomZoneV3::source_set_visited_v104(bool visited,std::string& e){
 return source_set_visited_byte_v104(visited?1:0,e);
}
bool CanonicalRoomZoneV3::source_set_visited_byte_v104(std::uint8_t visited,std::string& e){
 if(!module38c_){e.clear();return true;}
 std::uint8_t* byte{};std::shared_ptr<void> pin;
 if(!runtime_v104_.module_visited||!runtime_v104_.module_visited(module38c_,byte,pin,e)||!pin||!byte){if(e.empty())e="Required actual Module3fc visited cell";return false;}
 *byte=visited;e.clear();return true;
}
bool CanonicalRoomZoneV3::source_update_v104(std::string& e){
 const auto& s=runtime_v104_;std::array<float,6> bounds;
 std::copy_n(base_.absolute_aabb12c(),6,bounds.begin()); //snapshot BEFORE current-Level/camera call
 std::array<std::array<float,4>,6> planes;
 if(!s.owner||!s.camera_planes||!s.camera_planes(planes,e))return false;
 for(const auto& plane:planes){
  const float x=plane[0]>=0.f?bounds[0]:bounds[3];
  const float y=plane[1]>=0.f?bounds[1]:bounds[4];
  const float z=plane[2]>=0.f?bounds[2]:bounds[5];
  const float distance=((plane[0]*x+plane[1]*y)+plane[2]*z)+plane[3];
  if(distance>0.f){
   if((local389_||local388_)&&!source_activate_v104(false,e))return false;
   local389_=0;local388_=0;e.clear();return true;
  }
 }
 if((!local389_||local388_)&&!source_activate_v104(true,e))return false;
 local389_=1;
 if(!s.count_visible_room||!s.count_visible_room(e))return false;
 bool visited{};if(!source_has_been_visited_v104(visited,e))return false;
 if(!visited){
  std::array<float,3> position;bool character_present{};
  if(!s.local_player_position||!s.local_player_position(position,character_present,e))return false;
  if(character_present&&source_has_inside_v104(position)&&!source_set_visited_v104(true,e))return false;
 }
 local388_=0;e.clear();return true;
}
}

namespace dh2::world {
bool CanonicalRoomZoneV3::source_destroy_v106(const RoomDestroyServicesV106& s,std::string& e){
 if(destroy_complete_v106_){e.clear();return true;}
 if(destroy_attempted_v106_){e=destroy_failure_v106_.empty()?"RoomZone D1 cannot replay a reached prefix":destroy_failure_v106_;return false;}
 if(!s.owner||!s.game_object_d2||(node384_&&!s.node384_drop)){e="Required actual Room/Zone node drop and qualified GameObjectD2 owners";return false;}
 destroy_attempted_v106_=true;const auto fail=[&](){destroy_failure_v106_=e.empty()?"Reached RoomZone source D1 failed":e;e=destroy_failure_v106_;return false;};
 try{
  //396828: container free/reset only, no Deactivate or occupant callbacks.
  occupants_.clear();
  //397bc4/397b38: reference drop before source384 NULL store.
  if(node384_){const auto actual=node384_;if(!s.node384_drop(actual,e))return fail();
   if(node384_!=actual){e="Zone node384 changed during captured reference drop";return fail();}node384_=0;}
  if(!s.game_object_d2(base_,e))return fail();destroy_complete_v106_=true;e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return fail();}catch(...){e="Actual RoomZone D1 provider threw";return fail();}
}
}
