#include "authored_scene_subtree_v2.hpp"
#include <algorithm>
namespace dh2::world {
bool authored_scene_subtree_v2(const scene::Scene& full,const char* xref,scene::Scene& out,bool& found,std::string& e){
 found=false;if(!xref||!*xref){e="Required nonempty authored subscene xref";return false;}
 const auto id=std::string(xref)+"-node";std::size_t selected=full.graph.size();
 for(std::size_t i=0;i<full.graph.size();++i){const auto p=full.graph[i].parent;if(p< -1||p>=static_cast<std::int32_t>(i)){e="Invalid source node depth-first parent order";return false;}if(selected==full.graph.size()&&full.graph[i].id==id)selected=i;}
 if(selected==full.graph.size()){out={};return true;}
 scene::Scene result;result.materials=full.materials;std::vector<std::int32_t> map(full.graph.size(),-1);
 for(std::size_t i=selected;i<full.graph.size();++i){const auto p=full.graph[i].parent;if(i!=selected&&(p<0||map[static_cast<std::size_t>(p)]<0))continue;
  map[i]=static_cast<std::int32_t>(result.graph.size());auto n=full.graph[i];n.parent=i==selected?-1:map[static_cast<std::size_t>(p)];
  if(i==selected){std::fill_n(n.translation,3,0.f);std::fill_n(n.quaternion,3,0.f);n.quaternion[3]=1.f;std::fill_n(n.scale,3,1.f);}
  result.graph.push_back(std::move(n));
 }
 for(const auto& i:full.instances){if(i.node_index>=map.size()){e="Invalid source mesh node index";return false;}if(map[i.node_index]<0)continue;auto instance=i;instance.node_index=static_cast<std::uint32_t>(map[i.node_index]);result.instances.push_back(std::move(instance));}
 result.nodes=static_cast<unsigned>(result.graph.size());if(!scene::update_world(result,e))return false;out=std::move(result);found=true;return true;
}
}
