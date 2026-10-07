#include "gameplay_camera_scene_membership_v15.hpp"
namespace dh2::camera {
bool CameraNodeLifetimeV15::grab(std::string& e){if(!alive_||!scene_){e="Released actual camera node grab";return false;}++references_;return true;}
bool CameraNodeLifetimeV15::drop(std::string& e){if(!alive_||!references_){e="Required live camera-node reference before source drop";return false;}--references_;if(!references_){alive_=false;scene_.reset();}return true;}
bool CameraNodeLifetimeV15::camera_index(std::uint32_t& out,std::string& e)const{if(!alive_||!scene_||index_>=scene_->cameras().size()){e="Released actual camera instance binding";return false;}out=index_;return true;}
void CameraNodeLifetimeV15::notify_parent_visibility(bool parent)noexcept{parent121_=parent;if(local120_&&parent)flags11c_|=1u;else flags11c_&=~1u;}
GameplayCameraSceneMembershipV15::GameplayCameraSceneMembershipV15(std::shared_ptr<GameplayCameraSceneV3> scene,std::shared_ptr<world::GameObjectSceneRootRegistryV1> manager,std::function<bool(std::uint32_t,std::uintptr_t,std::string&)> changed):manager_(manager),scene_(std::move(scene)),camera_manager_changed_(std::move(changed)){
 if(scene_){nodes_.resize(scene_->graph().graph.size());for(unsigned i=0;i<scene_->cameras().size();++i)cameras_.push_back(std::make_shared<CameraNodeLifetimeV15>(scene_,i));}
}
void GameplayCameraSceneMembershipV15::notify_node(std::size_t index,bool parent){auto& n=nodes_[index];const auto before=n.flags11c&1u;n.parent121=parent;if(n.local120&&parent)n.flags11c|=1u;else n.flags11c&=~1u;if(before!=(n.flags11c&1u)){for(unsigned i=0;i<nodes_.size();++i)if(scene_->graph().graph[i].parent==static_cast<std::int32_t>(index))notify_node(i,(n.flags11c&1u)!=0);for(unsigned i=0;i<cameras_.size();++i)if(scene_->cameras()[i].node==index)cameras_[i]->notify_parent_visibility((n.flags11c&1u)!=0);}}
bool GameplayCameraSceneMembershipV15::changed_manager(std::uintptr_t manager,std::string& e){manager110_=manager;if(!camera_manager_changed_){e="Required actual camera onChangedSceneManager projection owner";return false;}for(unsigned i=0;i<cameras_.size();++i){cameras_[i]->source_manager110()=manager;if(!camera_manager_changed_(i,manager,e))return false;}return true;}
bool GameplayCameraSceneMembershipV15::notify_visibility(bool parent,std::string& e){if(!scene_||nodes_.size()!=scene_->graph().graph.size()){e="Required same actual camera-root visibility tree";return false;}const auto before=root_.flags11c&1u;root_.parent121=parent;if(root_.local120&&parent)root_.flags11c|=1u;else root_.flags11c&=~1u;if(before!=(root_.flags11c&1u))for(unsigned i=0;i<nodes_.size();++i)if(scene_->graph().graph[i].parent<0)notify_node(i,(root_.flags11c&1u)!=0);return true;}
bool GameplayCameraSceneMembershipV15::attach_graph(std::string& e){
 if(add_attempted_||!scene_||nodes_.empty()||parents_released_){e="Required fresh actual camera-root construction";return false;}auto manager=manager_.lock();if(!manager){e="Released same World scene-root manager";return false;}
 world::GameObjectSceneRootBorrowV1 b;b.owner=shared_from_this();b.identity=root_identity();b.flags11c=&root_.flags11c;b.parentec=&parentec_;std::weak_ptr<GameplayCameraSceneMembershipV15> weak=shared_from_this();
 b.notify_visibility=[weak](bool visible,std::string& error){auto self=weak.lock();if(!self){error="Released camera visibility receiver";return false;}return self->notify_visibility(visible,error);};
 b.remove_animators=[weak](std::string& error){auto self=weak.lock();if(!self){error="Released camera animator receiver";return false;}return self->remove_animators(error);};
 b.scene_manager_changed=[weak](std::uintptr_t id,std::string& error){auto self=weak.lock();if(!self){error="Released camera scene-manager receiver";return false;}return self->changed_manager(id,error);};
 add_attempted_=true;if(!manager->add_child(std::move(b),e))return false;registered_=true;manager->force_register();return true;
}
bool GameplayCameraSceneMembershipV15::attach_animator(std::shared_ptr<GameplayCameraAnimatorV10> animator,std::string& e){
 if(!registered_||!animator||!animator->ready()||animator->scene_borrow()!=scene_){e="Required actual same-camera root AnimatorSet/onBind";return false;}
 for(const auto& old:animators_)if(old==animator){e="Camera animator already attached";return false;}animators_.push_back(std::move(animator));return true;
}
bool GameplayCameraSceneMembershipV15::remove_animators(std::string&){for(auto& animator:animators_)animator->release_after_unregistration();animators_.clear();return true;}
bool GameplayCameraSceneMembershipV15::detach_visual(std::string& e){
 if(!add_attempted_)return true;auto manager=manager_.lock();if(!manager){e="Required live same manager for actual camera detach";return false;}
 if(parentec_){if(!manager->release_visual_root(root_identity(),e))return false;}else{if(!remove_animators(e))return false;manager->notify_hierarchy_changed();}
 registered_=false;add_attempted_=false;return true;
}
bool GameplayCameraSceneMembershipV15::release_parent_camera_references(std::string& e){if(parentec_||!animators_.empty()){e="Camera root still registered before source root destruction";return false;}if(parents_released_)return true;for(auto& camera:cameras_)if(!camera->drop(e))return false;cameras_.clear();parents_released_=true;return true;}
bool GameplayCameraSceneMembershipV15::retain_camera(std::uint32_t index,std::shared_ptr<CameraNodeLifetimeV15>& out,std::string& e){if(index>=cameras_.size()||parents_released_){e="Required same constructed camera child";return false;}if(!cameras_[index]->grab(e))return false;out=cameras_[index];return true;}
world::GameObjectSceneCameraBorrowV13 GameplayCameraSceneMembershipV15::scene_camera_borrow(std::shared_ptr<CameraNodeLifetimeV15> node){world::GameObjectSceneCameraBorrowV13 out;if(!node)return out;out.owner=node;out.identity=reinterpret_cast<std::uintptr_t>(node.get());out.grab=[node](std::string& e){return node->grab(e);};out.drop=[node](std::string& e){return node->drop(e);};return out;}
const std::vector<std::uint32_t> GameplayCameraSceneMembershipV15::visibility_flags()const{std::vector<std::uint32_t> out;for(auto& n:nodes_)out.push_back(n.flags11c);return out;}
}
