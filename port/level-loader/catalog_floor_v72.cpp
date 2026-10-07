#include "catalog_floor_v72.hpp"
#include <algorithm>
#include <cstring>
#include <exception>
#include <utility>
namespace dh2::loader {
namespace {
template<class A,class B>bool same_owner(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
bool scope_live(const ScopeV67& s,const CatalogFloorServicesV72& leaves,std::string& e){
 auto world=s.actual_world.lock();auto level=s.level.lock();auto objects=s.objects.lock();
 if(!world||!level||!objects||!s.properties){e="Floor actual World/Level/manager expired";return false;}
 if(!s.services_owner||same_owner(s.services_owner,world)||same_owner(s.services_owner,level)||same_owner(s.services_owner,objects)){e="Floor services must be independent of containing owners";return false;}
 return leaves.validate_current(s,e);
}
}
CatalogFloorV72::CatalogFloorV72(ScopeV67 s,CatalogFloorServicesV72 leaves):scope_(std::move(s)),services_(std::move(leaves)){}
bool CatalogFloorV72::create(ScopeV67 scope,CatalogFloorServicesV72 leaves,std::shared_ptr<CatalogFloorV72>& output,PartV67& part,std::string& e){
 if(!leaves.prepare_services||!leaves.validate_current||!leaves.admit_release_record||!leaves.release_completed){e="Floor production catalog requires real typed engine/release providers";return false;}
 if(!scope_live(scope,leaves,e))return false;
 const world::CanonicalFactoryEntryV1* exact=nullptr;for(const auto& f:world::canonical_factories_v1())if(f.name&&!std::strcmp(f.name,"Floor")&&f.original_address==0x34104c)exact=&f;
 if(!exact){e="Required actual Floor34104c original factory entry";return false;}
 try{auto owner=std::shared_ptr<CatalogFloorV72>(new CatalogFloorV72(std::move(scope),std::move(leaves)));PartV67 candidate;candidate.owner=owner;candidate.entries.push_back(*exact);
  candidate.construct=[owner](const auto& f,const auto& q,auto& out,std::string& e){return owner->construct(f,q,out,e);};output=std::move(owner);part=std::move(candidate);e.clear();return true;
 }catch(const std::exception& ex){e=std::string("Floor source owner allocation failed: ")+ex.what();return false;}
}
bool CatalogFloorV72::construct(const world::CanonicalFactoryEntryV1& f,const world::CanonicalSourceObjectRequestV1& q,world::CanonicalClassReceiverV1& output,std::string& e){
 if(busy_){failed_=true;e="Floor source constructor cannot reenter";return false;}if(failed_){e="Floor failed source candidate requires discard";return false;}
 if(!f.name||std::strcmp(f.name,"Floor")||f.original_address!=0x34104c||!q.source_lease||!q.attribute){e="Required exact Floor entry/source XML declaration";return false;}
 // Transient pins cover source construction only; retained factory scope stays weak.
 auto world=scope_.actual_world.lock();auto level=scope_.level.lock();auto objects=scope_.objects.lock();
 if(!scope_live(scope_,services_,e))return false;busy_=true;struct Finish {bool& value;~Finish(){value=false;}} finish{busy_};
 try{
  auto record=std::make_shared<FloorRecordV72>();record->declaration=q.source_lease;prefixes_.push_back(record);
  auto slot=std::make_shared<std::weak_ptr<world::CanonicalFloorV72>>();world::GameObjectInitializationServicesV1 init;world::FloorServicesV72 engine;
  if(!services_.prepare_services(scope_,q,slot,init,engine,e)||failed_||!scope_live(scope_,services_,e)){failed_=true;return false;}
  if(!slot->expired()||record->receiver){failed_=true;e="Floor service provider replaced source C1";return false;}
  if(!same_owner(init.owner,scope_.services_owner)||!same_owner(engine.owner,scope_.services_owner)||!engine.validate_current){failed_=true;e="Floor typed services replaced independent authority";return false;}
  // Journal first: includes C1-incomplete prefix and SAME runtime/XML receipt.
  if(!services_.admit_release_record(scope_,q,record,e)||failed_||!scope_live(scope_,services_,e)){failed_=true;return false;}
  if(record->receiver||!slot->expired()){failed_=true;e="Floor release admission replaced actual source C1";return false;}
  record->receiver=std::make_unique<world::CanonicalFloorV72>(scope_.services_owner,record->runtime,std::move(init),std::move(engine));
  auto receiver=std::shared_ptr<world::CanonicalFloorV72>(record,record->receiver.get());*slot=receiver;
  auto out=world::CanonicalFloorV72::factory_receiver(std::move(receiver),q.source_lease);
  if(!scope_live(scope_,services_,e)||failed_){failed_=true;return false;}
  records_.emplace(out.object.identity,record);output=std::move(out);e.clear();return true;
 }catch(const std::exception& ex){failed_=true;e=std::string("Floor source C1 failed: ")+ex.what();return false;}
}
std::shared_ptr<world::CanonicalFloorV72> CatalogFloorV72::find(std::uintptr_t id)const{auto r=records_.find(id);return r!=records_.end()&&r->second->receiver?std::shared_ptr<world::CanonicalFloorV72>(r->second,r->second->receiver.get()):nullptr;}
bool CatalogFloorV72::retire_after_source_release(std::uintptr_t id,std::string& e){
 if(busy_){e="Floor source retirement cannot reenter construction";return false;}auto found=records_.find(id);if(found==records_.end()){e="Floor native release identity is not retained";return false;}
 if(!scope_live(scope_,services_,e))return false;busy_=true;struct Finish {bool& value;~Finish(){value=false;}} finish{busy_};
 if(!services_.release_completed(scope_,id,e)||!scope_live(scope_,services_,e))return false;
 auto record=found->second;record->receiver.reset();records_.erase(found);prefixes_.erase(std::remove(prefixes_.begin(),prefixes_.end(),record),prefixes_.end());e.clear();return true;
}
bool make_floor_catalog_part_v72(ScopeV67 scope,CatalogFloorServicesV72 leaves,PartV67& part,std::string& e,std::shared_ptr<CatalogFloorV72>* actual_owner){
 std::shared_ptr<CatalogFloorV72> owner;if(!CatalogFloorV72::create(std::move(scope),std::move(leaves),owner,part,e))return false;if(actual_owner)*actual_owner=std::move(owner);return true;
}
}


