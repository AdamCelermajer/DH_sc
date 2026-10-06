#include "canonical_room_zone_v3.hpp"
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
}
