#include "source_summon.hpp"

#include "../../../script-runtime/script_object_bridge.h"

#include <cstdio>
#include <cstring>

namespace dh2::enemy_ai {
namespace {
bool report(char* error,std::size_t capacity,const std::string& message){
 if(error&&capacity)std::snprintf(error,capacity,"%s",message.c_str());return false;
}
int required_failure(char* error,std::size_t capacity,const std::string& message){
 report(error,capacity,message);return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
bool required(bool delivered,const std::string& error,char* output,std::size_t capacity,const char* fallback){
 return delivered?true:report(output,capacity,error.empty()?fallback:error);
}
std::uint32_t source_unsigned(const dh2_script_value& value){
 std::uint32_t bits{};std::memcpy(&bits,&value.number,sizeof(bits));
 const auto exponent=(bits>>23)&255u;
 // Same ARM Value.getUnsigned float conversion used by source RegisterSummon.
 if((bits>>31)||exponent<127u||(exponent==255u&&(bits&0x7fffffu)))return 0;
 if(exponent>158u)return UINT32_MAX;
 return ((bits<<8)|0x80000000u)>>(158u-exponent);
}
std::int32_t source_signed(float value){
 std::uint32_t bits{};std::memcpy(&bits,&value,sizeof(bits));const auto exponent=(bits>>23)&255u;
 if((exponent==255u&&(bits&0x7fffffu))||exponent<127u)return 0;
 if(exponent>=158u)return bits>>31?INT32_MIN:INT32_MAX;
 const auto mantissa=(bits&0x7fffffu)|0x800000u;
 const auto integer=exponent>=150u?mantissa<<(exponent-150u):mantissa>>(150u-exponent);
 return bits>>31?-static_cast<std::int32_t>(integer):static_cast<std::int32_t>(integer);
}
bool as_bool(const dh2_script_value& value){return value.type==DH2_SCRIPT_BOOLEAN&&value.boolean!=0;}
bool same_receiver(const world::CanonicalClassReceiverV1& receiver,
 std::uintptr_t identity,const std::shared_ptr<void>& lease){
 return identity&&receiver.object.identity==identity&&receiver.object.lease&&lease&&
  receiver.object.lease.get()==lease.get()&&
  !receiver.object.lease.owner_before(lease)&&!lease.owner_before(receiver.object.lease);
}
}

int source_summon_callback_v1(void* opaque,const dh2_script_value* arguments,std::uint32_t count,
 dh2_script_value* results,std::uint32_t capacity,std::uint32_t* returned,
 char* error,std::size_t error_capacity){
 if(!returned||(count&&!arguments))return required_failure(error,error_capacity,"Malformed source _Summon callback");
 *returned=0;
 // Original wrapper checks the first numeric CharacterTable value before any
 // world/provider access. Preserve its ordinary ignored-invalid argument path.
 if(count<2||arguments[0].type!=DH2_SCRIPT_NUMBER)return 0;
 if(!opaque)return required_failure(error,error_capacity,"Required same source _Summon services owner");
 auto& services=*static_cast<SourceSummonServicesV1*>(opaque);
 if(!services.characters||!services.owner.lease||!services.owner.identity||
    !services.owner.position160||!services.owner.rotation16c)
  return required_failure(error,error_capacity,"Required same caller GameObject/Character position, rotation and lease");
 const auto id=source_unsigned(arguments[0]);
 if(id>=services.characters->names.size()||id>=services.characters->rows.size())return 0;
 const bool set_spawn_state=as_bool(arguments[1]);
 std::int32_t animation=0;
 std::array<float,3> position{services.owner.position160[0],services.owner.position160[1],services.owner.position160[2]};
 std::array<float,3> rotation{services.owner.rotation16c[0],services.owner.rotation16c[1],services.owner.rotation16c[2]};

 if(count==3&&arguments[2].type==DH2_SCRIPT_NUMBER){
  // The 3-argument overload treats its third number as a positive animation
  // index, not a one-axis position offset.
  animation=source_signed(arguments[2].number);
 }else if(count==3&&arguments[2].type==DH2_SCRIPT_SOURCE_OBJECT){
  if(!services.source_object_pose)return required_failure(error,error_capacity,"Required source-object pose resolver for _Summon overload");
  std::string pose_error;
  if(!required(services.source_object_pose(arguments[2].identity,position,rotation,pose_error),pose_error,error,error_capacity,"Required source-object pose resolver"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }else if(count==5||count==6){
  if(arguments[2].type!=DH2_SCRIPT_NUMBER||arguments[3].type!=DH2_SCRIPT_NUMBER||arguments[4].type!=DH2_SCRIPT_NUMBER)return 0;
  const std::array<float,3> offset{arguments[2].number,arguments[3].number,arguments[4].number};
  const bool orientation_relative=count==6&&as_bool(arguments[5]);
  if(!services.source_position_from_offsets)return required_failure(error,error_capacity,"Required source GetLookAtVec/offset position transform");
  std::string position_error;
  if(!required(services.source_position_from_offsets(services.owner.identity,
      {services.owner.position160[0],services.owner.position160[1],services.owner.position160[2]},
      {services.owner.rotation16c[0],services.owner.rotation16c[1],services.owner.rotation16c[2]},
      offset,orientation_relative,position,position_error),position_error,error,error_capacity,
      "Required source GetLookAtVec/offset position transform"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }else if(count!=2){
  return 0;
 }

 if(!services.create_npc)return required_failure(error,error_capacity,"Required same canonical Character::CreateNPC factory service");
 SourceCreateNpcResultV1 created;std::string service_error;
 if(!services.create_npc(static_cast<std::int32_t>(id),created,service_error)){
  return required_failure(error,error_capacity,service_error.empty()?"Source Character::CreateNPC failed":service_error);
 }
 if(!created.created)return 0; // The source ObjectHandle-to-Character cast was NULL.
 auto& receiver=created.receiver;const auto identity=receiver.object.identity;const auto lease=receiver.object.lease;
 if(!identity||!lease||!same_receiver(receiver,identity,lease))
  return required_failure(error,error_capacity,"Required same source-created Character receiver lease");
 if(!services.set_initial_position||!required(services.set_initial_position(receiver,position,service_error),service_error,error,error_capacity,"Required same Character SetInitialPosition/PFWorld"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 if(!same_receiver(receiver,identity,lease))return required_failure(error,error_capacity,"SetInitialPosition changed the source Character receiver");
 if(!services.set_position||!required(services.set_position(receiver,position,true,service_error),service_error,error,error_capacity,"Required same Character SetPosition/physical/visual"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 if(!same_receiver(receiver,identity,lease))return required_failure(error,error_capacity,"SetPosition changed the source Character receiver");
 if(!services.set_rotation||!required(services.set_rotation(receiver,rotation,service_error),service_error,error,error_capacity,"Required same Character SetRotation"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 if(!same_receiver(receiver,identity,lease))return required_failure(error,error_capacity,"SetRotation changed the source Character receiver");
 if(!services.mark_summoned||!required(services.mark_summoned(receiver,service_error),service_error,error,error_capacity,"Required source Character summoned byte+0x14e4"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;

 bool room_accepted=false;
 if(services.owner.room){
  if(!services.room_add_initial_object||!required(services.room_add_initial_object(services.owner.room,identity,room_accepted,service_error),service_error,error,error_capacity,"Required same caller RoomZone::AddInitialObject"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }
 if(!room_accepted){
  if(!services.manager_add_no_room||!required(services.manager_add_no_room(receiver,service_error),service_error,error,error_capacity,"Required same ObjectManager::AddNoRoomObject"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
  if(!services.zone_entered||!required(services.zone_entered(receiver,service_error),service_error,error,error_capacity,"Required same Character ZoneEntered"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }
 if(set_spawn_state){
  if(!services.set_spawn_state||!required(services.set_spawn_state(receiver,false,false,service_error),service_error,error,error_capacity,"Required same CharStateMachine::SM_SetSpawnState"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }

 // Source ReturnValues.pushUserData occurs before animation and the online
 // tail. On a later required-provider failure Lua still observes the exact
 // executed native prefix through the protected callback error.
 if(!results||!capacity)return required_failure(error,error_capacity,"Required source _Summon ReturnValues capacity");
 results[0]={};results[0].type=DH2_SCRIPT_IDENTITY;results[0].identity=identity;*returned=1;
 if(animation>0){
  if(!services.forward_animation||!required(services.forward_animation(receiver,animation,service_error),service_error,error,error_capacity,"Required same Character VisualObject::ForwardAnim"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }
 bool online=false;
 if(!services.online_byte5||!required(services.online_byte5(online,service_error),service_error,error,error_capacity,"Required same Application GetOnline byte5"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 if(online){
  if(!services.assign_network_id||!required(services.assign_network_id(receiver,service_error),service_error,error,error_capacity,"Required same ObjectManager::AssignObjectNetworkId"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
  if(!services.online_post_assignment||!required(services.online_post_assignment(receiver,services.owner.identity,service_error),service_error,error,error_capacity,"Required source _Summon online fields/OnlineGameState continuation"))return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
 }
 return 0;
}

} // namespace dh2::enemy_ai
