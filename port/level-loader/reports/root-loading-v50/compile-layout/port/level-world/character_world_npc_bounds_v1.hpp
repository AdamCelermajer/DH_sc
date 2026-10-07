#pragma once
#include "character_scene.hpp"
namespace dh2::character {
// Pure CalcMeshBox/Character SetRelativeAABB arithmetic over the actual CPU
// cached model-space pose and actual visual root matrix. Does not create a
// BodyConfig, fabricate physical identities, or own any mutable bounds.
bool character_npc_visual_bounds_v1(const resources::BresView&,
 const scene::Scene& actual_model_space_pose,const float* actual_root_matrix16,
 const float* actual_position3,std::int32_t collision_scale,std::uint8_t previous_flat,
 physical::CharacterOwnerBounds&,std::string&);
}
