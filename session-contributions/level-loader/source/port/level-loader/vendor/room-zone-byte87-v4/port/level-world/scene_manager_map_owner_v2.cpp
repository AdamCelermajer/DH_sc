#include "scene_manager_map_owner_v2.hpp"
#include <algorithm>
namespace dh2::world {
SceneManagerMapOwnerV2::SceneManagerMapOwnerV2(std::shared_ptr<GameObjectSceneRootRegistryV1> manager):scene_manager_(std::move(manager)){}
bool SceneManagerMapOwnerV2::visibility(bool parent,std::string& error){
 const bool previous=(flags_&1u)!=0;
 parent_visible_=parent;
 if(visible())flags_|=1u;else flags_&=~1u;
 if(previous==visible())return true;
 for(auto& child:children_){
  if(!child.notify_parent_visibility){error="Scene map requires actual child visibility receiver";return false;}
  if(!child.notify_parent_visibility(visible(),error))return false;
 }
 return true;
}
bool SceneManagerMapOwnerV2::add(SceneMapNodeBorrowV2 child,std::string& error){
 error.clear();
 auto manager=scene_manager_.lock();
 if(!manager){error="Scene map requires SAME SceneManager root owner";return false;}
 if(!constructed_){
  auto lease=weak_from_this().lock();
  if(!lease){error="Scene map requires retained factory lease";return false;}
  constructed_=true; // source28c publication precedes root.addChild
  GameObjectSceneRootBorrowV1 node;node.owner=lease;node.identity=identity();node.flags11c=&flags_;node.parentec=&parent_;
  node.notify_visibility=[this](bool value,std::string& e){return visibility(value,e);};
  node.remove_animators=[this](std::string& e){
   // Source CSceneNode has an empty animator list; child animator destruction
   // is required if this group has acquired actual child nodes.
   if(!children_.empty()){e="Scene map release requires child animator removal continuation";return false;}return true;
  };
  if(!manager->add_child(std::move(node),error))return false;
  local_visible_=false; // source slot48(false) after manager publication/drop
  if(!visibility(parent_visible_,error))return false;
 }
 if(!child.owner||!child.identity||!child.parentec||!child.flags11c){error="Scene map requires actual retained node fields";return false;}
 // Local lease is source grab; it survives detach and every reached failure.
 if(*child.parentec){
  if(!child.cached_position||!child.set_position){error="Scene map reparent requires cached-position/setPosition source receivers";return false;}
  std::array<float,3> position{};
  if(!child.cached_position(position,error)||!child.set_position(position,error))return false;
  if(*child.parentec==identity()){
   if(!remove(child.identity,error))return false;
  }else{
   if(!child.detach){error="Scene map requires actual previous-parent detach receiver";return false;}
   if(!child.detach(error))return false;
  }
 }
 if(!child.optimize_static){error="Scene map requires complete actual OptimizeStatic receiver";return false;}
 if(!child.optimize_static(error))return false;
 *child.parentec=identity();*child.flags11c|=0x40u;children_.push_back(std::move(child));
 manager->notify_hierarchy_changed();
 if(!children_.back().notify_parent_visibility){error="Scene map requires actual child visibility receiver";return false;}
 return children_.back().notify_parent_visibility(visible(),error);
}
bool SceneManagerMapOwnerV2::remove(std::uintptr_t id,std::string& error){
 error.clear();auto found=std::find_if(children_.begin(),children_.end(),[&](const auto& c){return c.identity==id;});
 if(found==children_.end()){error="Scene map detach requires actual registered child";return false;}
 if(!found->parentec||*found->parentec!=identity()){error="Scene map child parent changed outside same hierarchy";return false;}
 auto manager=scene_manager_.lock();if(!manager){error="Scene map manager lifetime expired";return false;}
 *found->parentec=0;children_.erase(found);manager->notify_hierarchy_changed();return true;
}
bool SceneManagerMapOwnerV2::release(std::string& error){
 error.clear();if(!constructed_)return true;
 if(!children_.empty()){error="Scene map release requires actual child lifetime teardown first";return false;}
 auto manager=scene_manager_.lock();if(!manager){error="Scene map manager lifetime expired";return false;}
 if(!manager->release_visual_root(identity(),error))return false;
 constructed_=false;return true;
}
std::vector<std::uintptr_t> SceneManagerMapOwnerV2::children()const{
 std::vector<std::uintptr_t> result;for(const auto& child:children_)result.push_back(child.identity);return result;
}
}
