#pragma once
#include "../scene-materials/scene.hpp"
namespace dh2::world {
// SceneManager LoadScene(nonempty xref) appends literal '-node', then getNode
// depth-first compares serialized SNode.id. ResetPositionFromFile resets the
// selected first child TRS before parenting it to the new actual root.
bool authored_scene_subtree_v2(const scene::Scene&,const char* xref,scene::Scene&,bool& found,std::string&);
}
