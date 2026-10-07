#pragma once
#include "production_noncharacter_catalog_v67.hpp"
#include <canonical_openable_graph_v21.hpp>
#include <canonical_destructible_container_v16.hpp>
#include <canonical_item_candidate_transport_v57.hpp>
namespace dh2::loader {
using OpenableSlotV67=std::shared_ptr<std::weak_ptr<world::CanonicalOpenableContainerV1>>;
using DestructibleSlotV67=std::shared_ptr<std::weak_ptr<world::CanonicalDestructibleContainerV16>>;
// Native factory services supply the actual source precache callbacks using
// these SAME retained receiver slots/tables; catalog preserves and requires them.
// Main unpublishes and runs genuine V21 release / V16 destroy / Item erased
// before retiring physics/service owners; C++ lease expiry is not that protocol.
// Main supplies concrete cache/scene/device/physics/condition/script/quest/loot
// leaves. These callbacks prepare typed SERVICES, never construct a substitute
// receiver. Capture World/Level weakly and retain independent service owners.
// Slots are empty during preparation and filled after actual C1, before InitPost.
class ContainerCatalogOwnerV67;
struct CatalogContainerServicesV67 {
 std::shared_ptr<const world::OpenableContainerTableV1> openable_table;
 std::shared_ptr<const world::DestructibleContainerTableV16> destructible_table;
 std::function<bool(const ScopeV67&,const world::CanonicalSourceObjectRequestV1&,
  const OpenableSlotV67&,world::CanonicalOpenableGraphServicesV21&,std::string&)> openable_services;
 std::function<bool(const ScopeV67&,const world::CanonicalSourceObjectRequestV1&,
  const DestructibleSlotV67&,world::GameObjectInitializationServicesV1&,
  world::DestructibleContainerServicesV16&,std::string&)> destructible_services;
 // Main admits these exact C1s to its real release journal before InitPost.
 // Admission failure retains the C1 in this owner's typed prefix journal.
 std::function<bool(const ScopeV67&,const world::CanonicalSourceObjectRequestV1&,
  const std::shared_ptr<world::CanonicalOpenableGraphV21>&,std::string&)> admit_openable;
 std::function<bool(const ScopeV67&,const world::CanonicalSourceObjectRequestV1&,
  const std::shared_ptr<world::CanonicalDestructibleContainerV16>&,std::string&)> admit_destructible;
 // Whole actual class teardown, including V21 graph release and actual
 // base/script/network destruction in original order. Graph.release alone is
 // not a claim of the complete deleting destructor. No passive expiry D0.
 std::function<bool(const std::shared_ptr<world::CanonicalOpenableGraphV21>&,
  std::string&)> teardown_openable;
 // Query the actual Main source factory/transport publication authority.
 // The bool reports actual absence after unpublication, not an approval flag.
 std::function<bool(const world::CanonicalObjectBorrowV1&,bool&,std::string&)> transport_unpublished;
 // Optional only when Main already has its actual Item authority. No Item
 // entry is advertised without it; this part creates no Item pool/live owner.
 std::weak_ptr<character::WorldItemLiveOwnerV5> actual_items;
};
// Public typed lifecycle authority for THIS part's actual C1 journals.
// Storage can retire only after successful genuine teardown and absence from
// SAME manager and Main transport. Failed bodies/prefixes remain retained.
class ContainerCatalogOwnerV67 {
 struct Impl;std::unique_ptr<Impl> impl_;
 ContainerCatalogOwnerV67(ScopeV67,CatalogContainerServicesV67);
 friend bool make_container_catalog_part_v67(ScopeV67,CatalogContainerServicesV67,
  PartV67&,std::string&,std::shared_ptr<ContainerCatalogOwnerV67>*);
public:
 ~ContainerCatalogOwnerV67();
 ContainerCatalogOwnerV67(const ContainerCatalogOwnerV67&)=delete;
 bool construct(const world::CanonicalFactoryEntryV1&,
  const world::CanonicalSourceObjectRequestV1&,world::CanonicalClassReceiverV1&,std::string&);
 std::shared_ptr<world::CanonicalOpenableGraphV21> openable(std::uintptr_t)const;
 std::shared_ptr<world::CanonicalDestructibleContainerV16> destructible(std::uintptr_t)const;
 bool teardown(std::uintptr_t,std::string&);
 bool retire_after_unpublication(std::uintptr_t,std::string&);
 std::size_t retained_count()const noexcept;
};
// Actual entries: OpenableContainer340da4, DestructibleContainer340d5c,
// and Item340d38 only through Main's SAME existing Item factory/PropertyMap.
// Capture actual_owner for explicit typed release/retirement. Part.owner is
// that SAME public owner; neither API creates a new World/manager/VM.
bool make_container_catalog_part_v67(ScopeV67,CatalogContainerServicesV67,
 PartV67&,std::string&,std::shared_ptr<ContainerCatalogOwnerV67>* actual_owner=nullptr);
}
