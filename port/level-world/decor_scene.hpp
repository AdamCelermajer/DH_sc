#pragma once
#include "decor_body_config.hpp"
#include "../scene-materials/scene.hpp"

namespace dh2::physical {
struct DecorSceneInput {
 float position[3],rotation_degrees[3],scale[3];
 float marker_bounds[6],marker_parent_scale[3];
};
struct DecorSceneOutput {
 float effective_scale[3],rotation_radians[3],root_quaternion[4];
 float root_matrix[16],mesh_box[6];
};
struct DecorSceneMarker {
 std::uint32_t found=0,node_index=0,geometry=0;
 float bounds[6]{},parent_scale[3]{1,1,1};
};
static_assert(sizeof(DecorSceneInput)==72&&sizeof(DecorSceneOutput)==128);
// Uses the complete immutable scene, before render-only helpers are removed.
// The verified Crypt marker factory case is one static mesh and no child nodes.
// Other marker topologies fail explicitly; absence is a successful found=0.
bool decor_scene_marker(const resources::BresView&,const scene::Scene&,
 DecorSceneMarker&,std::string& error);
// Loads a complete scene for callers whose render instance list was filtered.
bool decor_scene_marker(const resources::BresView&,DecorSceneMarker&,std::string& error);
}
extern "C" {
// GameObject InitPost scale/degree conversion, VisualObject SetRotation, root
// relative transformation and the original marker CalcMeshBox branch.
int dh2_decor_scene(dh2::physical::DecorSceneOutput*,const dh2::physical::DecorSceneInput*);
}
