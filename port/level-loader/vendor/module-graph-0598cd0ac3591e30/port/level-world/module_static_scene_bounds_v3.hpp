#pragma once
#include "module_static_scene_v2.hpp"
namespace dh2::world {
// RootSceneNode::RefreshBoundingBox35c854 calls CONST computeBoundingBox65cf8c:
// union cached world mesh bounds, then subtract the root LOCAL position.
// Hidden children participate; detached children do not. No skin/particle
// bbox fallback is accepted in this actual SWAMP static-mesh domain.
bool module_static_scene_bounds_v3(const resources::BresView&,ModuleStaticSceneV2&,
 std::array<float,6>& world_box,std::string&);
// Separate source virtual34 query. Membership removal does not manufacture
// a RefreshBoundingBox call or invalidate the source cached box.
bool module_static_scene_query_bounds_v3(ModuleStaticSceneV2&,
 std::array<float,6>& world_box,std::string&);
}
