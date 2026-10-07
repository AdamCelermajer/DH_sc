#pragma once
#include "decor_scene.hpp"
#include "../engine-math/math.hpp"
#include "../engine-skinning/skinning.hpp"
namespace dh2::physical {
struct CharacterSkinBoundsInput {
 const math::Matrix4f* joints;
 const float* joint_boxes;
 std::uint32_t joint_count,box_count;
};
struct CharacterMeshEntry {
 float bounds[6],parent_scale[3];
 std::uint32_t skinned;
};
struct CharacterMeshBoxInput {
 const CharacterMeshEntry* entries;
 std::uint32_t count,reserved;
 DecorSceneInput placement;
};
struct CharacterOwnerBoundsInput {
 float mesh_box[6],position[3];
 std::int32_t collision_scale;
 std::uint32_t already_scaled,previous_flat;
};
struct CharacterOwnerBounds {
 float relative_box[6],absolute_box[6];
 std::uint32_t flat,update_pf_count;
};
static_assert(sizeof(CharacterSkinBoundsInput)==24&&sizeof(CharacterMeshEntry)==40);
static_assert(sizeof(CharacterMeshBoxInput)==88);
static_assert(sizeof(CharacterOwnerBoundsInput)==48&&sizeof(CharacterOwnerBounds)==56);
// Uses the provided cached model-space joint pose. Complete Scene instances
// encode selected equipment; components attached to one node are unioned as
// the original modular provider before applying that node's parent scale.
bool character_scene_entries(const resources::BresView&,const scene::Scene&,
 std::vector<CharacterMeshEntry>&,std::string& error);
}
extern "C" {
int dh2_character_skin_bounds(float* out6,const dh2::physical::CharacterSkinBoundsInput*);
int dh2_character_mesh_box(dh2::physical::DecorSceneOutput*,const dh2::physical::CharacterMeshBoxInput*);
// InitPost reads base properties12/13/14, before GameObject's scale clamp.
int dh2_character_visual_scale(float* out3,const std::int32_t* base_scale3);
// Character SetRelativeAABB reads resolved property16 (Collision_Scale),
// then invokes the shared GameObject padding/absolute-bound/PF production.
int dh2_character_owner_bounds(dh2::physical::CharacterOwnerBounds*,const dh2::physical::CharacterOwnerBoundsInput*);
}
