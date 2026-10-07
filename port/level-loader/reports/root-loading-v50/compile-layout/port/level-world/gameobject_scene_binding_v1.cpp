#include "gameobject_scene_binding_v1.hpp"
#include "../engine-math/math.hpp"
#include <cmath>
namespace dh2::visual {
bool GameObjectSceneBindingV1::bind(const scene::Scene& s,std::string& e){
 e.clear();std::vector<std::string> ids;ids.reserve(s.graph.size());
 for(std::size_t i=0;i<s.graph.size();++i){auto& n=s.graph[i];if(n.parent< -1||n.parent>=static_cast<std::int32_t>(i)){e="GameObject authored graph order rejected";return false;}ids.push_back(n.id);}
 for(auto& instance:s.instances)if(instance.node_index>=s.graph.size()){e="GameObject authored mesh link rejected";return false;}
 identities_=std::move(ids);return true;
}
bool GameObjectSceneBindingV1::update_world(scene::Scene& s,std::string& e)const{
 e.clear();if(s.graph.size()!=identities_.size()){e="GameObject scene differs from retained binding";return false;}
 std::array<float,16> owner;dh2_node_matrix(owner.data(),root.position,root.quaternion,root.scale);
 std::vector<std::array<float,16>> matrices;matrices.reserve(s.graph.size());
 for(std::size_t i=0;i<s.graph.size();++i){auto& n=s.graph[i];
  if(n.id!=identities_[i]||n.parent< -1||n.parent>=static_cast<std::int32_t>(i)){e="GameObject retained graph identity/order differs";return false;}
  std::array<float,16> local;dh2_node_matrix(local.data(),n.translation,n.quaternion,n.scale);
  matrices.push_back(scene::multiply(n.parent<0?owner:matrices[n.parent],local));
  for(float v:matrices.back())if(!std::isfinite(v)){e="GameObject transform overflow";return false;}
 }
 for(auto& instance:s.instances)if(instance.node_index>=matrices.size()){e="GameObject retained mesh link differs";return false;}
 for(std::size_t i=0;i<matrices.size();++i)s.graph[i].world=matrices[i];
 for(auto& instance:s.instances)instance.world=matrices[instance.node_index];return true;
}
bool GameObjectSceneBindingV1::sample(scene::Scene& s,const animation::Player& p,std::int32_t ms,std::string& e){
 if(s.graph.size()!=identities_.size()){e="GameObject animation graph differs from retained binding";return false;}
 if(!p.sample(s,ms,e))return false;return update_world(s,e);
}
}
