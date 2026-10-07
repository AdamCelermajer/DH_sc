#include "catalog_projectiles_v99.hpp"
#include "projectile_resource_services_v99.hpp"
#include <algorithm>
#include <cstring>
#include <exception>
namespace dh2::loader {
namespace {
template<class A,class B>bool same(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
bool scope_independent(const ScopeV67& s,const std::shared_ptr<void>& p){
 return p&&!same(p,s.actual_world.lock())&&!same(p,s.level.lock())&&!same(p,s.objects.lock());
}
}
CatalogProjectilesV99::CatalogProjectilesV99(ScopeV67 s,CatalogProjectileServicesV99 services):scope_(std::move(s)),services_(std::move(services)){}
bool CatalogProjectilesV99::fail(std::string& e){failed_=true;if(failure_.empty())failure_=e.empty()?"Projectile catalog retained failed source prefix":e;e=failure_;return false;}
bool CatalogProjectilesV99::delivery(AuxiliaryDeliveryV67& out,std::string& e,bool release)const{
 if(failed_&&!release){e=failure_;return false;}
 out={scope_.actual_world.lock(),scope_.level.lock(),scope_.objects.lock(),scope_.properties};
 if(!out.actual_world||!out.level||!out.objects||!out.properties||!services_.validate_current){e="Required live SAME Projectile catalog World/Level/manager/property authority";return false;}
 if(!services_.validate_current(out,e))return false;
 if(failed_&&!release){e=failure_;return false;}return true;
}
bool CatalogProjectilesV99::create(ScopeV67 scope,CatalogProjectileServicesV99 services,
 std::shared_ptr<CatalogProjectilesV99>& out,PartV67& part,std::string& e){
 if(out||!scope_independent(scope,scope.services_owner)||!scope_independent(scope,services.owner)||
    !same(services.owner,scope.services_owner)||services.actual_graph.expired()||
    !services.validate_current||!services.lend_platform||!services.retain_release_record||!services.release_completed){
  e="Required independent SAME Projectile graph/platform/existing release-journal lenders";return false;
 }
 auto actual=std::shared_ptr<CatalogProjectilesV99>(new CatalogProjectilesV99(std::move(scope),std::move(services)));
 AuxiliaryDeliveryV67 pins;if(!actual->delivery(pins,e))return false;
 PartV67 result;result.owner=actual;
 for(const auto& entry:world::canonical_factories_v1())if(!std::strcmp(entry.name,"Projectile")||!std::strcmp(entry.name,"LaserTypeProjectile"))result.entries.push_back(entry);
 if(result.entries.size()!=2){e="Required both original Projectile/Laser factory entries";return false;}
 result.construct=[actual](const auto& entry,const auto& request,auto& receiver,std::string& e){return actual->construct(entry,request,receiver,e);};
 out=std::move(actual);part=std::move(result);e.clear();return true;
}
bool CatalogProjectilesV99::construct(const world::CanonicalFactoryEntryV1& entry,
 const world::CanonicalSourceObjectRequestV1& source,world::CanonicalClassReceiverV1& out,std::string& e){
 if(busy_){e="Projectile catalog C1 reentered";return fail(e);}if(failed_){e=failure_;return false;}
 const bool laser=entry.name&&!std::strcmp(entry.name,"LaserTypeProjectile");
 if(!entry.name||(!laser&&std::strcmp(entry.name,"Projectile"))||
    entry.original_address!=(laser?0x340cf0u:0x340d14u)||!source.source_lease){
  e="Required authentic Projectile9/Laser10 factory/source lease";return false;
 }
 AuxiliaryDeliveryV67 pins;if(!delivery(pins,e))return false;
 if(same(source.source_lease,pins.actual_world)||same(source.source_lease,pins.level)||same(source.source_lease,pins.objects)){
  e="Projectile declaration lease cannot retain containing native scope";return false;
 }
 busy_=true;struct Busy{bool& value;~Busy(){value=false;}}busy{busy_};
 auto record=std::make_shared<CanonicalProjectileRecordV99>();record->source_lease=source.source_lease;
 auto release=std::make_shared<AuxiliaryReleaseRecordV67>();release->record=record;release->class_name=entry.name;record->release=release;
 records_.push_back({record,release}); //actual constructor prefix BEFORE admission/C1.
 const auto weak=std::weak_ptr<CanonicalProjectileRecordV99>(record);const auto catalog=weak_from_this();
 release->has_constructed_owner=[weak]{auto r=weak.lock();return r&&bool(r->owner);};
 release->constructor_state=[weak]{auto r=weak.lock();return r?r->constructor_state:CanonicalConstructorStateV89::failed;};
 release->identity=[weak]{auto r=weak.lock();return r&&r->owner?r->owner->base().identity():0;};
 release->destroy_source=[weak,catalog](std::string& e){
  auto r=weak.lock();auto c=catalog.lock();AuxiliaryDeliveryV67 pins;
  if(!c||!r||r->constructor_state!=CanonicalConstructorStateV89::completed||!r->owner||r->retired||!c->delivery(pins,e,true)){
   if(e.empty())e="Required SAME completed Projectile C1 for qualified D2";return false;
  }
  r->teardown_started=true;
  if(!r->owner->destroy_source(e))return false;
  r->destroyed=true; //genuine fixture-qualified D2 receipt, never passive expiry.
  return c->delivery(pins,e,true);
 };
 release->retire_storage_after_source_release_v91=[weak,catalog](std::string& e){
  auto r=weak.lock();auto c=catalog.lock();if(!c||!r||!r->owner){e="Required SAME Projectile storage retirement";return false;}
  return c->retire_after_source_release_v99(r->owner->base().identity(),e);
 };
 release->borrow_save_header=[weak](level::LevelSaveObjectBorrowV2& out,std::string& e){
  auto r=weak.lock();if(!r||!r->owner||r->retired||r->constructor_state!=CanonicalConstructorStateV89::completed){e="Required SAME live Projectile OBJS header";return false;}
  auto& b=r->owner->base();out={};out.identity=reinterpret_cast<const void*>(b.identity());out.checkpoint28=b.byte(0x28);
  out.gametype48=b.string(0x48);out.map_name=b.string(0x30);out.room64=&b.room64();out.disabled81=b.byte(0x81);
  out.receiver_lease_v86=std::shared_ptr<void>(r,r->owner.get());e.clear();return true;
 };
 release->make_save_connection=[weak](NonCharacterRestoreServicesV89 leaves,std::shared_ptr<NonCharacterSaveConnectionV89>& out,std::string& e){
  auto r=weak.lock();if(!r||r->retired){e="Required SAME Projectile save record";return false;}
  return make_noncharacter_record_save_v89(r,[](auto& actual,NonCharacterSaveFieldsV89& fields,std::string& e){return gameobject_save_fields_v89(actual.base(),fields,e);},std::move(leaves),out,e);
 };
 try{
  if(!services_.retain_release_record(pins,release,source,e)||!delivery(pins,e))return fail(e);
  record->constructor_state=CanonicalConstructorStateV89::constructing;
  if(laser)record->owner=std::make_unique<world::CanonicalLaserTypeProjectileV99>(scope_.services_owner,record->runtime);
  else record->owner=std::make_unique<world::CanonicalProjectileV99>(scope_.services_owner,record->runtime);
  record->constructor_state=CanonicalConstructorStateV89::completed;
  if(!delivery(pins,e))return fail(e);
  auto graph=services_.actual_graph.lock();if(!graph){e="Required SAME actual GameObject graph";return fail(e);}
  world::CanonicalBaseBorrowV68 base=[weak,catalog](std::shared_ptr<void>& pin,world::CanonicalGameObjectBaseOwnerV1*& out,std::string& e){
   auto r=weak.lock();auto c=catalog.lock();AuxiliaryDeliveryV67 pins;
   if(!r||!c||!r->owner||r->retired||!c->delivery(pins,e,true)){if(e.empty())e="Required live SAME Projectile graph borrower";return false;}
   pin=std::shared_ptr<void>(r,r->owner.get());out=&r->owner->base();return true;
  };
  if(!graph->observe_base_v78(record->owner->base().identity(),base,e)||!delivery(pins,e))return fail(e);
  world::GameObjectInitializationServicesV1 init;world::GameObjectSetPositionServicesV2 position;
  std::shared_ptr<world::ProjectileResourceServicesV99> resources;
  const bool lent=services_.lend_platform(pins,*record->owner,init,position,resources,e);
  //A lender may have produced real cleanup services before its failed suffix.
  //Commit that SAME independent bag before testing delivery/graph upgrade.
  if(resources){
   if(!scope_independent(scope_,std::shared_ptr<void>(resources))||same(resources,record)||!scope_independent(scope_,resources->owner)||!resources->current){
    if(e.empty())e="Projectile resource bag cannot retain containing scope/receiver";return fail(e);
   }
   std::string receipt;
   if(!record->owner->retain_resources_v99(resources,receipt)){if(e.empty())e=receipt;return fail(e);}
  }
  if(!lent||!delivery(pins,e))return fail(e);
  if(!scope_independent(scope_,init.owner)||!scope_independent(scope_,position.owner)||!resources){
   e="Projectile platform must retain independent service owners and real weak native leaves";return fail(e);
  }
  if(!graph->bind(record->owner->base().identity(),base,init,position,e)||!delivery(pins,e))return fail(e);
  if(!record->owner->bind_platform_v99(std::move(init),std::move(position),std::move(resources),e))return fail(e);
  //No InitPost has run: existing SourceFactory/Spawn owns property/order calls.
  const auto alias=std::shared_ptr<world::CanonicalProjectileV99>(record,record->owner.get());
  world::CanonicalClassReceiverV1 made;made.object=alias->canonical(alias);made.source_lease=source.source_lease;
  auto live=[weak,catalog](std::shared_ptr<CanonicalProjectileRecordV99>& r,std::string& e){
   r=weak.lock();auto c=catalog.lock();AuxiliaryDeliveryV67 pins;
   if(!r||!c||!r->owner||r->retired||r->teardown_started||r->constructor_state!=CanonicalConstructorStateV89::completed||!c->delivery(pins,e)){
    if(e.empty())e="Projectile class dispatch requires SAME live completed C1";return false;
   }return true;
  };
  made.properties=[live]{std::shared_ptr<CanonicalProjectileRecordV99> r;std::string e;return live(r,e)?r->owner->properties():world::CanonicalPropertyActorV1{};};
  made.init_post=[live](std::string& e){std::shared_ptr<CanonicalProjectileRecordV99> r;return live(r,e)&&r->owner->init_post(e);};
  made.is_game_object=[live](bool& value,std::string& e){std::shared_ptr<CanonicalProjectileRecordV99> r;if(!live(r,e))return false;value=true;e.clear();return true;};
  made.position=[live](auto& value,std::string& e){std::shared_ptr<CanonicalProjectileRecordV99> r;return live(r,e)&&r->owner->position(value,e);};
  made.set_position=[live](const auto& value,bool destination,std::string& e){std::shared_ptr<CanonicalProjectileRecordV99> r;return live(r,e)&&r->owner->set_position(value,destination,e);};
  made.source_loading_fields_v95=[live](auto& fields,std::string& e){std::shared_ptr<CanonicalProjectileRecordV99> r;if(!live(r,e))return false;return r->owner->source_loading_fields_v95(std::shared_ptr<void>(r,r->owner.get()),fields,e);};
  made.source_is_updatable_v95=[live](bool& value,std::string& e){std::shared_ptr<CanonicalProjectileRecordV99> r;return live(r,e)&&r->owner->source_is_updatable_v95(value,e);};
  made.source_init_final_v95=[live](std::string& e){std::shared_ptr<CanonicalProjectileRecordV99> r;return live(r,e)&&r->owner->init_final(e);};
  out=std::move(made);e.clear();return true;
 }catch(const std::exception& ex){if(record->constructor_state==CanonicalConstructorStateV89::constructing)record->constructor_state=CanonicalConstructorStateV89::failed;e=ex.what();return fail(e);}
 catch(...){if(record->constructor_state==CanonicalConstructorStateV89::constructing)record->constructor_state=CanonicalConstructorStateV89::failed;e="Projectile C1/resource lender threw; original prefix retained";return fail(e);}
}
bool CatalogProjectilesV99::borrow_projectile_v99(std::uintptr_t id,std::shared_ptr<world::CanonicalProjectileV99>& out,std::string& e)const{
 AuxiliaryDeliveryV67 pins;if(!delivery(pins,e,true))return false;
 for(const auto& entry:records_){const auto& r=entry.record;if(r->owner&&r->owner->base().identity()==id&&
    r->constructor_state==CanonicalConstructorStateV89::completed&&!r->retired&&!r->teardown_started){
   out=std::shared_ptr<world::CanonicalProjectileV99>(r,r->owner.get());e.clear();return true;
  }}
 e="No SAME live canonical Projectile/Laser receiver at native identity";return false;
}
bool CatalogProjectilesV99::retire_after_source_release_v99(std::uintptr_t id,std::string& e){
 AuxiliaryDeliveryV67 pins;if(!delivery(pins,e,true))return false;
 auto i=std::find_if(records_.begin(),records_.end(),[id](const Entry& entry){return entry.record->owner&&entry.record->owner->base().identity()==id;});
 if(i==records_.end()||!i->record->destroyed||i->record->retired){e="Required genuine completed SAME Projectile D2 before storage retirement";return false;}
 const auto record=i->record;
 if(record->retirement_busy){e="Projectile storage retirement reentered";return false;}
 record->retirement_busy=true;struct Retirement{bool& active;~Retirement(){active=false;}}retirement{record->retirement_busy};
 if(!services_.release_completed(pins,id,e)||!delivery(pins,e,true))return false;
 auto graph=services_.actual_graph.lock();if(!graph){e="Required SAME Projectile common graph retirement";return false;}
 if(graph->source_contains_v92(id)&&!graph->retire_completed_source_v92(id,e))return false;
 //Incoming native alias proof +transport erase are already complete. A caller
 //holding the same allocation keeps host bytes alive; no native D0 replay.
 record->retired=true;
 i=std::find_if(records_.begin(),records_.end(),[&](const Entry& entry){return entry.record==record;});
 if(i==records_.end()){e="Projectile C1 receipt changed during real retirement";return false;}
 records_.erase(i);e.clear();return true;
}
bool make_projectile_catalog_part_v99(ScopeV67 scope,CatalogProjectileServicesV99 services,PartV67& part,std::string& e,
 std::shared_ptr<CatalogProjectilesV99>* owner){
 std::shared_ptr<CatalogProjectilesV99> actual;if(!CatalogProjectilesV99::create(std::move(scope),std::move(services),actual,part,e))return false;
 if(owner)*owner=std::move(actual);return true;
}
}
