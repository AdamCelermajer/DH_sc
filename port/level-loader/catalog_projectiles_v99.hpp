#pragma once
#include "production_noncharacter_catalog_v67.hpp"
#include "catalog_auxiliary_v67.hpp"
#include <canonical_projectile_v99.hpp>
#include <canonical_gameobject_graph_v68.hpp>
namespace dh2::loader {
//ONE runtime/C1 record, alias-owned by every factory, manager and pool loan.
//C1 admission uses the EXISTING AuxiliaryReleaseRecord/SourceReleaseJournal.
struct CanonicalProjectileRecordV99 {
 actor::RuntimeState runtime;
 std::unique_ptr<world::CanonicalProjectileV99> owner;
 CanonicalConstructorStateV89 constructor_state{CanonicalConstructorStateV89::prepared};
 std::shared_ptr<const void> source_lease;
 std::weak_ptr<AuxiliaryReleaseRecordV67> release; //journal owns it; no record-release-record cycle.
 bool teardown_started{},destroyed{},retired{},retirement_busy{};
};
struct CatalogProjectileServicesV99 {
 std::shared_ptr<void> owner; //independent Main cache/resource primitive owner.
 std::weak_ptr<world::CanonicalGameObjectGraphV68> actual_graph;
 std::function<bool(const AuxiliaryDeliveryV67&,std::string&)> validate_current;
 //Services loan only; no constructor/whole class InitPost/D0 delegation.
 std::function<bool(const AuxiliaryDeliveryV67&,world::CanonicalProjectileV99&,
  world::GameObjectInitializationServicesV1&,world::GameObjectSetPositionServicesV2&,
  std::shared_ptr<world::ProjectileResourceServicesV99>&,std::string&)> lend_platform;
 std::function<bool(const AuxiliaryDeliveryV67&,const std::shared_ptr<AuxiliaryReleaseRecordV67>&,
  const world::CanonicalSourceObjectRequestV1&,std::string&)> retain_release_record;
 std::function<bool(const AuxiliaryDeliveryV67&,std::uintptr_t,std::string&)> release_completed;
};
class CatalogProjectilesV99 final:public std::enable_shared_from_this<CatalogProjectilesV99> {
 ScopeV67 scope_;CatalogProjectileServicesV99 services_;
 struct Entry {std::shared_ptr<CanonicalProjectileRecordV99> record;std::shared_ptr<AuxiliaryReleaseRecordV67> release;};
 std::vector<Entry> records_;
 bool busy_{},failed_{};std::string failure_;
 CatalogProjectilesV99(ScopeV67,CatalogProjectileServicesV99);
 bool delivery(AuxiliaryDeliveryV67&,std::string&,bool release=false)const;
 bool fail(std::string&);
public:
 static bool create(ScopeV67,CatalogProjectileServicesV99,
  std::shared_ptr<CatalogProjectilesV99>&,PartV67&,std::string&);
 bool construct(const world::CanonicalFactoryEntryV1&,const world::CanonicalSourceObjectRequestV1&,
  world::CanonicalClassReceiverV1&,std::string&);
 //Read-only typed alias for Main process-pool projection, never another C1.
 bool borrow_projectile_v99(std::uintptr_t,std::shared_ptr<world::CanonicalProjectileV99>&,std::string&)const;
 bool retire_after_source_release_v99(std::uintptr_t,std::string&);
 std::size_t retained_count()const noexcept{return records_.size();}
};
bool make_projectile_catalog_part_v99(ScopeV67,CatalogProjectileServicesV99,
 PartV67&,std::string&,std::shared_ptr<CatalogProjectilesV99>* actual_owner=nullptr);
}
