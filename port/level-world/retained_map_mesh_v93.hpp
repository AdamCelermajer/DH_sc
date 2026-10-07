#pragma once
#include "retained_visual_child_v91.hpp"
#include "scene_manager_map_owner_v2.hpp"
namespace dh2::world {
// All fields and geometry belong to the SAME source-created IMeshSceneNode.
// Source AddNodeToMap callbacks own that receiver/resource only; old parent,
// Scene and map authority are weak, never a whole Visual/base alias.
// Final actual map-parent drop after old Visual membership has gone.
bool retire_retained_map_mesh_v106(const std::shared_ptr<RetainedMeshNodeV91>&,std::string&);
bool read_retained_mesh_name_v93(const std::shared_ptr<RetainedMeshNodeV91>&,
 const std::shared_ptr<SceneManagerMapOwnerV2>&,std::string&,unsigned&,std::string&);
bool lend_retained_map_mesh_v93(const std::shared_ptr<RetainedMeshNodeV91>&,
 const std::shared_ptr<SceneManagerMapOwnerV2>&,SceneMapNodeBorrowV2&,std::string&);
}
