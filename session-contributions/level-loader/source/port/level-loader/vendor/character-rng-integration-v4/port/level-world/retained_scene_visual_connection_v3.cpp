#include "retained_scene_visual_connection_v3.hpp"
namespace dh2::world {
RetainedSceneVisualConnectionV3::RetainedSceneVisualConnectionV3(CanonicalGameObjectBaseOwnerV1& b,RetainedGameObjectVisualServicesV1 s,std::shared_ptr<GameObjectSceneRootRegistryV1> manager):manager_(std::move(manager)){
 s.register_root=[this](auto root,auto& e){
  if(!manager_){e="Required retained SAME SceneManager root owner";return false;}
  auto visual=connection_->lookup_root(root);if(!visual){e="Required constructing SAME retained visual root";return false;}
  GameObjectSceneRootBorrowV1 borrow;borrow.owner=visual;borrow.identity=root;
  borrow.flags11c=&visual->scene_flags();borrow.parentec=&visual->root_parent_identity();
  // Weak receiver captures avoid self-owned animator/visibility leases.
  std::weak_ptr<RetainedGameObjectVisualV1> weak=visual;
  borrow.notify_visibility=[weak](bool visible,auto& error){auto v=weak.lock();if(!v){error="Released source visibility receiver";return false;}return v->notify_root_visibility(visible,error);};
  borrow.remove_animators=[weak](auto& error){auto v=weak.lock();if(!v){error="Released source animator receiver";return false;}return v->remove_root_animators(error);};
  return manager_->add_child(std::move(borrow),e);
 };
 s.force_register=[this](auto,auto& e){if(!manager_){e="Required SAME SceneManager ForceRegister owner";return false;}manager_->force_register();return true;};
 s.release_root=[this](auto root,auto& e){
  if(!manager_){e="Required SAME SceneManager root release owner";return false;}
  auto visual=connection_->lookup_root(root);if(!visual){e="Required SAME source root release receiver";return false;}
  // Source remove59706c is a genuine empty-parent leaf after animator removal.
  // The visual still owns its root even if it was detached earlier.
  if(!visual->root_parent_identity()){if(!visual->remove_root_animators(e))return false;manager_->notify_hierarchy_changed();return true;}
  return manager_->release_visual_root(root,e);
 };
 connection_=std::make_shared<RetainedGameObjectVisualAssetConnectionV2>(b,std::move(s));
}
}
