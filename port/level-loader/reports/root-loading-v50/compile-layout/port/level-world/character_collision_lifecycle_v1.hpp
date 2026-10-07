#pragma once
#include "character_world_npc_object_v1.hpp"
namespace dh2::character {
// Whole GameObject.Enable/DisableCollisions394a3c/3949b0, borrowing the same
// PFObject/World and actual physical-filter owner. No lifecycle field copies.
class CharacterCollisionLifecycleV1 {
 navigation::NavigationObject& object_;const navigation::CollisionWorld* geometry_;
 navigation::ObstacleRegistry& obstacles_;std::uint64_t identity_;
 WorldNpcObjectServicesV1 services_;std::string error_;
 bool apply(bool);
public:
 CharacterCollisionLifecycleV1(navigation::NavigationObject& object,
  const navigation::CollisionWorld* geometry,navigation::ObstacleRegistry& obstacles,
  std::uint64_t id,WorldNpcObjectServicesV1 services):object_(object),geometry_(geometry),obstacles_(obstacles),identity_(id),services_(services){}
 bool enable(){return apply(true);}bool disable(){return apply(false);}
 void rebind_geometry(const navigation::CollisionWorld* current)noexcept{geometry_=current;}
 const std::string& error()const noexcept{return error_;}
};
}
