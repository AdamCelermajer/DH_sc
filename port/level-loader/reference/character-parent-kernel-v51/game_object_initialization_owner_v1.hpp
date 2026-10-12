#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "game_object_initialization_services_v51.hpp"
#include <functional>
#include <vector>
namespace dh2::world {
// All services operate on this SAME canonical base/visual/PF graph. A missing
// reached service is an error, not a successful no-op or a generated visual.

// Whole ObjectBase::InitPost33ec0c, GameObject::InitPost38be5c and
// GameObject::InitFinal38cd48 control/stores over real constructor/property
// storage. Visual construction, condition compilation and PF are explicit
// concrete providers. A loader must discard a failed candidate; no retry gate
// is invented inside these original methods.
class GameObjectInitializationOwnerV1 {
 CanonicalGameObjectBaseOwnerV1& base_;
 GameObjectInitializationServicesV1 services_;
 bool missing(const char*,std::string&)const;
 bool spawn(std::int32_t&,std::string&);
public:
 GameObjectInitializationOwnerV1(CanonicalGameObjectBaseOwnerV1& b,GameObjectInitializationServicesV1 s):base_(b),services_(std::move(s)){}
 bool object_base_init_post(std::string&);
 bool init_post(bool& source_eligible,std::string&);
 bool init_final(bool& source_eligible,std::string&);
};
}
