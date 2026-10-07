#include "catalog_environment_v67.hpp"
#include <exception>
#include <cstring>
namespace dh2::loader {
bool CatalogEnvironmentV67::source_borrow_v113(std::uintptr_t id,std::shared_ptr<world::CanonicalLightPointV53>& out,std::string& e){
 if(release_busy_||!factory_){e="Actual LightPoint factory is in source delivery";return false;}
 for(const auto& record:factory_->records())if(record&&record->owner&&record->owner->identity()==id){out=std::shared_ptr<world::CanonicalLightPointV53>(record,record->owner.get());e.clear();return true;}
 e="Actual SAME LightPoint source receiver absent";return false;
}
namespace {
struct ReleaseBusyV67 {bool& busy;explicit ReleaseBusyV67(bool& b):busy(b){busy=true;}~ReleaseBusyV67(){busy=false;}};
template<class A,class B>bool same_owner(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){
 return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
bool borrow(const ScopeV67& s,const LightEnvironmentLeavesV67& p,LightDeliveryV67& b,std::string& e){
 b={s.actual_world.lock(),s.level.lock(),s.objects.lock(),s.properties};
 b.runtime_owner=p.owner;
 if(!b.actual_world||!b.level||!b.objects||!b.properties){e="LightPoint actual World/Level/manager lifetime expired";return false;}
 return p.validate_current(b,e);
}
template<class F>bool leaf(const ScopeV67& s,const LightEnvironmentLeavesV67& p,F call,std::string& e){
 LightDeliveryV67 b;if(!borrow(s,p,b,e))return false;
 if(!call(b))return false;
 // Validate again while the same transient actual owner pins still exist.
 return p.validate_current(b,e);
}
bool node_lifetime(const ScopeV67& s,const world::LightNodeBorrowV53& n,std::string& e){
 // The journal/root may retain the actual node. Its passive pin must not in
 // turn retain the World that owns this journal/catalog.
 if(same_owner(n.owner,s.actual_world.lock())||same_owner(n.owner,s.level.lock())||same_owner(n.owner,s.objects.lock())){
  e="LightPoint native node pin must not retain containing World/Level/manager";return false;
 }
 return true;
}
template<class F>bool receiver_leaf(const ScopeV67& s,const LightEnvironmentLeavesV67& p,
 const std::weak_ptr<CanonicalLightPointRecordV53>& weak,F call,std::string& e){
 auto record=weak.lock();if(!record||!record->owner){e="LightPoint SAME receiver lifetime expired";return false;}
 return leaf(s,p,[&](auto& b){b.receiver=record->owner.get();return call(b);},e);
}
world::LightPointInitServicesV53 services(const ScopeV67& s,const std::shared_ptr<const LightEnvironmentLeavesV67>& p,std::weak_ptr<CanonicalLightPointRecordV53> record){
  world::LightPointInitServicesV53 v;v.owner=s.services_owner;v.names=p->names;
 v.bind_objectbase_dtor=[s,p,record](auto id,auto& out,auto& error){
  return receiver_leaf(s,*p,record,[&](const auto& b){
   if(b.receiver->identity()!=id){error="LightPoint base continuation receiver identity mismatch";return false;}
   return p->bind_objectbase_dtor(b,id,out,error);},error);};
 v.first_scene_light=[s,p,record](const auto& dae,const char* path,bool a,bool z,auto& n,auto& e){
  if(!receiver_leaf(s,*p,record,[&](const auto& b){return p->first_scene_light(b,dae,path,a,z,n,e);},e))return false;return node_lifetime(s,n,e);};
 v.construct_light_node=[s,p,record](bool a,auto& n,auto& e){
  if(!receiver_leaf(s,*p,record,[&](const auto& b){return p->construct_light_node(b,a,n,e);},e))return false;return node_lifetime(s,n,e);};
 v.attach_root=[s,p,record](const auto& n,auto& e){
  return receiver_leaf(s,*p,record,[&](const auto& b){return p->attach_root(b,n,e);},e);};
 v.add_automatic=[s,p,record](auto id,const auto& n,auto& e){
  return receiver_leaf(s,*p,record,[&](const auto& b){return p->add_automatic(b,id,n,e);},e);};
 v.borrow_light_parameters=[s,p,record](const auto& n,auto& f,auto& e){
  if(!receiver_leaf(s,*p,record,[&](const auto& b){return p->borrow_light_parameters(b,n,f,e);},e))return false;
  if(!f.validate_current){e="LightPoint actual CLight validator absent";return false;}
  // Actual CLight pointers remain guarded during SyncData, including the final
  // post-write validation. Preserve the provider's own native cell validator.
  auto native=f.validate_current;f.validate_current=[s,p,record,native](auto& error){
   return receiver_leaf(s,*p,record,[&](const auto&){return native(error);},error);};return true;};
 v.debug_switch=[s,p,record](const char* n,bool& o,auto& e){
  return receiver_leaf(s,*p,record,[&](const auto& b){return p->debug_switch(b,n,o,e);},e);};
 v.set_type=[s,p,record](const auto& n,std::uint16_t t,auto& e){
  return receiver_leaf(s,*p,record,[&](const auto& b){return p->set_type(b,n,t,e);},e);};
 v.refresh_attachment=[s,p,record](auto id,const auto& n,auto& names,auto& e){
  if(&names!=p->names.get()){e="LightPoint requires SAME existing light-set Names owner";return false;}
  return receiver_leaf(s,*p,record,[&](const auto& b){return p->refresh_attachment(b,id,n,names,e);},e);};
 v.bind_attachment_v113=[s,p,record](auto& out,auto& e){if(!p->bind_attachment_v113){e="Actual LightPoint attachment frame provider required";return false;}return receiver_leaf(s,*p,record,[&](const auto& b){return p->bind_attachment_v113(b,out,e);},e);};
 return v;
}
}
bool CatalogEnvironmentV67::create(ScopeV67 s,std::shared_ptr<const LightEnvironmentLeavesV67> p,
 std::shared_ptr<CatalogEnvironmentV67>& out,PartV67& part,std::string& e){
 if(!p||!s.services_owner||(!same_owner(p->owner,s.services_owner)||p->owner.get()!=s.services_owner.get())||!p->names||!p->validate_current||!p->bind_objectbase_dtor||
    !p->first_scene_light||!p->construct_light_node||!p->attach_root||!p->add_automatic||
    !p->borrow_light_parameters||!p->debug_switch||!p->set_type||!p->refresh_attachment||
    !p->retain_release_record||!p->release_completed){e="Production LightPoint requires genuine scene/light-set/CLight/release journal leaves";return false;}
 LightDeliveryV67 b;if(!borrow(s,*p,b,e))return false;
 for(const auto& pin:{p->owner,std::shared_ptr<void>(p->names)}){
  if(same_owner(pin,b.actual_world)||same_owner(pin,b.level)||same_owner(pin,b.objects)){
   e="LightPoint services/Names must not own containing World/Level/manager";return false;}
 }
 const world::CanonicalFactoryEntryV1* original=nullptr;
 for(const auto& x:world::canonical_factories_v1())if(x.name&&!std::strcmp(x.name,"LightPoint")&&x.original_address==0x34115c)original=&x;
 if(!original){e="Actual LightPoint34115c factory entry unavailable";return false;}
 try{
  auto actual=std::shared_ptr<CatalogEnvironmentV67>(new CatalogEnvironmentV67);
  CanonicalLightPointFactoryInputsV53 input;input.world=s.services_owner;
  // No capture of actual: factory callbacks cannot form owner->factory->owner.
  input.services=[s,p](const auto& source,const auto& record,auto& v,auto& error){
   if(!leaf(s,*p,[&](const auto& delivery){return p->retain_release_record(delivery,record,source,error);},error))return false;
   if(p->observe_record_v113&&!leaf(s,*p,[&](const auto& delivery){return p->observe_record_v113(delivery,record,error);},error))return false;
   v=services(s,p,record);return true;};
  actual->scope_=std::move(s);actual->leaves_=std::move(p);
  actual->factory_=std::make_unique<CanonicalLightPointFactoryV53>(std::move(input));
  PartV67 selected;selected.owner=actual;selected.entries={*original};
  selected.construct=[actual](const auto& x,const auto& source,auto& receiver,auto& error){return actual->construct(x,source,receiver,error);};
  out=std::move(actual);part=std::move(selected);e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool CatalogEnvironmentV67::construct(const world::CanonicalFactoryEntryV1& entry,
 const world::CanonicalSourceObjectRequestV1& source,world::CanonicalClassReceiverV1& out,std::string& e){
 if(release_busy_||release_started_){e="LightPoint catalog is quiesced for source release";return false;}
 LightDeliveryV67 b;if(!borrow(scope_,*leaves_,b,e))return false;
 // Existing factory owns exact C1, source lease and receiver transport. Its
 // InitPost has original RGB/attenuation scaling and genuine registry order.
 return factory_->construct(entry,source,out,e);
}
bool CatalogEnvironmentV67::prepare_source_release(std::shared_ptr<world::LightQuiescenceLeaseV67> lease,std::string& e){
 if(release_busy_){release_reentered_=true;e="Recursive environment source release rejected";return false;}
 ReleaseBusyV67 guard{release_busy_};release_reentered_=false;all_prepared_=false;
 LightDeliveryV67 b;if(!borrow(scope_,*leaves_,b,e))return false;
 if(!lease||!lease->owning_thread_barrier||!lease->validate_current_owning_thread()||
    same_owner(lease->owning_thread_barrier,b.actual_world)||same_owner(lease->owning_thread_barrier,b.level)||same_owner(lease->owning_thread_barrier,b.objects)){
  e="Required independent current light release barrier through actual Scene.Clear";return false;
 }
 release_started_=true; // no new constructor can alter the retained journal
 for(const auto& record:factory_->records())if(record->owner&&!record->owner->prepare_source_teardown(lease,e))return false;
 if(release_reentered_){e="Recursive environment source preflight rejected; journal retained";return false;}
 all_prepared_=true;e.clear();return true;
}
bool CatalogEnvironmentV67::execute_source_release(std::uintptr_t id,std::shared_ptr<world::LightQuiescenceLeaseV67> lease,std::string& e){
 if(release_busy_){release_reentered_=true;e="Recursive environment source execution rejected";return false;}
 if(!all_prepared_){e="Prepare every retained LightPoint before any source Flush mutation";return false;}
 ReleaseBusyV67 guard{release_busy_};release_reentered_=false;
 LightDeliveryV67 b;if(!borrow(scope_,*leaves_,b,e))return false;
 for(const auto& record:factory_->records())if(record->owner&&record->owner->identity()==id)return record->owner->execute_source_teardown(std::move(lease),e);
 e="LightPoint source release requires SAME retained canonical receiver";return false;
}
bool CatalogEnvironmentV67::erase_after_source_release(std::uintptr_t id,std::string& e){
 if(release_busy_){release_reentered_=true;e="Recursive environment journal erase rejected";return false;}
 ReleaseBusyV67 guard{release_busy_};release_reentered_=false;
 bool completed=false;
 for(const auto& record:factory_->records())if(record->owner&&record->owner->identity()==id){
  completed=record->owner->source_teardown_state()==world::LightTeardownStateV67::ObjectBaseCompleted;break;
 }
 if(!completed){e="LightPoint source D0 incomplete; retain actual receiver journal";return false;}
 if(!leaf(scope_,*leaves_,[&](const auto& b){return leaves_->release_completed(b,id,e);},e))return false;
 if(release_reentered_){e="Recursive environment erase validation rejected; journal retained";return false;}
 factory_->erased(id);e.clear();return true;
}
bool CatalogEnvironmentV67::source_frame_update_v113(std::uintptr_t id,std::string& e){
 LightDeliveryV67 b;if(release_started_||release_busy_||!borrow(scope_,*leaves_,b,e)){if(e.empty())e="LightPoint source frame cannot run during class release";return false;}
 for(const auto& record:factory_->records())if(record&&record->owner&&record->owner->identity()==id)return record->owner->source_frame_update_v113(e);
 e="Actual selected LightPoint frame receiver absent";return false;
}
}




