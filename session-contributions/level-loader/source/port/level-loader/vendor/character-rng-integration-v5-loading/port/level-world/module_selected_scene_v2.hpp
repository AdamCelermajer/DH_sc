#pragma once
#include "../scene-materials/scene.hpp"
namespace dh2::world {
struct ModuleSelectedSceneV2 {
 scene::Scene scene;
 std::vector<std::uint8_t> node_visibility,instance_visibility;
};
// Source LoadScene3596f8 appends literal "-node" to nonempty xref, then
// database.getNode searches visual scene0 by SNode.id in depth-first order.
// A source name miss is delivered=true/found=false, never the complete scene.
bool module_selected_scene_v2(const resources::BresView&,const char* xref,
 ModuleSelectedSceneV2&,bool& found,std::string&);
}
