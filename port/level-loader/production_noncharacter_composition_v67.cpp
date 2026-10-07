#include "production_noncharacter_composition_v67.hpp"
#include <exception>
#include <utility>
namespace dh2::loader {
bool compose_production_noncharacter_catalog_v67(ScopeV67 scope,
 ProductionNonCharacterServicesV67 services,const std::vector<std::string>& required,
 std::shared_ptr<ProductionNonCharacterCatalogV67>& out,ClassFactoryV67& factory,std::string& e,ProductionNonCharacterOwnersV67* actual_owners){
 try{
  std::vector<PartV67> parts;ProductionNonCharacterOwnersV67 owners;
  PartV67 containers;if(!make_container_catalog_part_v67(scope,std::move(services.containers),containers,e,&owners.containers))return false;
  parts.push_back(std::move(containers));
  PartV67 auxiliary;auto auxiliary_services=std::make_shared<const CatalogAuxiliaryServicesV67>(std::move(services.auxiliary));
  if(!CatalogAuxiliaryV67::create(scope,std::move(auxiliary_services),owners.auxiliary,auxiliary,e))return false;
  parts.push_back(std::move(auxiliary));
  if(services.lights){
   PartV67 lights;
   if(!CatalogEnvironmentV67::create(scope,std::move(services.lights),owners.environment,lights,e))return false;
   parts.push_back(std::move(lights));
  }
  if(services.generic){
   PartV67 part;
   if(!CatalogGenericV70::create(scope,std::move(services.generic),owners.generic,part,e))return false;
   parts.push_back(std::move(part));
  }
  if(services.colbox){
   PartV67 part;
   if(!make_colbox_catalog_part_v71(scope,*services.colbox,part,e,&owners.colbox))return false;
   parts.push_back(std::move(part));
  }
  if(services.floor){
   PartV67 part;
   if(!make_floor_catalog_part_v72(scope,*services.floor,part,e,&owners.floor))return false;
   parts.push_back(std::move(part));
  }
  // Extra families join the same immutable constructor route before XML.
  // The existing catalog validates each original entry and owner in one place.
  for(auto& part:services.additional_parts_v101){
   owners.additional_catalog_owners_v101.push_back(part.owner);
   parts.push_back(std::move(part));
  }
  std::shared_ptr<ProductionNonCharacterCatalogV67> actual;
  if(!ProductionNonCharacterCatalogV67::create(std::move(scope),std::move(parts),actual,e)||
     !actual->require_entries(required,e))return false;
  auto callback=actual->factory();out=std::move(actual);factory=std::move(callback);
  if(actual_owners)*actual_owners=std::move(owners);e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
}
