#include "actor_scene_retention_v1.hpp"
#include <algorithm>
#include <cmath>
#include <exception>

namespace dh2::actor_scene_retention_v1 {namespace {
bool finite(const float* values,std::size_t count){
    return std::all_of(values,values+count,[](float value){return std::isfinite(value);});
}
bool valid_root(const visual::Root& root){
    return !(root.presence&~3u)&&finite(root.position,3)&&finite(root.quaternion,4)&&
        finite(root.scale,3)&&finite(root.animated,3)&&finite(root.secondary,3)&&finite(root.helper,3);
}
}
void Snapshot::clear()noexcept{
    playback_=nullptr;bank_=nullptr;nodes_.clear();root_={};animated_=-1;
}
bool Snapshot::capture(const actor::BlendedPlayback& playback,const actor::ClipBank& bank,
                       const visual::SceneBinding& visual,const scene::Scene& scene,std::string& error){
    error.clear();
    if(!playback.matches_binding(bank,visual,scene,error))return false;
    if(!valid_root(visual.root)){error="Retained animation root is invalid";return false;}
    try{
        Snapshot next;next.nodes_.reserve(scene.graph.size());
        for(std::size_t i=0;i<scene.graph.size();++i){
            const auto& node=scene.graph[i];
            if(node.parent< -1||node.parent>=std::int32_t(i)||!finite(node.translation,3)||
               !finite(node.quaternion,4)||!finite(node.scale,3)){
                error="Retained animation graph or local pose is invalid";return false;
            }
            NodePose pose;pose.id=node.id;pose.parent=node.parent;
            std::copy_n(node.translation,3,pose.translation);std::copy_n(node.quaternion,4,pose.quaternion);
            std::copy_n(node.scale,3,pose.scale);next.nodes_.push_back(std::move(pose));
        }
        for(const auto& instance:scene.instances)if(instance.node_index>=scene.graph.size()){
            error="Retained animation instance link is invalid";return false;
        }
        next.root_=visual.root;next.animated_=visual.animated_node();next.playback_=&playback;next.bank_=&bank;
        *this=std::move(next);return true;
    }catch(const std::exception& failure){error=failure.what();return false;}
}
bool Snapshot::restore(const actor::BlendedPlayback& playback,const actor::ClipBank& bank,
                       visual::SceneBinding& visual,scene::Scene& scene,std::string& error)const{
    error.clear();
    if(empty()||playback_!=&playback||bank_!=&bank){error="Retained animation owner identity differs";return false;}
    if(scene.graph.size()!=nodes_.size()){error="Retained animation graph size differs";return false;}
    for(std::size_t i=0;i<nodes_.size();++i){const auto& node=scene.graph[i];const auto& pose=nodes_[i];
        if(node.id!=pose.id||node.parent!=pose.parent){error="Retained animation node or parent identity differs";return false;}
    }
    try{
        scene::Scene next_scene=scene;visual::SceneBinding next_visual;
        if(!next_visual.bind(next_scene,error))return false;
        if(next_visual.animated_node()!=animated_){error="Retained animation root identity differs";return false;}
        if(!playback.matches_binding(bank,next_visual,next_scene,error))return false;
        for(std::size_t i=0;i<nodes_.size();++i){auto& node=next_scene.graph[i];const auto& pose=nodes_[i];
            std::copy_n(pose.translation,3,node.translation);std::copy_n(pose.quaternion,4,node.quaternion);
            std::copy_n(pose.scale,3,node.scale);
        }
        next_visual.root=root_;
        if(!next_visual.update_world(next_scene,error))return false;
        scene=std::move(next_scene);visual=std::move(next_visual);return true;
    }catch(const std::exception& failure){error=failure.what();return false;}
}
}
