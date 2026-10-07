#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
namespace dh2::world {
struct GameObjectStopServicesV111 {
 std::function<bool(bool&,std::string&)> updating_position_from_physics64;
 std::function<bool(std::shared_ptr<void>&,physical::NativeBody*&,std::string&)> physical;
};
// Whole GameObject.Stop3938f8. No scene sample, world step, root/pose
// reconstruction or visual synchronization is hidden in this method.
bool gameobject_stop_source_v111(CanonicalGameObjectBaseOwnerV1&,
 const GameObjectStopServicesV111&,std::string&);
}
