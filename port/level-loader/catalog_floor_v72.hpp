#pragma once
#include "canonical_floor_v72.hpp"
#include "production_noncharacter_catalog_v67.hpp"
namespace dh2::loader {
struct FloorRecordV72 {
 actor::RuntimeState runtime;
 std::unique_ptr<world::CanonicalFloorV72> receiver;
 std::shared_ptr<const void> declaration;
};
using FloorSlotV72=std::shared_ptr<std::weak_ptr<world::CanonicalFloorV72>>;
struct CatalogFloorServicesV72 {
 // The services builder lends real native leaves for the EMPTY weak receiver
 // slot, which is filled with SAME C1 before any InitPost leaf is called.
 // No whole Floor constructor callback and no strong containing owner capture.
 std::function<bool(const ScopeV67&,const world::CanonicalSourceObjectRequestV1&,const FloorSlotV72&,
  world::GameObjectInitializationServicesV1&,world::FloorServicesV72&,std::string&)> prepare_services;
 std::function<bool(const ScopeV67&,std::string&)> validate_current;
 std::function<bool(const ScopeV67&,const world::CanonicalSourceObjectRequestV1&,
  const std::shared_ptr<FloorRecordV72>&,std::string&)> admit_release_record;
 std::function<bool(const ScopeV67&,std::uintptr_t,std::string&)> release_completed;
};
class CatalogFloorV72 final {
 ScopeV67 scope_;CatalogFloorServicesV72 services_;
 std::vector<std::shared_ptr<FloorRecordV72>> prefixes_;
 std::map<std::uintptr_t,std::shared_ptr<FloorRecordV72>> records_;
 bool busy_{},failed_{};
 CatalogFloorV72(ScopeV67,CatalogFloorServicesV72);
public:
 static bool create(ScopeV67,CatalogFloorServicesV72,std::shared_ptr<CatalogFloorV72>&,PartV67&,std::string&);
 bool construct(const world::CanonicalFactoryEntryV1&,const world::CanonicalSourceObjectRequestV1&,
  world::CanonicalClassReceiverV1&,std::string&);
 // Keep actual C1/runtime/source receipt until genuine D0, unpublication and
 // alias exhaustion. Native destruction itself is receiver->destroy().
 bool retire_after_source_release(std::uintptr_t,std::string&);
 std::shared_ptr<world::CanonicalFloorV72> find(std::uintptr_t)const;
};
bool make_floor_catalog_part_v72(ScopeV67,CatalogFloorServicesV72,PartV67&,std::string&,
 std::shared_ptr<CatalogFloorV72>* actual_owner=nullptr);
}

