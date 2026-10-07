#include "character_world_npc_scene_v1.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::character {
CharacterWorldNpcSceneV1::CharacterWorldNpcSceneV1(visual::SceneBinding& visual,scene::Scene& scene,
 std::uint32_t& light,WorldNpcSceneServicesV1 services):visual_(visual),scene_(scene),light_(light),services_(services){}
bool CharacterWorldNpcSceneV1::changed_bounds(std::string& error){
 if(!services_.refresh_bounds){error="required source Visual CalcMeshBox/UpdateOwnerAABB/PF unavailable";return false;}
 return services_.refresh_bounds(services_.context,error);
}
bool CharacterWorldNpcSceneV1::receive_init_post(const physical::NpcBodyProjection& projection,const float* position,std::string& error){
 error.clear();if(!position){error="required actual NPC position unavailable";return false;}
 std::copy_n(position,3,visual_.root.position);
 std::copy_n(projection.visual.root_quaternion,4,visual_.root.quaternion);
 std::copy_n(projection.visual.effective_scale,3,visual_.root.scale);
 return visual_.update_world(scene_,error);
}
bool CharacterWorldNpcSceneV1::sync(const float* position,const float* euler,const float* scale,std::string& error){
 error.clear();if(!position||!euler||!scale){error="required actual NPC source transforms unavailable";return false;}
 std::copy_n(position,3,visual_.root.position);visual_.root.flags|=8;
 if(!visual_.update_world(scene_,error))return false;
 float quaternion[4];if(dh2_visual_rotation(quaternion,euler)){error="source visual quaternion conversion failed";return false;}
 bool same=true;for(unsigned i=0;i<4;++i)same&=quaternion[i]==visual_.root.quaternion[i];
 if(!same){if(!visual_.set_rotation(euler)||!visual_.update_world(scene_,error)||!changed_bounds(error))return false;}
 same=true;for(unsigned i=0;i<3;++i)same&=scale[i]==visual_.root.scale[i];
 if(!same){std::copy_n(scale,3,visual_.root.scale);if(!visual_.update_world(scene_,error)||!changed_bounds(error))return false;}
 return true;
}
bool CharacterWorldNpcSceneV1::find_node(const char* name,std::uintptr_t& result,std::string& error)const{
 error.clear();if(!name){error="required actual source scene node name unavailable";return false;}
 std::vector<std::vector<unsigned>> children(scene_.graph.size()+1);std::vector<unsigned> pending;
 for(unsigned i=0;i<scene_.graph.size();++i){const auto parent=scene_.graph[i].parent;if(parent< -1||parent>=std::int32_t(i)){error="source scene hierarchy invalid";return false;}children[parent<0?scene_.graph.size():unsigned(parent)].push_back(i);}
 for(auto i=children.back().rbegin();i!=children.back().rend();++i)pending.push_back(*i);
 while(!pending.empty()){auto i=pending.back();pending.pop_back();if(scene_.graph[i].name==name){result=reinterpret_cast<std::uintptr_t>(&scene_.graph[i]);return true;}for(auto j=children[i].rbegin();j!=children[i].rend();++j)pending.push_back(*j);}
 result=0;return true;
}
bool CharacterWorldNpcSceneV1::node_position(std::uintptr_t node,float* xyz,std::string& error)const{
 error.clear();if(!xyz){error="required actual node position destination unavailable";return false;}
 for(const auto& current:scene_.graph)if(reinterpret_cast<std::uintptr_t>(&current)==node){std::copy_n(current.world.data()+12,3,xyz);return true;}
 error="target node no longer belongs to actual NPC scene";return false;
}
}
