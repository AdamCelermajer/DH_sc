#pragma once
#include "decor_scene.hpp"
namespace dh2::world {
// Original CalcMeshBox472408 enumerates static mesh nodes first, skinned only
// if none, unions their bbox after immediate-parent scale, then applies root
// linear transform and centers extent on Vec3fOrigin. No mesh returns zero.
bool visual_mesh_box_fallback_v2(const resources::BresView&,const scene::Scene&,const float* root_position,const float* root_quaternion,const float* root_scale,float* out6,std::string&);
}
