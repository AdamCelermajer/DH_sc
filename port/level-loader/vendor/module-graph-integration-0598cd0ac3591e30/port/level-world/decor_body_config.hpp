#pragma once
#include "character_body_config.hpp"
namespace dh2::physical {
struct DecorMeshBoxInput {
 float bounds[6],parent_scale[3],node_matrix[16];
};
struct DecorBodyInput {
 void* owner;void* new_physical;void* previous_physical;
 std::uint32_t visual_present,colbox_found,collision_group_override,disable_physical;
 float mesh_box[6],position[3]; // CalcMeshBox output and owner placement, game units
 std::uint32_t previous_flat;
};
struct DecorBodyConfig {
 CharacterBodyConfig physical;
 float relative_box[6],absolute_box[6];
 std::uint32_t flat,mesh_pf_updates;
};
static_assert(sizeof(DecorBodyInput)==80&&offsetof(DecorBodyInput,mesh_box)==40);
static_assert(sizeof(DecorBodyConfig)==264&&offsetof(DecorBodyConfig,relative_box)==208);
static_assert(sizeof(DecorMeshBoxInput)==100);
}
extern "C" {
// Definitions and ordered constructor/attachment services. The scene's first
// depth-first node whose name begins "_colbox_" supplies colbox_found. Mesh-box
// evaluation/scene transforms remain caller services. The ordinary ApplyMeshBox
// PF update is counted separately from physical.requests. No invented shapes.
int dh2_decor_body_config(dh2::physical::DecorBodyConfig*,const dh2::physical::DecorBodyInput*);
// Exact CalcMeshBox branch when the authored marker exists. Node bounding-box,
// parent scale and root matrix are scene services; matrix translation is zeroed.
// The original transforms two endpoints, orders them, and recenters at zero.
int dh2_decor_marker_mesh_box(float* out6,const dh2::physical::DecorMeshBoxInput*);
// Literal Level::_LoadProcess gameplay bounds, already in physics world units.
int dh2_decor_level_world_bounds(float* out4);
}
