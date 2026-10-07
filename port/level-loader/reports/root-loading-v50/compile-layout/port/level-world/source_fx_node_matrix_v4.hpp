#pragma once
#include "../engine-math/math.hpp"
#include "../scene-materials/scene.hpp"
namespace dh2::fx {
// Whole CMatrix4 product35e118, including65-byte identity-copy branches.
void source_fx_matrix_multiply_v4(math::Matrix4f&,const math::Matrix4f&,const math::Matrix4f&);
// Same producer used by the FX manager's TRS outer transform. The quaternion
// matrix writes the identity hint; no float-equality heuristic is involved.
void source_fx_trs_matrix_v4(math::Matrix4f&,const float*,const float*,const float*);
// Native typed projection of the SAME retained graph world matrix. Every
// local TRS uses the original quaternion producer (hint0), so ordinary world
// multiplication or parent-identity copy preserves that source hint0. Copies
// actual graph values, never a second Scene or a guessed identity matrix.
bool source_fx_node_world_matrix_v4(const scene::Scene&,std::uint32_t,math::Matrix4f&,std::string&);
bool source_fx_rebuild_graph_world_v4(scene::Scene&,std::string&);
}
