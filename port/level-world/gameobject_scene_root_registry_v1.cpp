#include "gameobject_scene_root_registry_v1.hpp"
#include <algorithm>
#include <cstring>
#include <exception>
#include <cmath>
namespace dh2::world {
bool GameObjectSceneRootRegistryV1::source_find_light_v113(const std::string& name,std::shared_ptr<NativeLightV113>& out,std::string& e)const{
 out.reset();const auto actual=children_;for(const auto& entry:actual)if(entry.root.light_by_name_v113){std::shared_ptr<NativeLightV113> found;if(!entry.root.light_by_name_v113(name,found,e))return false;if(found){out=std::move(found);break;}}
 e.clear();return true; //same GetNode miss/wrong subtype -> NULL
}
bool GameObjectSceneRootRegistryV1::add_child(GameObjectSceneRootBorrowV1 b,std::string& e){
 // addChild ignores NULL/self. For non-null actual nodes all field producers
 // are mandatory; foreign parent migration is outside this scoped owner.
 if(!b.identity||b.identity==identity_)return true;
 if(!identity_||!b.owner||!b.flags11c||!b.parentec||!b.notify_visibility||!b.remove_animators){e="Required SAME scene root fields/visibility/animator owner";return false;}
 if(*b.parentec&&*b.parentec!=identity_){e="Required actual previous scene parent RemoveChild owner";return false;}
 const bool reference=bool(b.acquire_parent_reference_v110);
 if(reference!=bool(b.release_parent_reference_v110)){e="Scene root parent reference hooks must be paired";return false;}
 if(std::any_of(parent_prefixes_v110_.begin(),parent_prefixes_v110_.end(),[&](const auto& prefix){return prefix->root.identity==b.identity;})){
  e="Root addChild has an outstanding native reference prefix";return false;
 }
 auto p=std::find_if(children_.begin(),children_.end(),[&](const Entry& v){return v.root.identity==b.identity;});
 if(reference&&*b.parentec==identity_&&p==children_.end()){e="Intrusive parent field has no matching actual Scene membership";return false;}
 if(p!=children_.end()&&(p->root.owner.owner_before(b.owner)||b.owner.owner_before(p->root.owner)||
    p->root.parentec!=b.parentec||p->root.flags11c!=b.flags11c||p->parent_reference_owned_v110!=reference)){
  e="Root addChild identity was reused with another native loan";return false;
 }
 std::shared_ptr<ParentReferencePrefixV110> prefix;
 try{
  //Native vector capacity/receipt allocation precedes the original grab so a
  //host allocation failure cannot lose an acquired intrusive reference.
  children_.reserve(children_.size()+1);
  if(reference){prefix=std::make_shared<ParentReferencePrefixV110>();prefix->root=b;parent_prefixes_v110_.push_back(prefix);
   prefix->busy=true;bool acquired{};
   try{acquired=b.acquire_parent_reference_v110(e);}catch(...){prefix->busy=false;prefix->unproved=true;prefix->failure="Native parent grab threw with unknown reference receipt";throw;}
   prefix->busy=false;
   if(!acquired){parent_prefixes_v110_.remove(prefix);if(e.empty())e="Actual scene parent grab failed";return false;}
   prefix->acquired=true;
  }
  //598880..8a4: new grab precedes old remove/drop. Removal may reenter other
  //nodes, so re-find membership rather than retaining vector iterators.
  if(std::any_of(children_.begin(),children_.end(),[&](const auto& child){return child.root.identity==b.identity;})&&
     !unlink_parent_v110(b.identity,e,false))return false;
  children_.push_back({std::move(b),false,reference});
  if(prefix){prefix->acquired=false;parent_prefixes_v110_.remove(prefix);} //reference transferred to actual membership
  //Pin callback receiver independently of children_ mutations.
  const auto root=children_.back().root;
  *root.parentec=identity_;*root.flags11c|=0x40u;
  if(root.scene_manager_changed&&!root.scene_manager_changed(scene_manager_identity_v16(),e))return false;
  notify_hierarchy_changed();return root.notify_visibility((root_flags_&1u)!=0,e);
 }catch(const std::exception& ex){e=ex.what();return false;}catch(...){e="Source addChild native reference/publication threw";return false;}
}
bool GameObjectSceneRootRegistryV1::borrow_registered_root_v110(std::uintptr_t id,GameObjectSceneRootBorrowV1& out,std::string& e)const{
 const auto found=std::find_if(children_.begin(),children_.end(),[id](const auto& entry){return entry.root.identity==id;});
 if(!id||found==children_.end()||!found->root.owner||!found->root.parentec||*found->root.parentec!=identity_){e="Required SAME actual registered root loan";return false;}
 out=found->root;e.clear();return true;
}
bool GameObjectSceneRootRegistryV1::release_parent_prefix_v110(const std::shared_ptr<ParentReferencePrefixV110>& prefix,std::string& e){
 if(!prefix||prefix->busy){e="Native scene parent reference release reentered";return false;}
 if(prefix->unproved){e=prefix->failure;return false;}
 if(prefix->released||!prefix->acquired){parent_prefixes_v110_.remove(prefix);e.clear();return true;}
 if(!prefix->root.owner||!prefix->root.parentec||*prefix->root.parentec||!prefix->root.release_parent_reference_v110){e="Pending parent reference still has membership or lost its actual owner";return false;}
 prefix->busy=true;struct Scope{bool& busy;~Scope(){busy=false;}}scope{prefix->busy};
 try{
  if(!prefix->root.release_parent_reference_v110(e)){if(e.empty())e="Actual parent drop/native D1 incomplete";prefix->failure=e;return false;}
  prefix->released=true;prefix->acquired=false; //completed BEFORE removing diagnostic storage
  parent_prefixes_v110_.remove(prefix);e.clear();return true;
 }catch(const std::exception& ex){prefix->unproved=true;prefix->failure=ex.what();e=prefix->failure;return false;}
 catch(...){prefix->unproved=true;prefix->failure="Native parent drop threw with unknown reference receipt";e=prefix->failure;return false;}
}
bool GameObjectSceneRootRegistryV1::drain_parent_prefixes_v110(std::uintptr_t id,std::string& e){
 //Snapshot pins receipts through native callbacks that may release other
 //roots. This copies no source reference and advances no scene clock.
 const auto prefixes=parent_prefixes_v110_;
 for(const auto& prefix:prefixes)if((!id||prefix->root.identity==id)&&!release_parent_prefix_v110(prefix,e))return false;
 e.clear();return true;
}
bool GameObjectSceneRootRegistryV1::unlink_parent_v110(std::uintptr_t id,std::string& e,bool notify){
 auto p=std::find_if(children_.begin(),children_.end(),[id](const auto& entry){return entry.root.identity==id;});
 if(p==children_.end()||!p->root.owner||!p->root.parentec||*p->root.parentec!=identity_){e="Required SAME registered root parent reference";return false;}
 std::shared_ptr<ParentReferencePrefixV110> prefix;
 try{if(p->parent_reference_owned_v110){prefix=std::make_shared<ParentReferencePrefixV110>();prefix->root=p->root;prefix->acquired=true;parent_prefixes_v110_.push_back(prefix);}}
 catch(const std::exception& ex){e=ex.what();return false;}
 //59701c..6050: unlink/clear parent BEFORE the actual intrusive drop.
 *p->root.parentec=0;children_.erase(p);if(notify)notify_hierarchy_changed();
 return prefix?release_parent_prefix_v110(prefix,e):true;
}
bool GameObjectSceneRootRegistryV1::release_visual_root(std::uintptr_t id,std::string& e){
 auto p=std::find_if(children_.begin(),children_.end(),[&](const Entry& v){return v.root.identity==id;});
 if(p==children_.end()){e="Required registered SAME visual root release receiver";return false;}
 // VisualD1 virtual74 then virtual68/drop. Keep progress if a required
 // animator service fails; do not unlink a still-live registered receiver.
 if(!p->animators_removed){const auto root=p->root;if(!root.remove_animators(e))return false;
  p=std::find_if(children_.begin(),children_.end(),[id](const auto& entry){return entry.root.identity==id;});
  if(p==children_.end()||p->root.parentec!=root.parentec||p->root.owner.owner_before(root.owner)||root.owner.owner_before(p->root.owner)){e="Visual root membership changed during removeAnimators";return false;}
  p->animators_removed=true;notify_hierarchy_changed();
 }
 return unlink_parent_v110(id,e,false);
}
bool GameObjectSceneRootRegistryV1::remove_root_parent_reference_v1(std::uintptr_t id,std::string& e){
 // Distinct from VisualD1 virtual74/removeAnimators before68. Native Scene
 // removal drops its parent ownership before the reached child D1/removeAll.
 return unlink_parent_v110(id,e);
}
std::vector<std::uintptr_t> GameObjectSceneRootRegistryV1::roots()const{std::vector<std::uintptr_t> out;for(auto& p:children_)out.push_back(p.root.identity);return out;}
bool GameObjectSceneRootRegistryV1::source_find_modular_v114(skinning::SourceModularSkinBorrowV114& out,std::string& e)const{
 out={};const auto roots=children_; //pins actual receiver across source callbacks
 for(const auto& entry:roots){
  if(!entry.root.modular_receivers_v114){e="Required actual SceneManager SearchByType modular descendant loan";return false;}
  std::vector<skinning::SourceModularSkinBorrowV114> found;if(!entry.root.modular_receivers_v114(found,e))return false;
  for(auto& receiver:found){if(receiver.root.expired()||!receiver.mesh){e="Retired actual modular mesh search receiver";return false;}out=std::move(receiver);}
 }
 e.clear();return true; //471964..978 deliberately keeps LAST source match.
}
bool GameObjectSceneRootRegistryV1::set_active_camera_v13(GameObjectSceneCameraBorrowV13 next,std::string& e){
 if(active_camera_e4_.identity==next.identity)return true;
 if(next.identity){if(!next.owner||!next.grab||!next.drop){e="Required actual SceneManager camera owner/reference methods";return false;}if(!next.grab(e))return false;}
 // Original rereads the old slot after grabbing next. Retain the callback
 // receiver locally so reentrant drop can replace the manager safely; source
 // outer store still wins after that callback returns.
 auto old=active_camera_e4_;
 if(old.identity){if(!old.owner||!old.drop){e="Required actual old active-camera drop receiver";return false;}if(!old.drop(e))return false;}
 active_camera_e4_=std::move(next);fields_.render_dirty289=1;return true;
}
bool GameObjectSceneRootRegistryV1::source_scene_phase_v69(std::uint64_t epoch,std::uint32_t timer,std::string& e){
 if(scene_failed_v69_){e=scene_error_v69_;return false;}
 auto failed=[&](const char* message){scene_failed_v69_=true;
  if(e.empty())e=message;scene_error_v69_=e;return false;};
 if(scene_busy_v69_)return failed("Scene animator phase reentered its SAME owner");
 if(scene_epoch_produced_v69_&&scene_epoch_v69_==epoch){e.clear();return true;}
 if(scene_epoch_produced_v69_&&epoch<scene_epoch_v69_)return failed("Native draw provenance epoch moved backward");
 struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(scene_busy_v69_);
 // Record prefix before callbacks: a partial failed traversal cannot replay
 // already-delivered animation events just because another consumer draws.
 scene_epoch_v69_=epoch;scene_epoch_produced_v69_=true;
 try{
  if(fields_.hierarchy_dirty288||collected_roots_v69_.size()!=children_.size()){
   collected_roots_v69_.clear();collected_roots_v69_.reserve(children_.size());
   for(const auto& child:children_)collected_roots_v69_.push_back(child.root);
   fields_.hierarchy_dirty288=0;
  }
  // Pin actual collected receiver leases across callbacks. Native lifecycle
  // safety skips a root explicitly unregistered by an earlier callback;
  // no offscreen, quality, dt or synthetic animation-state skip is added.
  const auto actual=collected_roots_v69_;
  for(const auto& root:actual){
   const auto current=std::find_if(children_.begin(),children_.end(),[&](const Entry& value){return value.root.identity==root.identity;});
   if(current==children_.end())continue;
   if(!root.owner||!root.flags11c||!root.parentec||*root.parentec!=identity_||!root.scene_phase_v69)
    return failed("Required SAME registered root animator/cache phase provider");
   if(!root.scene_phase_v69(timer,e)||scene_failed_v69_)return failed("Reached actual root animator/cache phase failed");
  }
  e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return failed("Scene phase allocation/provider exception");}
 catch(...){return failed("Unknown actual scene phase exception");}
}
bool GameObjectSceneRootRegistryV1::source_update_v102(float delta,bool optimized,
 const SceneUpdateTransportV102& transport,std::string& e){
 if(scene_failed_v69_){e=scene_error_v69_;return false;}
 const auto fail=[&](const char* why){scene_failed_v69_=true;if(e.empty())e=why;scene_error_v69_=e;return false;};
 if(scene_busy_v69_)return fail("Actual CSceneManager.update reentered its animator traversal");
 struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} busy(scene_busy_v69_);
 //58b9f0: sentinel reads Timer, otherwise add to the SAME float254 cell.
 //Store precedes collect/animation and survives any reached provider failure.
 if(delta==-123456.f){
  std::uint32_t timer{};
  if(!transport.provider||!transport.timer||!transport.timer(timer,e))return fail("Required original Scene update Timer");
  time254_v102_=static_cast<float>(timer);
 }else time254_v102_=time254_v102_+delta;
 if(!std::isfinite(time254_v102_)||time254_v102_<0.f||static_cast<double>(time254_v102_)>=4294967296.0)
  return fail("Scene float254 outside supported source unsigned-time conversion");
 const auto time=static_cast<std::uint32_t>(time254_v102_);
 try{
  if(fields_.hierarchy_dirty288){
   collected_roots_v69_.clear();collected_roots_v69_.reserve(children_.size());
   for(const auto& child:children_)collected_roots_v69_.push_back(child.root);
   update_nodes_v102_.clear();update_nodes_produced_v102_=false;
   if(transport.collect_nodes){
    if(!transport.provider||!transport.collect_nodes(update_nodes_v102_,e))return fail("Actual Scene collectAllNodes failed");
    update_nodes_produced_v102_=true;
   }
   fields_.hierarchy_dirty288=0;
  }
  if(optimized){
   //Virtual18 dispatch is distinct from recursive root virtual14. Never
   //substitute model-root animation for an unavailable descendant provider.
   if(!update_nodes_produced_v102_){
    if(!transport.provider||!transport.collect_nodes||!transport.collect_nodes(update_nodes_v102_,e))
     return fail("Required actual complete node collection for optimized Scene update");
    update_nodes_produced_v102_=true;
   }
   for(std::size_t i=0;i<update_nodes_v102_.size();++i){
    const auto node=update_nodes_v102_[i];
    if(!node.owner||!node.identity||!node.on_update18||!node.on_update18(time,e))
     return fail("Required actual collected node virtual18");
   }
  }else{
   //Source root4.virtual14 owns recursive animator/transform traversal.
   //The retained native roots supply that traversal over their same assets,
   //controllers and mutable pose. This is not a draw-epoch/physics update.
   const auto roots=collected_roots_v69_;
   for(const auto& root:roots){
    const auto live=std::find_if(children_.begin(),children_.end(),[&](const Entry& entry){return entry.root.identity==root.identity;});
    if(live==children_.end())continue;
    if(!root.owner||!root.flags11c||!root.parentec||*root.parentec!=identity_||!root.scene_phase_v69||
       !root.scene_phase_v69(time,e))return fail("Actual scene-root virtual14 traversal failed");
   }
  }
  e.clear();return true;
 }catch(const std::exception& error){e=error.what();return fail("Native Scene update traversal exception");}
}
bool GameObjectSceneRootRegistryV1::source_register_nodes_v69(bool native_force,std::uint8_t script30,
 const SceneRegistrationTransportV69& transport,std::string& e){
 if(!transport.provider||!transport.clear_render_lists||!transport.register_nodes||!transport.refresh_cached_nodes){
  e="Required actual SceneManager render registration/cache transport";return false;
 }
 // Exact original357ea4 counter/needRegister prefix. The actual ScriptManager
 // singleton is an in-place34-byte object; GOT1a20+30 reads its real byte30.
 ++fields_.counter440;
 if(source_need_register_v69_||script30){
  fields_.counter440=0;
  source_need_register_v69_=script30?true:fields_.force448!=0;
 }else source_need_register_v69_=fields_.force448!=0;
 bool rebuild=native_force||fields_.render_dirty289;
 if(!rebuild){
  std::int32_t counter,cadence;std::memcpy(&counter,&fields_.counter440,4);std::memcpy(&cadence,&fields_.cadence444,4);
  // Original dirty branch returns directly to registration before division.
  if(!cadence){e="Original SceneManager cadence444 division by zero rejected";return false;}
  // ARM signed idivmod has remainder0 for INT_MIN/-1; avoid native UB.
  const auto remainder=(counter==INT32_MIN&&cadence==-1)?0:counter%cadence;
  rebuild=remainder==0;
 }
 if(rebuild){
  if(!transport.clear_render_lists(e)||!transport.register_nodes(e))return false;
  fields_.force448=0;fields_.counter440=0;fields_.render_dirty289=0;
 }else if(!transport.refresh_cached_nodes(e))return false;
 e.clear();return true;
}
bool GameObjectSceneRootRegistryV1::retire_source_cached_aliases_v106(std::string& e){
 if(scene_busy_v69_||!children_.empty()){e="Scene clear requires actual class/map root teardown, with no active animator delivery";return false;}
 // Native cached traversal leases must be released even after an earlier
 // scene-phase error. This adds no source animation/counter advancement.
 if(!drain_parent_prefixes_v110(0,e))return false;
 collected_roots_v69_.clear();update_nodes_v102_.clear();update_nodes_produced_v102_=false;
 source_clear_preloaded_scenes_v81();notify_hierarchy_changed();e.clear();return true;
}
}


namespace dh2::world {
bool GameObjectSceneRootRegistryV1::retire_unpublished_root_aliases_v92(std::uintptr_t id,std::string& e){
 if(scene_busy_v69_){e="Native root alias retirement requires actual owning-thread traversal barrier";return false;}
 if(std::any_of(children_.begin(),children_.end(),[id](const auto& child){return child.root.identity==id;})){e="Actual scene still publishes native visual root";return false;}
 if(!drain_parent_prefixes_v110(id,e))return false;
 collected_roots_v69_.erase(std::remove_if(collected_roots_v69_.begin(),collected_roots_v69_.end(),[id](const auto& child){return child.identity==id;}),collected_roots_v69_.end());
 // Hierarchy changes invalidate optimized descendants as a domain. This drops
 // actual cache leases, not native nodes; next original collection reproduces
 // SAME graph. Keep source epoch/time/clock unchanged to avoid animation replay.
 update_nodes_v102_.clear();update_nodes_produced_v102_=false;e.clear();return true;
}
}
