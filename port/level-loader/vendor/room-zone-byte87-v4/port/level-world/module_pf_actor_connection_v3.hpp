#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "navigation_producers.hpp"
namespace dh2::world {
// Borrows the same canonical actor PFObject and actual candidate world's
// collision/obstacle owners. No parallel PFObject or registry is allocated.
class ModulePFActorConnectionV3 {
 CanonicalGameObjectBaseOwnerV1& base_;
 std::shared_ptr<void> world_pin_;
 const navigation::CollisionWorld* geometry_;
 navigation::ObstacleRegistry* obstacles_;
public:
 ModulePFActorConnectionV3(CanonicalGameObjectBaseOwnerV1& b,std::shared_ptr<void> pin,
  const navigation::CollisionWorld* g,navigation::ObstacleRegistry* o):base_(b),world_pin_(std::move(pin)),geometry_(g),obstacles_(o){}
 bool init(bool,const float*,float,std::uintptr_t,std::string&);
 bool update(std::string&);
};
}
