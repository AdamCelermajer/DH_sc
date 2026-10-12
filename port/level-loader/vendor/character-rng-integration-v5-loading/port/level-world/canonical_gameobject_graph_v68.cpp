#include "canonical_gameobject_graph_v68.hpp"
#include <utility>
namespace dh2::world {
namespace {bool required(const char* what,std::string& e){e=std::string("Required canonical object graph ")+what;return false;}}
CanonicalGameObjectGraphV68::CanonicalGameObjectGraphV68(CanonicalGameObjectGraphServicesV68 s):services_(std::move(s)){}
bool CanonicalGameObjectGraphV68::borrow(const std::shared_ptr<Entry>& entry,
 std::shared_ptr<void>& lease,CanonicalGameObjectBaseOwnerV1*& base,std::string& e)const{
 lease.reset();base=nullptr;
 if(!entry||!services_.owner||!services_.roots||!services_.current||!services_.current(e))return false;
 if(!entry->borrow||!entry->borrow(lease,base,e))return false;
 if(!lease||!base||base->identity()!=entry->identity)return required("same live class/base lease",e);
 return true;
}
bool CanonicalGameObjectGraphV68::visual(const std::shared_ptr<Entry>& entry,std::uintptr_t id,
 std::shared_ptr<RetainedGameObjectVisualV1>& out,std::string& e)const{
 out.reset();std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
 if(!borrow(entry,lease,base,e))return false;
 const auto* slot=base->pointer(0x2d8);
 if(entry->external_visual){if(!entry->external_visual(out,e))return false;}
 else out=entry->visual?entry->visual->attached():nullptr;
 if(!slot||*slot!=id||!out||reinterpret_cast<std::uintptr_t>(out.get())!=id)
  return required("same attached VisualObject2d8",e);
 return true;
}
bool CanonicalGameObjectGraphV68::observe_base_v78(std::uintptr_t id,CanonicalBaseBorrowV68 actual,std::string& e){
 if(!id||!actual||entries_.find(id)!=entries_.end())return required("sole constructor base admission",e);
 auto entry=std::make_shared<Entry>();entry->identity=id;entry->borrow=std::move(actual);entry->base_only_v78=true;
 std::shared_ptr<void> pin;CanonicalGameObjectBaseOwnerV1* base{};
 if(!borrow(entry,pin,base,e))return false;
 // Storage is the existing weak graph entry. Actual class/manager/journal
 // still own C1; this transport makes no native method or visual allocation.
 entries_.emplace(id,std::move(entry));e.clear();return true;
}
bool CanonicalGameObjectGraphV68::bind(std::uintptr_t id,CanonicalBaseBorrowV68 actual,
 GameObjectInitializationServicesV1& init,GameObjectSetPositionServicesV2& position,std::string& e){
 if(!id||!actual||!init.owner||!position.owner||!services_.owner||!services_.roots||!services_.visual.read_asset)
  return required("actual constructor/platform/scene/cache owners",e);
 auto found=entries_.find(id);std::shared_ptr<Entry> entry;
 std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
 if(found!=entries_.end()){
  entry=found->second;if(!entry->base_only_v78)return required("one resource binding per actual constructor",e);
  if(!borrow(entry,lease,base,e))return false;
  std::shared_ptr<void> proposed_pin;CanonicalGameObjectBaseOwnerV1* proposed{};
  if(!actual(proposed_pin,proposed,e))return false;
  if(!proposed_pin||proposed!=base||lease.owner_before(proposed_pin)||proposed_pin.owner_before(lease))
   return required("same admitted base during visual-resource upgrade",e);
 }else{
  entry=std::make_shared<Entry>();entry->identity=id;entry->borrow=std::move(actual);
  if(!borrow(entry,lease,base,e))return false;
 }
 entry->platform=init;entry->position=position;
 const auto weak=weak_from_this();const std::weak_ptr<Entry> weak_entry=entry;
 auto source=services_.visual;
 source.parent_is_animated=[weak,weak_entry](bool& value,std::string& error){
  const auto graph=weak.lock();const auto item=weak_entry.lock();std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
  if(!graph||!graph->borrow(item,lease,base,error))return false;
  if(!graph->services_.is_animated)return required("actual class IsAnimated virtual",error);
  return graph->services_.is_animated(*base,value,error);
 };
 source.update_pf=[weak,weak_entry](std::string& error){
  const auto graph=weak.lock();const auto item=weak_entry.lock();std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
  if(!graph||!graph->borrow(item,lease,base,error))return false;
  if(!item->platform.update_pf_object)return required("actual UpdatePFObject",error);
  return item->platform.update_pf_object(error);
 };
 entry->visual=std::make_shared<RetainedSceneVisualConnectionV3>(*base,std::move(source),services_.roots);
 entry->assets=std::make_unique<GameObjectVisualAssetOwnerV1>(*base,entry->visual->services(entry->visual));
 entry->base_only_v78=false;
 // Retain the reached resource prefix before any subsequent source callback.
 entries_.emplace(id,entry);
 auto scoped=[weak,weak_entry](std::shared_ptr<CanonicalGameObjectGraphV68>& graph,
  std::shared_ptr<Entry>& item,std::shared_ptr<void>& lease,CanonicalGameObjectBaseOwnerV1*& base,std::string& error){
  graph=weak.lock();item=weak_entry.lock();
  return graph&&graph->borrow(item,lease,base,error);
 };
 position.visual_sync=[weak,weak_entry](std::uintptr_t visual_id,std::string& error){
  const auto graph=weak.lock();std::shared_ptr<RetainedGameObjectVisualV1> visual;
  return graph&&graph->visual(weak_entry.lock(),visual_id,visual,error)&&visual->sync(error);
 };
 entry->position=position;
 init.set_position=[scoped](const float* p,bool destination,std::string& error){
  std::shared_ptr<CanonicalGameObjectGraphV68> graph;std::shared_ptr<Entry> item;std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
  return scoped(graph,item,lease,base,error)&&game_object_set_position_v2(*base,p,destination,item->position,error);
 };
 init.load_visual=[scoped](std::string& error){
  std::shared_ptr<CanonicalGameObjectGraphV68> graph;std::shared_ptr<Entry> item;std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
  return scoped(graph,item,lease,base,error)&&item->assets->load_visual(error);
 };
 init.visual_sync=position.visual_sync;
 init.visual_set_light_set=[weak,weak_entry](std::uintptr_t visual_id,std::int32_t light,std::string& error){
  const auto graph=weak.lock();std::shared_ptr<RetainedGameObjectVisualV1> visual;
  if(!graph||!graph->visual(weak_entry.lock(),visual_id,visual,error))return false;
  visual->store_light_set(light);return true;
 };
 init.visual_root=[weak,weak_entry](std::uintptr_t visual_id,std::uintptr_t& root,std::string& error){
  const auto graph=weak.lock();std::shared_ptr<RetainedGameObjectVisualV1> visual;
  if(!graph||!graph->visual(weak_entry.lock(),visual_id,visual,error))return false;
  root=visual->root_identity();return true;
 };
 init.node_from_name=[scoped](std::uintptr_t root,const char* name,std::uintptr_t& node,std::string& error){
  std::shared_ptr<CanonicalGameObjectGraphV68> graph;std::shared_ptr<Entry> item;std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
  if(!scoped(graph,item,lease,base,error))return false;
  const auto visual=item->visual->attached();
  if(!visual||visual->root_identity()!=root)return required("same scene root for named node",error);
  return visual->node_from_name(name,node,error);
 };
 e.clear();return true;
}
bool CanonicalGameObjectGraphV68::capture_visuals(std::vector<std::shared_ptr<RetainedGameObjectVisualV1>>& out,std::string& e)const{
 std::vector<std::shared_ptr<RetainedGameObjectVisualV1>> current;
 for(const auto& row:entries_){
  std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
  if(!borrow(row.second,lease,base,e))return false;
  auto* slot=base->pointer(0x2d8);if(!slot)return required("visual field during scene submission",e);
  if(!*slot)continue;
  std::shared_ptr<RetainedGameObjectVisualV1> visual;
  if(!this->visual(row.second,*slot,visual,e)||!visual->ready())return false;
  current.push_back(std::move(visual));
 }
 out=std::move(current);e.clear();return true;
}
bool CanonicalGameObjectGraphV68::borrow_base_v77(std::uintptr_t id,std::shared_ptr<void>& pin,CanonicalGameObjectBaseOwnerV1*& base,std::string& e)const{
 pin.reset();base=nullptr;const auto found=entries_.find(id);
 if(found==entries_.end())return required("registered canonical base receiver",e);
 return borrow(found->second,pin,base,e);
}
bool CanonicalGameObjectGraphV68::borrow_visual(std::uintptr_t id,std::shared_ptr<RetainedGameObjectVisualV1>& out,std::string& e)const{
 out.reset();for(const auto& row:entries_){
  std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
  if(!borrow(row.second,lease,base,e))return false;
  const auto* slot=base->pointer(0x2d8);
  if(slot&&*slot==id)return visual(row.second,id,out,e);
 }
 return required("registered canonical visual",e);
}
bool CanonicalGameObjectGraphV68::observe_visual(std::uintptr_t id,CanonicalBaseBorrowV68 actual,
 std::function<bool(std::shared_ptr<RetainedGameObjectVisualV1>&,std::string&)> visual,std::string& e){
 if(!id||!actual||!visual||entries_.find(id)!=entries_.end())return required("sole external visual admission",e);
 auto entry=std::make_shared<Entry>();entry->identity=id;entry->borrow=std::move(actual);entry->external_visual=std::move(visual);
 std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
 if(!borrow(entry,lease,base,e))return false;
 entries_.emplace(id,std::move(entry));e.clear();return true;
}
bool CanonicalGameObjectGraphV68::retire_observed_visual(std::uintptr_t id,std::string& e){
 const auto found=entries_.find(id);
 if(found==entries_.end()||!found->second->external_visual)return required("actual external visual entry",e);
 std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};
 if(!borrow(found->second,lease,base,e))return false;
 const auto* slot=base->pointer(0x2d8);
 if(!slot||*slot)return required("source VisualObject2d8 clear before external retirement",e);
 std::shared_ptr<RetainedGameObjectVisualV1> visual;
 if(!found->second->external_visual(visual,e))return false;
 if(visual&&visual->ready()&&visual->root_identity())return required("actual external scene release before retirement",e);
 entries_.erase(found);e.clear();return true;
}
bool CanonicalGameObjectGraphV68::release_resources(std::uintptr_t id,std::string& e){
 const auto found=entries_.find(id);if(found==entries_.end())return required("actual resource teardown entry",e);
 const auto entry=found->second;
 if(entry->external_visual)return required("container's own qualified visual teardown",e);
 if(entry->releasing)return required("non-reentrant resource teardown",e);
 std::shared_ptr<void> lease;CanonicalGameObjectBaseOwnerV1* base{};if(!borrow(entry,lease,base,e))return false;
 if(entry->base_only_v78){
  const auto* visual=base->pointer(0x2d8);if(!visual||*visual)return required("NULL visual on uninitialized admitted base",e);
  entries_.erase(found);e.clear();return true; // qualified absent visual only
 }
 entry->releasing=true;struct Scope{bool& value;~Scope(){value=false;}} scope{entry->releasing};
 if(!entry->assets->set_visual(std::uintptr_t{0},e)||!entry->visual->discard_unattached(e)||!entry->visual->discard_failed(e))return false;
 entries_.erase(found);e.clear();return true;
}
}

namespace dh2::world {
bool CanonicalGameObjectGraphV68::borrow_native_body_v90(std::uintptr_t id,std::shared_ptr<void>& pin,
 physical::NativeBody*& out,std::string& e)const{
 pin.reset();out=nullptr;std::shared_ptr<void> receiver;CanonicalGameObjectBaseOwnerV1* base{};
 if(!borrow_base_v77(id,receiver,base,e)||!base)return false;
 auto physical=base->pointer(0x2dc);if(!physical||!*physical||!services_.native_body_v90){e="Required SAME assigned GameObject physical facet";return false;}
 std::shared_ptr<void> resource;if(!services_.native_body_v90(*base,resource,out,e)||!resource||!out||!out->body)return false;
 struct Lease {std::shared_ptr<void> receiver,resource;};pin=std::make_shared<Lease>(Lease{std::move(receiver),std::move(resource)});
 e.clear();return true;
}
}
