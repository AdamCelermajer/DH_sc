#include "retained_character_visual_connection_v5.hpp"
#include <algorithm>
namespace dh2::world {
RetainedCharacterVisualConnectionV5::RetainedCharacterVisualConnectionV5(GameObjectVisualFieldBorrowV5 fields,std::uintptr_t& same_visual,RetainedGameObjectVisualServicesV1 s,std::shared_ptr<GameObjectSceneRootRegistryV1> manager):fields_(std::move(fields)),visual2d8_(same_visual),manager_(std::move(manager)){
 s.register_root=[this](auto root,auto& e){
  if(!manager_){e="Required retained SAME SceneManager root owner";return false;}
  auto visual=lookup_root(root);if(!visual){e="Required constructing SAME retained visual root";return false;}
  GameObjectSceneRootBorrowV1 borrow;borrow.owner=visual;borrow.identity=root;
  borrow.flags11c=&visual->scene_flags();borrow.parentec=&visual->root_parent_identity();
  // Weak receiver captures avoid self-owned animator/visibility leases.
  std::weak_ptr<RetainedGameObjectVisualV1> weak=visual;
  borrow.notify_visibility=[weak](bool visible,auto& error){auto v=weak.lock();if(!v){error="Released source visibility receiver";return false;}return v->notify_root_visibility(visible,error);};
  borrow.remove_animators=[weak](auto& error){auto v=weak.lock();if(!v){error="Released source animator receiver";return false;}return v->remove_root_animators(error);};
  borrow.modular_receivers_v114=[weak](auto& out,auto& error){auto v=weak.lock();if(!v){error="Released source modular-search root";return false;}return v->source_modular_receivers_v114(weak,out,error);};
  borrow.light_by_name_v113=[weak](const auto& name,auto& out,auto& e){auto v=weak.lock();return v&&v->source_light_by_name_v113(name,out,e);};
  borrow.scene_phase_v69=[weak,manager=std::weak_ptr<GameObjectSceneRootRegistryV1>(manager_)](std::uint32_t stamp,std::string& error){
   auto v=weak.lock();auto roots=manager.lock();if(!v||!roots){error="Released actual Character visual/Scene phase";return false;}
   return v->source_scene_phase_v69(stamp,[roots](std::string& e){roots->notify_visibility_changed_v3();e.clear();return true;},error);
  };
  return manager_->add_child(std::move(borrow),e);
 };
 s.find_modular_v114=[manager=std::weak_ptr<GameObjectSceneRootRegistryV1>(manager_)](auto& out,auto& e){auto actual=manager.lock();if(!actual){e="Retired SAME modular-search SceneManager";return false;}return actual->source_find_modular_v114(out,e);};
 s.notify_visibility_v114=[manager=std::weak_ptr<GameObjectSceneRootRegistryV1>(manager_)](auto& e){auto actual=manager.lock();if(!actual){e="Retired SAME modular visibility receiver";return false;}actual->notify_visibility_changed_v3();e.clear();return true;};
 s.force_register=[this](auto,auto& e){if(!manager_){e="Required SAME SceneManager ForceRegister owner";return false;}manager_->force_register();return true;};
 s.release_root=[this](auto root,auto& e){
  if(!manager_){e="Required SAME SceneManager root release owner";return false;}
  auto visual=lookup_root(root);if(!visual){e="Required SAME source root release receiver";return false;}
  // Source remove59706c is a genuine empty-parent leaf after animator removal.
  // The visual still owns its root even if it was detached earlier.
  if(!visual->root_parent_identity()){if(!visual->remove_root_animators(e))return false;manager_->notify_hierarchy_changed();return true;}
  return manager_->release_visual_root(root,e);
 };
 source_=std::move(s);
}

std::shared_ptr<RetainedGameObjectVisualV1> RetainedCharacterVisualConnectionV5::lookup(std::uintptr_t id)const{auto p=visuals_.find(id);return p==visuals_.end()?nullptr:p->second.visual;}
std::shared_ptr<RetainedGameObjectVisualV1> RetainedCharacterVisualConnectionV5::attached()const{return lookup(visual2d8_);}
std::shared_ptr<RetainedGameObjectVisualV1> RetainedCharacterVisualConnectionV5::lookup_root(std::uintptr_t id)const{if(!id)return nullptr;for(auto& p:visuals_)if(p.second.visual->root_identity()==id)return p.second.visual;return nullptr;}
std::size_t RetainedCharacterVisualConnectionV5::failed_count()const{std::size_t n=0;for(auto& p:visuals_)if(p.second.failed)++n;return n;}
bool RetainedCharacterVisualConnectionV5::destroy(std::uintptr_t id,std::string& e){auto p=visuals_.find(id);if(p==visuals_.end()){e="Required SAME retained visual deleting-destructor receiver";return false;}if(!p->second.visual->release(e))return false;visuals_.erase(p);creation_order_.erase(std::remove(creation_order_.begin(),creation_order_.end(),id),creation_order_.end());return true;}
GameObjectVisualAssetServicesV1 RetainedCharacterVisualConnectionV5::services(std::shared_ptr<void> lease){
 GameObjectVisualAssetServicesV1 s;s.owner=std::move(lease);
 s.construct=[this](auto parent,const auto& model,const auto& xref,std::uintptr_t& result,auto& e){
  result=0;if(parent!=fields_.identity){e="Required SAME canonical visual parent";return false;}
  auto visual=std::make_shared<RetainedGameObjectVisualV1>(fields_,source_);auto id=reinterpret_cast<std::uintptr_t>(visual.get());
  visuals_.emplace(id,Candidate{visual,true});creation_order_.push_back(id);
  if(!visual->initialize(model.c_str(),xref.c_str(),e))return false;
  visuals_.at(id).failed=false;result=id;return true;
 };
 s.root=[this](auto id,auto& root,auto& e){auto v=lookup(id);if(!v){e="Required owned visual root receiver";return false;}root=v->root_identity();return true;};
 s.destroy=[this](auto id,auto& e){return destroy(id,e);};
 s.set_root_game_object=[this](auto root,auto parent,auto& e){for(auto& p:visuals_)if(p.second.visual->root_identity()==root)return p.second.visual->set_root_game_object(parent,e);e="Required SAME root204 receiver";return false;};
 return s;
}
bool RetainedCharacterVisualConnectionV5::discard_failed(std::string& e){
 auto ids=creation_order_;for(auto it=ids.rbegin();it!=ids.rend();++it){auto p=visuals_.find(*it);if(p==visuals_.end()||!p->second.failed)continue;
  if(visual2d8_==*it){e="Failed visual unexpectedly attached to canonical base";return false;}
  if(!destroy(*it,e))return false;
 }return true;
}
bool RetainedCharacterVisualConnectionV5::discard_unattached(std::string& e){
 if(visual2d8_){e="Detach canonical visual through source SetVisualObject before registry discard";return false;}
 auto ids=creation_order_;for(auto it=ids.rbegin();it!=ids.rend();++it)if(!destroy(*it,e))return false;return true;
}
}
