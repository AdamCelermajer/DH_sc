#include "catalog_colbox_v71.hpp"
#include <algorithm>
#include <cstring>
#include <exception>
#include <utility>
namespace dh2::loader {
namespace {
template<class A,class B>bool same_owner(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
bool scope_live(const ScopeV67& s,const CatalogColBoxServicesV71& leaves,std::string& e){
 auto world=s.actual_world.lock();auto level=s.level.lock();auto objects=s.objects.lock();
 if(!world||!level||!objects||!s.properties){e="ColBox actual World/Level/manager expired";return false;}
 if(!s.services_owner||same_owner(s.services_owner,world)||same_owner(s.services_owner,level)||same_owner(s.services_owner,objects)){e="ColBox services must be independent of containing owners";return false;}
 return leaves.validate_current(s,e);
}
}
CatalogColBoxV71::CatalogColBoxV71(ScopeV67 s,CatalogColBoxServicesV71 leaves):scope_(std::move(s)),services_(std::move(leaves)){}
bool CatalogColBoxV71::create(ScopeV67 scope,CatalogColBoxServicesV71 leaves,std::shared_ptr<CatalogColBoxV71>& output,PartV67& part,std::string& e){
 if(!leaves.prepare_services||!leaves.validate_current||!leaves.admit_release_record||!leaves.release_completed){e="ColBox production catalog requires real typed engine/release providers";return false;}
 if(!scope_live(scope,leaves,e))return false;
 const world::CanonicalFactoryEntryV1* exact=nullptr;for(const auto& f:world::canonical_factories_v1())if(f.name&&!std::strcmp(f.name,"ColBox")&&f.original_address==0x340fe0)exact=&f;
 if(!exact){e="Required actual ColBox340fe0 original factory entry";return false;}
 try{auto owner=std::shared_ptr<CatalogColBoxV71>(new CatalogColBoxV71(std::move(scope),std::move(leaves)));PartV67 candidate;candidate.owner=owner;candidate.entries.push_back(*exact);
  candidate.construct=[owner](const auto& f,const auto& q,auto& out,std::string& e){return owner->construct(f,q,out,e);};output=std::move(owner);part=std::move(candidate);e.clear();return true;
 }catch(const std::exception& ex){e=std::string("ColBox source owner allocation failed: ")+ex.what();return false;}
}
bool CatalogColBoxV71::construct(const world::CanonicalFactoryEntryV1& f,const world::CanonicalSourceObjectRequestV1& q,world::CanonicalClassReceiverV1& output,std::string& e){
 if(busy_){failed_=true;e="ColBox source constructor cannot reenter";return false;}if(failed_){e="ColBox failed source candidate requires discard";return false;}
 if(!f.name||std::strcmp(f.name,"ColBox")||f.original_address!=0x340fe0||!q.source_lease||!q.attribute){e="Required exact ColBox entry/source XML declaration";return false;}
 // Transient pins cover source construction only; retained factory scope stays weak.
 auto world=scope_.actual_world.lock();auto level=scope_.level.lock();auto objects=scope_.objects.lock();
 if(!scope_live(scope_,services_,e))return false;busy_=true;struct Finish {bool& value;~Finish(){value=false;}} finish{busy_};
 try{
  auto record=std::make_shared<ColBoxRecordV71>();record->declaration=q.source_lease;prefixes_.push_back(record);
  auto slot=std::make_shared<std::weak_ptr<world::CanonicalColBoxV71>>();world::GameObjectInitializationServicesV1 init;world::ColBoxServicesV71 engine;
  if(!services_.prepare_services(scope_,q,slot,init,engine,e)||failed_||!scope_live(scope_,services_,e)){failed_=true;return false;}
  if(!slot->expired()||record->receiver){failed_=true;e="ColBox service provider replaced source C1";return false;}
  if(!same_owner(init.owner,scope_.services_owner)||!same_owner(engine.owner,scope_.services_owner)||!engine.validate_current){failed_=true;e="ColBox typed services replaced independent authority";return false;}
  // Journal first: includes C1-incomplete prefix and SAME runtime/XML receipt.
  if(!services_.admit_release_record(scope_,q,record,e)||failed_||!scope_live(scope_,services_,e)){failed_=true;return false;}
  if(record->receiver||!slot->expired()){failed_=true;e="ColBox release admission replaced actual source C1";return false;}
  record->receiver=std::make_unique<world::CanonicalColBoxV71>(scope_.services_owner,record->runtime,std::move(init),std::move(engine));
  auto receiver=std::shared_ptr<world::CanonicalColBoxV71>(record,record->receiver.get());*slot=receiver;
  auto out=world::CanonicalColBoxV71::factory_receiver(std::move(receiver),q.source_lease);
  if(!scope_live(scope_,services_,e)||failed_){failed_=true;return false;}
  records_.emplace(out.object.identity,record);output=std::move(out);e.clear();return true;
 }catch(const std::exception& ex){failed_=true;e=std::string("ColBox source C1 failed: ")+ex.what();return false;}
}
std::shared_ptr<world::CanonicalColBoxV71> CatalogColBoxV71::find(std::uintptr_t id)const{auto r=records_.find(id);return r!=records_.end()&&r->second->receiver?std::shared_ptr<world::CanonicalColBoxV71>(r->second,r->second->receiver.get()):nullptr;}
bool CatalogColBoxV71::retire_after_source_release(std::uintptr_t id,std::string& e){
 if(busy_){e="ColBox source retirement cannot reenter construction";return false;}auto found=records_.find(id);if(found==records_.end()){e="ColBox native release identity is not retained";return false;}
 if(!scope_live(scope_,services_,e))return false;busy_=true;struct Finish {bool& value;~Finish(){value=false;}} finish{busy_};
 if(!services_.release_completed(scope_,id,e)||!scope_live(scope_,services_,e))return false;
 auto record=found->second;record->receiver.reset();records_.erase(found);prefixes_.erase(std::remove(prefixes_.begin(),prefixes_.end(),record),prefixes_.end());e.clear();return true;
}
bool make_colbox_catalog_part_v71(ScopeV67 scope,CatalogColBoxServicesV71 leaves,PartV67& part,std::string& e,std::shared_ptr<CatalogColBoxV71>* actual_owner){
 std::shared_ptr<CatalogColBoxV71> owner;if(!CatalogColBoxV71::create(std::move(scope),std::move(leaves),owner,part,e))return false;if(actual_owner)*actual_owner=std::move(owner);return true;
}
}

