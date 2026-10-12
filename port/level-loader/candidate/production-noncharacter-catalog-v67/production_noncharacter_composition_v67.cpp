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
  std::shared_ptr<ProductionNonCharacterCatalogV67> actual;
  if(!ProductionNonCharacterCatalogV67::create(std::move(scope),std::move(parts),actual,e)||
     !actual->require_entries(required,e))return false;
  auto callback=actual->factory();out=std::move(actual);factory=std::move(callback);
  if(actual_owners)*actual_owners=std::move(owners);e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return false;}
}
}
