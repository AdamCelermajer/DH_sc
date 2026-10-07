#include "scene_manager_map_owner_v2.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
#include <algorithm>
namespace dh2::world {
bool SceneManagerMapOwnerV2::source_child_detached_v93(std::string& e){auto manager=scene_manager_.lock();if(!manager){e="Retired SAME map SceneManager during child detach";return false;}manager->notify_hierarchy_changed();e.clear();return true;}
std::uintptr_t SceneManagerMapOwnerV2::scene_manager_identity_v1()const noexcept{auto m=scene_manager_.lock();return m?m->scene_manager_identity_v16():0;}
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
 if(native_d1_attempted_v1_){error="Cannot reparent a native-destroyed map group";return false;}
 auto manager=scene_manager_.lock();
 if(!manager){error="Scene map requires SAME SceneManager root owner";return false;}
 if(!constructed_){
  auto lease=weak_from_this().lock();
  if(!lease){error="Scene map requires retained factory lease";return false;}
  constructed_=true; // source28c publication precedes root.addChild
  GameObjectSceneRootBorrowV1 node;node.owner=lease;node.identity=identity();node.flags11c=&flags_;node.parentec=&parent_;
  node.notify_visibility=[this](bool value,std::string& e){return visibility(value,e);};
  node.modular_receivers_v114=[](auto&,std::string& e){e.clear();return true;}; //group CSceneNode itself; actual registered descendants retain their separate type loans
  node.remove_animators=[this](std::string& e){
   // Source CSceneNode has an empty animator list; child animator destruction
   // is required if this group has acquired actual child nodes.
   if(!children_.empty()){e="Scene map release requires child animator removal continuation";return false;}return true;
  };
  node.scene_phase_v69=[weak=weak_from_this()](std::uint32_t stamp,std::string& e){
   auto map=weak.lock();if(!map){e="Released SAME Scene map phase";return false;}return map->source_scene_phase_v69(stamp,e);
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
bool SceneManagerMapOwnerV2::source_scene_phase_v69(std::uint32_t stamp,std::string& error){
 if(!constructed_||scene_manager_.expired()){error="Required actual constructed Scene map group";return false;}
 if(((flags_&0x400u)&&!(flags_&1u))||!(flags_&0x200u)){error.clear();return true;}
 // This actual CSceneNode group has its source C1-empty animator list and
 // identity relative transform. It still updates its own absolute flags.
 if(flags_&0x5eu)flags_=(flags_|0x120u)&~0x50u;
 const auto actual=children_;
 for(const auto& child:actual){
  const auto found=std::find_if(children_.begin(),children_.end(),[&](const auto& row){return row.identity==child.identity;});
  if(found==children_.end())continue;
  if(!child.owner||!child.parentec||*child.parentec!=identity()||!child.scene_phase_v69){
   error="Required actual Scene map child phase/cache provider";return false;
  }
  if(!child.scene_phase_v69(stamp,error))return false;
 }
 error.clear();return true;
}
bool SceneManagerMapOwnerV2::release_scene_source_v1(const SceneMapDestructionV1& services,std::string& e){
 if(native_d1_complete_v1_){e.clear();return true;}
 if(native_d1_attempted_v1_){e=native_d1_failure_v1_.empty()?"Scene map D1 cannot replay/reenter a reached prefix":native_d1_failure_v1_;return false;}
 // Native Scene28c has no published group before first AddNodeToMap. This
 // legitimate absence invokes no class D0; its unused host adapter can expire.
 if(!constructed_&&children_.empty()){e.clear();return true;}
 if(!services.owner||!services.quiesce||!services.child_parent_drop||!services.native_map_d1_prefix||!services.native_map_d1_tail){e="Required actual Scene/map quiescence and native parent/map D1 leaves";return false;}
 if(!services.quiesce(*this,e))return false;
 auto manager=scene_manager_.lock();if(!manager){e="Required SAME SceneManager before map parent drop";return false;}
 native_d1_attempted_v1_=true;
 auto fail=[&]{native_d1_failure_v1_=e.empty()?"Required actual Scene map source teardown":e;e=native_d1_failure_v1_;return false;};
 // Original ISNode.remove59706c reaches parent.removeChild; the root parent
 // NULL/reference drop precedes this map's native D1 and removeAll5987e8.
 if(!manager->remove_root_parent_reference_v1(identity(),e)||!services.native_map_d1_prefix(*this,e))return fail();
 while(!children_.empty()){
  auto child=children_.front(); // modern diagnostic pin, not another native grab
  if(!child.owner||!child.parentec||*child.parentec!=identity()){e="Require SAME actual map child parent before source drop";return fail();}
  // removeAll598818..59882c clears links/parentec before child.drop. Existing
  // map remove supplies the SAME list/parent mutation, then Main releases its
  // native parent reference/transport alias; PFFloor still owns mesh40.
  if(!remove(child.identity,e)||!services.child_parent_drop(child,e))return fail();
 }
 if(!services.native_map_d1_tail(*this,e))return fail();
 if(parent_||!children_.empty()){e="Native map D1 retained/replaced child/root membership";return fail();}
 constructed_=false;native_d1_complete_v1_=true;e.clear();return true;
}
bool SceneManagerMapOwnerV2::native_d1_prefix_v106(std::string& e){
 // CSceneNode derived D1 has no separate native resource allocation; ISNode
 // parent/children/name/reference tail is executed by the same journal.
 if(!native_d1_attempted_v1_||parent_||scene_manager_.expired()){e="Map native D1 prefix requires the actual unparented source receiver";return false;}
 e.clear();return true;
}
bool SceneManagerMapOwnerV2::native_d1_tail_v106(std::string& e){
 if(!native_d1_attempted_v1_||parent_||!children_.empty()){e="Map native D1 tail requires completed real child drops";return false;}
 std::vector<SceneMapNodeBorrowV2>{}.swap(children_);std::string{}.swap(name24_v93_);
 scene_manager_.reset();e.clear();return true;
}
}

