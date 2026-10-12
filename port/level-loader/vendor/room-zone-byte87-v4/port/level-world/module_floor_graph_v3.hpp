#pragma once
#include "floors.hpp"
namespace dh2::world {
// Appends the just-published floor to the SAME retained navigation World.
// Unlike floors::build_graph this runs at each _LoadNavMesh continuation;
// earlier floors and their tree roots survive the append. Does not sew rooms.
bool module_floor_graph_append_v3(floors::World&,unsigned floor,std::string&);
}
