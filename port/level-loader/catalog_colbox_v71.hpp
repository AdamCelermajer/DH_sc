#pragma once
#include "canonical_colbox_v71.hpp"
#include "production_noncharacter_catalog_v67.hpp"
namespace dh2::loader {
struct ColBoxRecordV71 {
 actor::RuntimeState runtime;
 std::unique_ptr<world::CanonicalColBoxV71> receiver;
 std::shared_ptr<const void> declaration;
};
using ColBoxSlotV71=std::shared_ptr<std::weak_ptr<world::CanonicalColBoxV71>>;
struct CatalogColBoxServicesV71 {
 // The services builder lends real native leaves for the EMPTY weak receiver
 // slot, which is filled with SAME C1 before any InitPost leaf is called.
 // No whole ColBox constructor callback and no strong containing owner capture.
 std::function<bool(const ScopeV67&,const world::CanonicalSourceObjectRequestV1&,const ColBoxSlotV71&,
  world::GameObjectInitializationServicesV1&,world::ColBoxServicesV71&,std::string&)> prepare_services;
 std::function<bool(const ScopeV67&,std::string&)> validate_current;
 std::function<bool(const ScopeV67&,const world::CanonicalSourceObjectRequestV1&,
  const std::shared_ptr<ColBoxRecordV71>&,std::string&)> admit_release_record;
 std::function<bool(const ScopeV67&,std::uintptr_t,std::string&)> release_completed;
};
class CatalogColBoxV71 final {
 ScopeV67 scope_;CatalogColBoxServicesV71 services_;
 std::vector<std::shared_ptr<ColBoxRecordV71>> prefixes_;
 std::map<std::uintptr_t,std::shared_ptr<ColBoxRecordV71>> records_;
 bool busy_{},failed_{};
 CatalogColBoxV71(ScopeV67,CatalogColBoxServicesV71);
public:
 static bool create(ScopeV67,CatalogColBoxServicesV71,std::shared_ptr<CatalogColBoxV71>&,PartV67&,std::string&);
 bool construct(const world::CanonicalFactoryEntryV1&,const world::CanonicalSourceObjectRequestV1&,
  world::CanonicalClassReceiverV1&,std::string&);
 // Keep actual C1/runtime/source receipt until genuine D0, unpublication and
 // alias exhaustion. Native destruction itself is receiver->destroy().
 bool retire_after_source_release(std::uintptr_t,std::string&);
 std::shared_ptr<world::CanonicalColBoxV71> find(std::uintptr_t)const;
};
bool make_colbox_catalog_part_v71(ScopeV67,CatalogColBoxServicesV71,PartV67&,std::string&,
 std::shared_ptr<CatalogColBoxV71>* actual_owner=nullptr);
}
