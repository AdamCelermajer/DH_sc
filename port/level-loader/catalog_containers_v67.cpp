#include "catalog_containers_v67.hpp"
#include <cstring>
#include <algorithm>
#include <exception>
#include <utility>
namespace dh2::loader {
namespace {
struct PinsV67 {
 std::shared_ptr<void> actual_world;
 std::shared_ptr<CanonicalLevelContextV1> level;
 std::shared_ptr<world::CanonicalObjectManagerV1> objects;
};
template<class A,class B>bool same_control(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){
 return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
bool live(const ScopeV67& scope,PinsV67& out,std::string& error){
 out.actual_world=scope.actual_world.lock();out.level=scope.level.lock();out.objects=scope.objects.lock();
 if(!out.actual_world||!out.level||!out.objects||!scope.properties||!scope.services_owner){
  error="Required live SAME World/Level/ObjectManager/PropertyMap and independent container services";return false;
 }
 if(same_control(scope.services_owner,out.actual_world)||same_control(scope.services_owner,out.level)||
    same_control(scope.services_owner,out.objects)){
  error="Container services_owner cannot retain actual World/Level/ObjectManager";return false;
 }
 return true;
}
template<class T>bool independent(const std::shared_ptr<T>& owner,const PinsV67& pins){
 return !owner||(!same_control(owner,pins.actual_world)&&!same_control(owner,pins.level)&&!same_control(owner,pins.objects));
}
bool owner_matches(const std::shared_ptr<void>& owner,const ScopeV67& scope){
 return owner&&owner.get()==scope.services_owner.get()&&same_control(owner,scope.services_owner);
}
void guarded_dispatch(const ScopeV67& scope,world::CanonicalClassReceiverV1& receiver,std::shared_ptr<bool> active={}){
 if(receiver.properties){auto body=std::move(receiver.properties);receiver.properties=[scope,active,body=std::move(body)](){
  PinsV67 pins;std::string error;if((active&&!*active)||!live(scope,pins,error))return world::CanonicalPropertyActorV1{};return body();
 };}
 if(receiver.init_post){auto body=std::move(receiver.init_post);receiver.init_post=[scope,active,body=std::move(body)](std::string& e){
  PinsV67 pins;if(active&&!*active){e="Actual container teardown has started; old transport dispatch unavailable";return false;}return live(scope,pins,e)&&body(e);
 };}
 if(receiver.is_game_object){auto body=std::move(receiver.is_game_object);receiver.is_game_object=[scope,active,body=std::move(body)](bool& value,std::string& e){
  PinsV67 pins;if(active&&!*active){e="Actual container teardown has started; old transport dispatch unavailable";return false;}return live(scope,pins,e)&&body(value,e);
 };}
 if(receiver.position){auto body=std::move(receiver.position);receiver.position=[scope,active,body=std::move(body)](std::array<float,3>& value,std::string& e){
  PinsV67 pins;if(active&&!*active){e="Actual container teardown has started; old transport dispatch unavailable";return false;}return live(scope,pins,e)&&body(value,e);
 };}
 if(receiver.set_position){auto body=std::move(receiver.set_position);receiver.set_position=[scope,active,body=std::move(body)](const std::array<float,3>& value,bool destination,std::string& e){
  PinsV67 pins;if(active&&!*active){e="Actual container teardown has started; old transport dispatch unavailable";return false;}return live(scope,pins,e)&&body(value,destination,e);
 };}
}
struct DestructibleRecordV67 {
 actor::RuntimeState runtime{};
 world::CanonicalDestructibleContainerV16 receiver;
 DestructibleRecordV67(std::shared_ptr<void> owner,world::GameObjectInitializationServicesV1 init,
  world::DestructibleContainerServicesV16 services)
  :receiver(std::move(owner),runtime,std::move(init),std::move(services)){}
};
class ContainerPartV67 {
 ScopeV67 scope_;
 CatalogContainerServicesV67 services_;
 struct Journal {
  std::shared_ptr<world::CanonicalOpenableGraphV21> openable;
  std::shared_ptr<world::CanonicalDestructibleContainerV16> destructible;
  std::shared_ptr<const void> source;
  world::CanonicalObjectBorrowV1 actual;
  std::shared_ptr<bool> active=std::make_shared<bool>(true);
  OpenableSlotV67 openable_slot;DestructibleSlotV67 destructible_slot;
  bool admission_attempted{},admitted{},teardown_attempted{},torn_down{},busy{};
  std::string failure;
 };
 std::map<std::uintptr_t,Journal> journals_;
public:
 ContainerPartV67(ScopeV67 scope,CatalogContainerServicesV67 services)
  :scope_(std::move(scope)),services_(std::move(services)){}
 bool construct(const world::CanonicalFactoryEntryV1& entry,
  const world::CanonicalSourceObjectRequestV1& request,world::CanonicalClassReceiverV1& out,std::string& e){
  PinsV67 pins;if(!live(scope_,pins,e))return false;
  if(!entry.name||!request.source_lease||!independent(request.source_lease,pins)){e="Container catalog requires actual retained source declaration";return false;}
  try{
   if(!std::strcmp(entry.name,"OpenableContainer")){
    if(entry.original_address!=0x340da4){e="Foreign OpenableContainer original C1 entry";return false;}
    auto slot=std::make_shared<std::weak_ptr<world::CanonicalOpenableContainerV1>>();
    world::CanonicalOpenableGraphServicesV21 services;
    if(!services_.openable_services(scope_,request,slot,services,e))return false;
    if(!owner_matches(services.world,scope_)||!independent(services.roots,pins)||
       !independent(services.visual.owner,pins)||!independent(services.initialization.owner,pins)||
       !independent(services.position.owner,pins)||!independent(services.decor.owner,pins)||
       !independent(services.container.owner,pins)){
     e="Actual V21 container services must retain independent owners and weak World/Level";return false;
    }
    auto table=services_.openable_table;
    services.container.resolve_row=[table](const std::string& name,std::int32_t& id,
      world::OpenableContainerRowV1& row,std::string& error){return table->resolve(name,id,row,error);};
    if(!services.container.precache_complete_source_v42){e="Required factory-prepared actual openable precache callback";return false;}
    auto graph=std::make_shared<world::CanonicalOpenableGraphV21>(std::move(services));
    auto actual=std::shared_ptr<world::CanonicalOpenableContainerV1>(graph,&graph->receiver());
    *slot=actual; // Actual C1 is complete; no InitPost has run.
    auto candidate=graph->factory_receiver();candidate.source_lease=request.source_lease;
    if(candidate.object.identity!=actual->base().identity()||
       candidate.object.shared_handle!=&actual->base().shared_handle()||
       candidate.object.lease.get()!=actual.get()||slot->lock().get()!=actual.get()){
     e="V21 factory/weak slot must borrow SAME actual Openable C1 fields";out=std::move(candidate);return false;
    }
    Journal journal;journal.openable=graph;journal.source=request.source_lease;
    journal.actual=candidate.object;journal.openable_slot=slot;
    auto inserted=journals_.emplace(candidate.object.identity,std::move(journal));
    if(!inserted.second){e="Actual Openable C1 identity already journaled";out=std::move(candidate);return false;}
    auto& receipt=inserted.first->second;
    guarded_dispatch(scope_,candidate,receipt.active);out=std::move(candidate);
    receipt.busy=true;struct AdmissionGuard{bool& b;~AdmissionGuard(){b=false;}} admission_guard{receipt.busy};
    receipt.admission_attempted=true;
    if(!services_.admit_openable(scope_,request,graph,e)){receipt.failure=e;return false;}
    receipt.admitted=true;
    if(!graph->ready(e))return false; // Preserve actual constructed prefix on failure.
    e.clear();return true;
   }
   if(!std::strcmp(entry.name,"DestructibleContainer")){
    if(entry.original_address!=0x340d5c){e="Foreign DestructibleContainer original C1 entry";return false;}
    auto slot=std::make_shared<std::weak_ptr<world::CanonicalDestructibleContainerV16>>();
    world::GameObjectInitializationServicesV1 init;world::DestructibleContainerServicesV16 services;
    if(!services_.destructible_services(scope_,request,slot,init,services,e))return false;
    if(!owner_matches(services.owner,scope_)||!independent(init.owner,pins)||
       !independent(services.common.owner,pins)){
     e="Actual V16 container services must retain independent owners and weak World/Level";return false;
    }
    services.table=services_.destructible_table;
    if(!services.common.precache_complete_source_v42){e="Required factory-prepared actual destructible precache callback";return false;}
    auto record=std::make_shared<DestructibleRecordV67>(scope_.services_owner,std::move(init),std::move(services));
    auto actual=std::shared_ptr<world::CanonicalDestructibleContainerV16>(record,&record->receiver);
    *slot=actual; // This SAME receiver and Runtime are one alias-owned C1 record.
    auto candidate=world::CanonicalDestructibleContainerV16::factory_receiver(actual,request.source_lease);
    if(candidate.object.identity!=actual->base().identity()||
       candidate.object.shared_handle!=&actual->base().shared_handle()||
       candidate.object.lease.get()!=actual.get()||slot->lock().get()!=actual.get()){
     e="V16 factory/weak slot must borrow SAME actual Destructible C1 fields";out=std::move(candidate);return false;
    }
    Journal journal;journal.destructible=actual;journal.source=request.source_lease;
    journal.actual=candidate.object;journal.destructible_slot=slot;
    auto inserted=journals_.emplace(candidate.object.identity,std::move(journal));
    if(!inserted.second){e="Actual Destructible C1 identity already journaled";out=std::move(candidate);return false;}
    auto& receipt=inserted.first->second;
    guarded_dispatch(scope_,candidate,receipt.active);out=std::move(candidate);
    receipt.busy=true;struct AdmissionGuard{bool& b;~AdmissionGuard(){b=false;}} admission_guard{receipt.busy};
    receipt.admission_attempted=true;
    if(!services_.admit_destructible(scope_,request,actual,e)){receipt.failure=e;return false;}
    receipt.admitted=true;e.clear();return true;
   }
   if(!std::strcmp(entry.name,"Item")){
    if(entry.original_address!=0x340d38){e="Foreign Item original C1 entry";return false;}
    auto items=services_.actual_items.lock();if(!items){e="Required SAME existing actual Item owner";return false;}
    world::CanonicalItemCandidateTransportV57 actual(*items,*pins.objects,*scope_.properties,items);
    world::CanonicalClassReceiverV1 candidate;
    if(!actual.construct(entry,request,candidate,e)){out=std::move(candidate);return false;}
    if(candidate.object.identity)guarded_dispatch(scope_,candidate);
    out=std::move(candidate);e.clear();return true;
   }
   e="Entry not supported by actual containers/Item catalog part";return false;
  }catch(const std::exception& ex){e=ex.what();return false;}catch(...){e="Actual container C1/admission threw; reached journal prefix retained";return false;}
 }
 std::shared_ptr<world::CanonicalOpenableGraphV21> openable(std::uintptr_t id)const{
  auto i=journals_.find(id);return i==journals_.end()?nullptr:i->second.openable;
 }
 std::shared_ptr<world::CanonicalDestructibleContainerV16> destructible(std::uintptr_t id)const{
  auto i=journals_.find(id);return i==journals_.end()?nullptr:i->second.destructible;
 }
 std::size_t retained_count()const noexcept{return journals_.size();}
 bool teardown(std::uintptr_t id,std::string& e){
  auto i=journals_.find(id);if(i==journals_.end()){e="No actual container C1 journal for teardown";return false;}
  auto& journal=i->second;
  if(journal.busy){e="Actual container lifecycle reentered";return false;}
  if(journal.torn_down){e.clear();return true;}
  if(journal.teardown_attempted){e=journal.failure.empty()?"Failed actual container teardown prefix cannot replay":journal.failure;return false;}
  PinsV67 pins;if(!live(scope_,pins,e))return false;
  journal.busy=true;struct Guard{bool& b;~Guard(){b=false;}} guard{journal.busy};
  journal.teardown_attempted=true;
  bool completed=false;
  try{completed=journal.openable?services_.teardown_openable(journal.openable,e):journal.destructible->destroy(e);}
  catch(const std::exception& ex){e=ex.what();}
  catch(...){e="Actual container teardown threw; reached prefix retained";}
  // Real finalizers/services can use their SAME receiver during the body.
  // Only after return do old external transports become unavailable.
  *journal.active=false;
  if(!completed){journal.failure=e.empty()?"Actual container teardown failed; prefix retained":e;e=journal.failure;return false;}
  journal.torn_down=true;
  if(journal.openable_slot)journal.openable_slot->reset();
  if(journal.destructible_slot)journal.destructible_slot->reset();
  e.clear();return true;
 }
 bool retire_after_unpublication(std::uintptr_t id,std::string& e){
  auto i=journals_.find(id);if(i==journals_.end()){e="No actual container C1 journal for retirement";return false;}
  auto& journal=i->second;
  if(journal.busy||!journal.torn_down){e="Require completed actual class teardown before journal retirement";return false;}
  PinsV67 pins;if(!live(scope_,pins,e))return false;
  auto still_published=[&]{
   std::int32_t key{};const world::CanonicalObjectBorrowV1* actual{};
   bool more=pins.objects->source_ordered_begin_v38(key,actual);
   while(more){
    if(actual&&actual->identity==id)return true;
    more=pins.objects->source_ordered_next_v38(key,key,actual);
   }
   return false;
  };
  if(still_published()){e="SAME ObjectManager still publishes actual container";return false;}
  journal.busy=true;
  bool unpublished=false;
  try{
   if(!services_.transport_unpublished(journal.actual,unpublished,e)){journal.busy=false;return false;}
  }catch(const std::exception& ex){journal.busy=false;e=ex.what();return false;}
  catch(...){journal.busy=false;e="Actual Main transport publication query threw";return false;}
  journal.busy=false;
  if(!unpublished){e="Actual Main transport still publishes container";return false;}
  if(still_published()){e="Main transport query republished actual container; journal retained";return false;}
  journals_.erase(i);e.clear();return true;
 }
};
}
struct ContainerCatalogOwnerV67::Impl {
 ContainerPartV67 actual;
 Impl(ScopeV67 scope,CatalogContainerServicesV67 services):actual(std::move(scope),std::move(services)){}
};
ContainerCatalogOwnerV67::ContainerCatalogOwnerV67(ScopeV67 scope,CatalogContainerServicesV67 services)
 :impl_(std::make_unique<Impl>(std::move(scope),std::move(services))){}
ContainerCatalogOwnerV67::~ContainerCatalogOwnerV67()=default;
bool ContainerCatalogOwnerV67::construct(const world::CanonicalFactoryEntryV1& entry,
 const world::CanonicalSourceObjectRequestV1& source,world::CanonicalClassReceiverV1& out,std::string& e){
 return impl_->actual.construct(entry,source,out,e);
}
std::shared_ptr<world::CanonicalOpenableGraphV21> ContainerCatalogOwnerV67::openable(std::uintptr_t id)const{return impl_->actual.openable(id);}
std::shared_ptr<world::CanonicalDestructibleContainerV16> ContainerCatalogOwnerV67::destructible(std::uintptr_t id)const{return impl_->actual.destructible(id);}
bool ContainerCatalogOwnerV67::teardown(std::uintptr_t id,std::string& e){return impl_->actual.teardown(id,e);}
bool ContainerCatalogOwnerV67::retire_after_unpublication(std::uintptr_t id,std::string& e){return impl_->actual.retire_after_unpublication(id,e);}
std::size_t ContainerCatalogOwnerV67::retained_count()const noexcept{return impl_->actual.retained_count();}
bool make_container_catalog_part_v67(ScopeV67 scope,CatalogContainerServicesV67 services,PartV67& out,std::string& e,std::shared_ptr<ContainerCatalogOwnerV67>* actual_owner){
 PinsV67 pins;if(!live(scope,pins,e))return false;
 if(!services.openable_table||!services.destructible_table||!services.openable_services||!services.destructible_services||!services.admit_openable||
    !services.admit_destructible||!services.teardown_openable||!services.transport_unpublished){
  e="Required actual container tables/services, typed release-journal admission, complete teardown and transport publication query";return false;
 }
 if(!independent(services.openable_table,pins)||!independent(services.destructible_table,pins)){
  e="Container table leases cannot retain actual World/Level/ObjectManager";return false;
 }
 try{
  auto actual_items=services.actual_items.lock();
  const bool has_items=bool(actual_items);
  if(actual_items&&(&actual_items->factory().source_manager_v57()!=pins.objects.get()||
     &actual_items->factory().source_property_map_v57()!=scope.properties)){
   e="Item catalog authority belongs to another actual manager/PropertyMap";return false;
  }
  auto factory=std::shared_ptr<ContainerCatalogOwnerV67>(new ContainerCatalogOwnerV67(std::move(scope),std::move(services)));
  PartV67 candidate;candidate.owner=factory;
  candidate.entries={{"OpenableContainer",0x340da4},{"DestructibleContainer",0x340d5c}};
  if(has_items)candidate.entries.push_back({"Item",0x340d38});
  candidate.construct=[factory](const auto& entry,const auto& request,auto& receiver,auto& error){
   return factory->construct(entry,request,receiver,error);
  };
  out=std::move(candidate);if(actual_owner)*actual_owner=factory;e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
}
