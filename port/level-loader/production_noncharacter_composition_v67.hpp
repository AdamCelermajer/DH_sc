#pragma once
#include "catalog_containers_v67.hpp"
#include "catalog_auxiliary_v67.hpp"
#include "catalog_environment_v67.hpp"
#include "catalog_generic_v70.hpp"
#include "catalog_colbox_v71.hpp"
#include "catalog_floor_v72.hpp"
namespace dh2::loader {
struct ProductionNonCharacterOwnersV67 {
 std::shared_ptr<ContainerCatalogOwnerV67> containers;
 std::shared_ptr<CatalogAuxiliaryV67> auxiliary;
 std::shared_ptr<CatalogEnvironmentV67> environment;
 std::shared_ptr<CatalogGenericV70> generic;
 std::shared_ptr<CatalogColBoxV71> colbox;
 std::shared_ptr<CatalogFloorV72> floor;
 // Actual extra canonical child owners, retired with this existing owner bag.
 std::vector<std::shared_ptr<void>> additional_catalog_owners_v101;
};
struct ProductionNonCharacterServicesV67 {
 CatalogContainerServicesV67 containers;
 CatalogAuxiliaryServicesV67 auxiliary;
 // Optional only if this actual renderer has no admitted LightPoint provider.
 // A reached LightPoint declaration still fails explicitly when unimplemented.
 std::shared_ptr<const LightEnvironmentLeavesV67> lights;
 std::shared_ptr<const CatalogGenericServicesV70> generic;
 std::shared_ptr<const CatalogColBoxServicesV71> colbox;
 std::shared_ptr<const CatalogFloorServicesV72> floor;
 // Original factory parts, composed once into the same remaining catalog.
 // Existing create() validates independent ownership, original names/addresses,
 // reserved types and duplicate constructor authorities. No second registry.
 std::vector<PartV67> additional_parts_v101;
};
// Actual constructor composition for SourceFactory.remaining. Main's Character
// bridge chains this callback for other types; bind the final combined callback
// ONCE before XML construction. Module/Block/LevelConfig stay Root owned and
// Character/Player stay with Main; no second World/manager/cache is created.
// required_types comes from actual source metadata, never a SWAMP hardcode.
bool compose_production_noncharacter_catalog_v67(ScopeV67,
 ProductionNonCharacterServicesV67,const std::vector<std::string>& required_types,
 std::shared_ptr<ProductionNonCharacterCatalogV67>& catalog,ClassFactoryV67& factory,
 std::string& error,ProductionNonCharacterOwnersV67* actual_owners=nullptr);
// Main passes its actual SourceCampaignCandidateBorrowV55. This template only
// lends those same produced owners; it creates no renderer or stand-in receipt.
// The resulting callback is chained at Main's Character provider before the
// ONE bind_source_campaign_class_factory_v60 call. It does not bind a competing
// remaining factory or retain the containing candidate as a strong capture.
template<class ActualCandidate>
bool make_source_campaign_noncharacter_factory_v67(const ActualCandidate& actual,
 std::shared_ptr<void> independent_services_owner,ProductionNonCharacterServicesV67 services,
 const std::vector<std::string>& required_noncharacter_types,
 std::shared_ptr<ProductionNonCharacterCatalogV67>& catalog,ClassFactoryV67& factory,
 std::string& error,ProductionNonCharacterOwnersV67* actual_owners=nullptr){
 ScopeV67 scope;scope.actual_world=actual.actual_world;scope.level=actual.level;
 scope.objects=actual.objects;scope.properties=actual.properties;
 scope.services_owner=std::move(independent_services_owner);
 return compose_production_noncharacter_catalog_v67(std::move(scope),std::move(services),
  required_noncharacter_types,catalog,factory,error,actual_owners);
}
}
