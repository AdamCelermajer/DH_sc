#pragma once
#include "decor_scene.hpp"
#include <array>
#include <vector>
namespace dh2::world {
// Actual virtual30 local box and virtual90 parent scale, in source
// SearchByType traversal order. This is not an aggregate world-space box.
struct ModuleVisualMeshV2 {
 std::array<float,6> local_box{};
 std::array<float,3> parent_scale{};
 bool animated{};
};
// VisualObject::CalcMeshBox47211c fallback, including animated-first query,
// two-corner root-relative transform and recentering. Empty searches store zero
// directly without applying the common transform tail.
bool module_visual_mesh_box_v2(const std::vector<ModuleVisualMeshV2>&,
 const std::array<float,16>& actual_root_relative,
 std::array<float,6>&,std::string&);
}
