#include "production_noncharacter_catalog_v67.hpp"
#include <cstring>
#include <exception>
#include <utility>
namespace dh2::loader {
namespace {
template<class A,class B>bool same_owner(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){
 return a&&b&&!a.owner_before(b)&&!b.owner_before(a);
}
bool reserved(const char* n){
 return !std::strcmp(n,"Character")||!std::strcmp(n,"Player")||!std::strcmp(n,"Module")||!std::strcmp(n,"Block")||!std::strcmp(n,"LevelConfig");
}
const world::CanonicalFactoryEntryV1* original(const char* n){
 if(!n)return nullptr;
 for(const auto& entry:world::canonical_factories_v1())if(!std::strcmp(entry.name,n))return &entry;
 return nullptr;
}
}
bool ProductionNonCharacterCatalogV67::create(ScopeV67 scope,std::vector<PartV67> parts,
 std::shared_ptr<ProductionNonCharacterCatalogV67>& out,std::string& e){
 auto actual=scope.actual_world.lock();auto level=scope.level.lock();auto objects=scope.objects.lock();
 if(!actual||!level||!objects||!scope.properties||!scope.services_owner||parts.empty()){
  e="Production non-Character catalog requires SAME actual World/Level/manager/property map and independent providers";return false;
 }
 if(same_owner(scope.services_owner,actual)||same_owner(scope.services_owner,level)||same_owner(scope.services_owner,objects)){
  e="Catalog services must not retain containing World/Level/manager control block";return false;
 }
 try{
  auto candidate=std::shared_ptr<ProductionNonCharacterCatalogV67>(new ProductionNonCharacterCatalogV67);
  for(std::size_t i=0;i<parts.size();++i){
   const auto& part=parts[i];
   if(!part.owner||!part.construct||part.entries.empty()||same_owner(part.owner,actual)||same_owner(part.owner,level)||same_owner(part.owner,objects)){
    e="Catalog part requires genuine independent canonical factory authority";return false;
   }
   for(const auto& advertised:part.entries){
    const auto* entry=original(advertised.name);
    if(!entry||entry->original_address!=advertised.original_address||reserved(entry->name)){
     e="Catalog part advertises unknown/replaced/reserved original factory";return false;
    }
    if(!candidate->routes_.emplace(entry->name,Route{i,entry}).second){
     e=std::string("Duplicate non-Character constructor authority: ")+entry->name;return false;
    }
   }
  }
  candidate->scope_=std::move(scope);candidate->parts_=std::move(parts);out=std::move(candidate);e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
bool ProductionNonCharacterCatalogV67::construct(const world::CanonicalFactoryEntryV1& entry,
 const world::CanonicalSourceObjectRequestV1& source,world::CanonicalClassReceiverV1& out,std::string& e){
 // Pins exist only during this actual delivery. Child callback captures and
 // receiver service owners must remain independent/weak between calls.
 auto actual=scope_.actual_world.lock();auto level=scope_.level.lock();auto objects=scope_.objects.lock();
 if(!actual||!level||!objects||!source.source_lease){e="Production catalog actual owner/source lease expired";return false;}
 if(!entry.name){e="Production catalog requires actual original class name";return false;}
 const auto found=routes_.find(entry.name);
 if(found==routes_.end()){e=std::string("Unimplemented production non-Character factory: ")+entry.name;return false;}
 const auto& route=found->second;
 if(entry.original_address!=route.original->original_address){e="Production catalog original factory identity mismatch";return false;}
 // Exact routing once. A failed reached constructor is never retried through
 // another part, and its real output/prefix remains visible to the caller.
 return parts_[route.part].construct(*route.original,source,out,e);
}
ClassFactoryV67 ProductionNonCharacterCatalogV67::factory(){
 auto actual=shared_from_this();return [actual](const auto& entry,const auto& source,auto& out,auto& e){return actual->construct(entry,source,out,e);};
}
bool ProductionNonCharacterCatalogV67::supports(const std::string& name)const noexcept{return routes_.find(name)!=routes_.end();}
bool ProductionNonCharacterCatalogV67::require_entries(const std::vector<std::string>& names,std::string& e)const{
 for(const auto& name:names)if(!supports(name)){e="Required authored production constructor unavailable: "+name;return false;}
 e.clear();return true;
}
}
