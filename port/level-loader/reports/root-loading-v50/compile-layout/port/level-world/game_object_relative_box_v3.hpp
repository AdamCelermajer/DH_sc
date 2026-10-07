#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include <functional>
namespace dh2::world {
// Whole GameObject::SetRelativeAABB38b110. Source ignores its bool argument.
// The required UpdatePFObject continuation must use this SAME base/runtime.
bool game_object_relative_box_v3(CanonicalGameObjectBaseOwnerV1&,const float* box6,
 const std::function<bool(std::string&)>& update_pf,std::string&);
}
