#include "character_heading_owner_v1.hpp"
#include <cmath>
#include <cstring>
namespace dh2::character {
namespace {float word(std::uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
bool missing(std::string& error,const char* text){if(error.empty())error=text;return false;}
float squared(const float* v){return (v[0]*v[0]+v[1]*v[1])+v[2]*v[2];}}
bool CharacterHeadingOwnerV1::permitted()const noexcept{return command_.forced||(!command_.global_blocked&&!command_.locked);}
bool CharacterHeadingOwnerV1::command_head_towards(const float* direction,std::string& error){
 if(!permitted())return true;
 if(!command_.owner||!targets_.owner||command_.owner!=targets_.owner->identity||!direction)return missing(error,"Required same controllable Character heading owner");
 return character_head_towards(direction,error);
}
bool CharacterHeadingOwnerV1::command_stop(std::string& error){
 if(!permitted())return true;
 if(!command_.owner||!targets_.owner||command_.owner!=targets_.owner->identity)return missing(error,"Required same controllable Character Stop owner");
 return character_stop(error);
}
bool CharacterHeadingOwnerV1::character_head_towards(const float* direction,std::string& error){
 if(!direction)return missing(error,"Required Character heading direction");
 // Ctrl_HeadTowards 3adb60: full 3D length gate precedes skill/cast queries.
 if(!(squared(direction)>word(0x38d1b717))){
  if(!object_.heading.active)return true;
  navigation::set_heading_unchecked(object_.heading,direction,1);
  return character_stop(error);
 }
 if(machine_.current==6||machine_.current==7)return true;
 if(targets_.target){
  if(!services_.target_position)return missing(error,"Required live GetTargetPosition provider");
  const float* target{};const float* owner{};
  if(!services_.target_position(services_.context,targets_.target,target,error)||!target)return missing(error,"Required target GetTargetPosition backing");
  if(!targets_.owner||!services_.target_position(services_.context,targets_.owner->identity,owner,error)||!owner)return missing(error,"Required owner GetTargetPosition backing");
  const float delta[]{target[0]-owner[0],target[1]-owner[1],target[2]-owner[2]};
  // Point3D.angle/angleCos: original IEEE domain, no clamp/zero substitute.
  const auto dot=(direction[0]*delta[0]+direction[1]*delta[1])+direction[2]*delta[2];
  const auto angle=std::acos(dot/(std::sqrt(squared(direction))*std::sqrt(squared(delta))));
  if(angle>word(0x3fc90fdb)){
   if(dh2_character_ai_set_target(&targets_,0,0,&target_services_)!=0)return missing(error,"Required source SetTarget(NULL,false)");
   if(dh2_character_ai_sync_last_target(&targets_)!=0)return missing(error,"Required source SyncLastTarget");
  }
 }
 navigation::set_heading_unchecked(object_.heading,direction,1);
 if(!services_.raise_event||!services_.raise_event(services_.context,0,error))return missing(error,"Required Character RaiseEvent(0)");
 return true;
}
bool CharacterHeadingOwnerV1::character_stop(std::string& error){
 bool remote{};
 if(!services_.remotely_updated||!services_.remotely_updated(services_.context,remote,error))return missing(error,"Required Character IsRemotelyUpdated");
 if(remote)return true;
 if(!object_stop(error))return false;
 if(!services_.raise_event||!services_.raise_event(services_.context,0x3f,error))return missing(error,"Required Character RaiseEvent(0x3f)");
 return true;
}
bool CharacterHeadingOwnerV1::object_stop(std::string& error){
 if(!position_||!destination_)return missing(error,"Required same GameObject position/destination backing");
 // GameObject.Stop 3938f8 publishes all logical stores before body policy.
 if(dh2_nav_drop_path(&path_)!=0)return missing(error,"Required actual PFWorld DropPath backing");
 for(unsigned n=0;n<3;++n)destination_[n]=position_[n];
 object_.heading.active=0;object_.path_requested=0;
 for(auto& component:object_.heading.direction)component=0;
 if(!body_)return true; // source constructor-owned null physical pointer
 bool physics{};
 if(!services_.updating_from_physics||!services_.updating_from_physics(services_.context,physics,error))return missing(error,"Required Character IsUpdatingPositionFromPhysics");
 if(!physics)return true;
 if(dh2_native_body_stop(body_,position_)!=0)return missing(error,"Required same NativeBody Stop");
 return true;
}
}
