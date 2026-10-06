#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include <functional>
namespace dh2::world {
struct GameObjectSetPositionServicesV2 {
 std::shared_ptr<void> owner;
 // Source attached +2e0 object, whose XYZ starts at +0c. Required only nonnull.
 std::function<bool(std::uintptr_t,float*&,std::string&)> attached_position;
 std::function<bool(std::uintptr_t,float,float,std::string&)> physical_position;
 std::function<bool(std::uintptr_t,std::string&)> visual_sync;
};
// Whole GameObject::SetPosition393db4 + SetDestination393600. All writes borrow
// the caller's canonical base/runtime; reached backend failures retain prefix.
bool game_object_set_position_v2(CanonicalGameObjectBaseOwnerV1&,
 const float* position3,bool set_destination,const GameObjectSetPositionServicesV2&,std::string&);
}
