#include "authored_scene_subtree_v2.hpp"
#include <algorithm>
namespace dh2::world {
bool authored_scene_subtree_v2(const scene::Scene& full,const char* xref,scene::Scene& out,bool& found,std::string& e,
 const scene::AuthoredVisibilityV76* visibility,scene::AuthoredVisibilityV76* selected_visibility){
 found=false;if(selected_visibility)*selected_visibility={};if(bool(visibility)!=bool(selected_visibility)){e="Require paired SAME authored subtree visibility input/output";return false;}if(visibility&&(visibility->node_local.size()!=full.graph.size()||visibility->mesh_local.size()!=full.instances.size())){e="Authored subtree visibility differs from SAME full graph";return false;}if(!xref||!*xref){e="Required nonempty authored subscene xref";return false;}
 const auto id=std::string(xref)+"-node";std::size_t selected=full.graph.size();
 for(std::size_t i=0;i<full.graph.size();++i){const auto p=full.graph[i].parent;if(p< -1||p>=static_cast<std::int32_t>(i)){e="Invalid source node depth-first parent order";return false;}if(selected==full.graph.size()&&full.graph[i].id==id)selected=i;}
 if(selected==full.graph.size()){out={};return true;}
 scene::Scene result;result.materials=full.materials;std::vector<std::int32_t> map(full.graph.size(),-1);
 for(std::size_t i=selected;i<full.graph.size();++i){const auto p=full.graph[i].parent;if(i!=selected&&(p<0||map[static_cast<std::size_t>(p)]<0))continue;
  map[i]=static_cast<std::int32_t>(result.graph.size());auto n=full.graph[i];n.parent=i==selected?-1:map[static_cast<std::size_t>(p)];
  if(i==selected){std::fill_n(n.translation,3,0.f);std::fill_n(n.quaternion,3,0.f);n.quaternion[3]=1.f;std::fill_n(n.scale,3,1.f);}
  result.graph.push_back(std::move(n));
  if(visibility){const auto local=visibility->node_local[i];const auto parent=result.graph.back().parent;const auto effective=local&&(parent<0||selected_visibility->node_effective[static_cast<std::size_t>(parent)]);selected_visibility->node_local.push_back(local);selected_visibility->node_effective.push_back(effective?1u:0u);}
 }
 for(std::size_t original=0;original<full.instances.size();++original){const auto& i=full.instances[original];if(i.node_index>=map.size()){e="Invalid source mesh node index";return false;}if(map[i.node_index]<0)continue;auto instance=i;instance.node_index=static_cast<std::uint32_t>(map[i.node_index]);result.instances.push_back(std::move(instance));if(visibility){const auto local=visibility->mesh_local[original];selected_visibility->mesh_local.push_back(local);selected_visibility->mesh_effective.push_back(local&&selected_visibility->node_effective[result.instances.back().node_index]?1u:0u);}}
 for(const auto& light:full.lights_v113){if(light.node_index>=map.size()){e="Invalid actual light parent node";return false;}if(map[light.node_index]>=0)result.lights_v113.push_back({std::uint32_t(map[light.node_index]),light.light});}
 result.nodes=static_cast<unsigned>(result.graph.size());if(!scene::update_world(result,e))return false;out=std::move(result);found=true;return true;
}
}
