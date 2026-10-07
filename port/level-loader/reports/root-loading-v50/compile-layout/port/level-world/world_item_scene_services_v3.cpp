#include "world_item_scene_services_v3.hpp"
namespace dh2::world {
RetainedGameObjectVisualServicesV1 world_item_scene_services_v3(RetainedGameObjectVisualServicesV1 s,std::shared_ptr<GameObjectSceneRootRegistryV1> manager,std::function<std::shared_ptr<RetainedGameObjectVisualV1>(std::uintptr_t)> lookup){
 std::weak_ptr<GameObjectSceneRootRegistryV1> weak=manager;
 s.register_root=[weak,lookup](auto root,auto& e){auto m=weak.lock();if(!m||!lookup){e="Required SAME Item SceneManager/visual registry";return false;}auto v=lookup(root);if(!v||v->root_identity()!=root){e="Required SAME constructing Item root";return false;}
  GameObjectSceneRootBorrowV1 b;b.owner=v;b.identity=root;b.flags11c=&v->scene_flags();b.parentec=&v->root_parent_identity();std::weak_ptr<RetainedGameObjectVisualV1> node=v;
  b.notify_visibility=[node](bool visible,auto& error){auto n=node.lock();if(!n){error="Released actual Item root visibility receiver";return false;}return n->notify_root_visibility(visible,error);};
  b.remove_animators=[node](auto& error){auto n=node.lock();if(!n){error="Released actual Item animator receiver";return false;}return n->remove_root_animators(error);};return m->add_child(std::move(b),e);
 };
 s.force_register=[weak](auto,auto& e){auto m=weak.lock();if(!m){e="Required SAME Item SceneManager ForceRegister";return false;}m->force_register();return true;};
 s.release_root=[weak,lookup](auto root,auto& e){auto m=weak.lock();auto v=lookup?lookup(root):nullptr;if(!m||!v){e="Required SAME Item root release";return false;}if(!v->root_parent_identity()){if(!v->remove_root_animators(e))return false;m->notify_hierarchy_changed();return true;}return m->release_visual_root(root,e);};return s;
}
}
