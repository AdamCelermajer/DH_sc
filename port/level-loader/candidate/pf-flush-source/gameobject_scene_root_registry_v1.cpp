#include "gameobject_scene_root_registry_v1.hpp"
#include <algorithm>
#include <cstring>
#include <exception>
namespace dh2::world {
bool GameObjectSceneRootRegistryV1::add_child(GameObjectSceneRootBorrowV1 b,std::string& e){
 // addChild ignores NULL/self. For non-null actual nodes all field producers
 // are mandatory; foreign parent migration is outside this scoped owner.
 if(!b.identity||b.identity==identity_)return true;
 if(!identity_||!b.owner||!b.flags11c||!b.parentec||!b.notify_visibility||!b.remove_animators){e="Required SAME scene root fields/visibility/animator owner";return false;}
 if(*b.parentec&&*b.parentec!=identity_){e="Required actual previous scene parent RemoveChild owner";return false;}
 auto p=std::find_if(children_.begin(),children_.end(),[&](const Entry& v){return v.root.identity==b.identity;});
 if(p!=children_.end())children_.erase(p); // source grab/remove/reappend order
 children_.push_back({std::move(b),false});auto& root=children_.back().root;
 *root.parentec=identity_;*root.flags11c|=0x40u; // setParent source field/stale transform
 if(root.scene_manager_changed&&!root.scene_manager_changed(scene_manager_identity_v16(),e))return false;
 fields_.hierarchy_dirty288=1;fields_.render_dirty289=1;
 return root.notify_visibility((root_flags_&1u)!=0,e);
}
bool GameObjectSceneRootRegistryV1::release_visual_root(std::uintptr_t id,std::string& e){
 auto p=std::find_if(children_.begin(),children_.end(),[&](const Entry& v){return v.root.identity==id;});
 if(p==children_.end()){e="Required registered SAME visual root release receiver";return false;}
 // VisualD1 virtual74 then virtual68/drop. Keep progress if a required
 // animator service fails; do not unlink a still-live registered receiver.
 if(!p->animators_removed){if(!p->root.remove_animators(e))return false;p->animators_removed=true;fields_.hierarchy_dirty288=1;fields_.render_dirty289=1;}
 if(*p->root.parentec!=identity_){e="Canonical scene root parent changed without source RemoveChild";return false;}
 *p->root.parentec=0;children_.erase(p);return true;
}
bool GameObjectSceneRootRegistryV1::remove_root_parent_reference_v1(std::uintptr_t id,std::string& e){
 auto p=std::find_if(children_.begin(),children_.end(),[&](const Entry& node){return node.root.identity==id;});
 if(p==children_.end()||!p->root.owner||!p->root.parentec||*p->root.parentec!=identity_){e="Required SAME registered root parent reference";return false;}
 // Distinct from VisualD1 virtual74/removeAnimators before68. Native Scene
 // removal drops its parent ownership before the reached child D1/removeAll.
 *p->root.parentec=0;children_.erase(p);fields_.hierarchy_dirty288=1;fields_.render_dirty289=1;e.clear();return true;
}
std::vector<std::uintptr_t> GameObjectSceneRootRegistryV1::roots()const{std::vector<std::uintptr_t> out;for(auto& p:children_)out.push_back(p.root.identity);return out;}
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
}
