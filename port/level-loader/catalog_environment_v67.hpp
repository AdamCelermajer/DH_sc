#pragma once
#include "production_noncharacter_catalog_v67.hpp"
#include "canonical_light_point_factory_v53.hpp"
namespace dh2::loader {
// These pins exist only during a genuine engine leaf call. The retained
// callbacks below store ScopeV67 weak owners, never this delivery.
struct LightDeliveryV67 {
 std::shared_ptr<void> actual_world;
 std::shared_ptr<CanonicalLevelContextV1> level;
 std::shared_ptr<world::CanonicalObjectManagerV1> objects;
 world::CanonicalPropertyMapV1* properties{};
 // Set only during an actual receiver leaf, never retained by the provider.
 world::CanonicalLightPointV53* receiver{};
 std::shared_ptr<void> runtime_owner; //SAME independent selected factory service owner, call-duration only.
};
struct LightEnvironmentLeavesV67 {
 // Independent runtime authority; no strong containing World/Level/manager
 // aliases or captures. Same restriction applies to every callback and names.
 std::shared_ptr<void> owner;
 std::shared_ptr<world::LightSetNameOwnerV3> names;
 std::function<bool(const std::shared_ptr<void>&,std::string&)> register_delivery_owner_v113;
 //Native typed association with the existing class record, independent of
 //the actual source journal admission callback installed by V69.
 std::function<bool(const LightDeliveryV67&,const std::shared_ptr<CanonicalLightPointRecordV53>&,std::string&)> observe_record_v113;
 std::function<bool(const LightDeliveryV67&,std::string&)> validate_current;
 std::function<bool(const LightDeliveryV67&,std::uintptr_t,std::unique_ptr<world::LightObjectBaseContinuationV67>&,std::string&)> bind_objectbase_dtor;
 std::function<bool(const LightDeliveryV67&,const std::string&,const char*,bool,bool,
  world::LightNodeBorrowV53&,std::string&)> first_scene_light;
 std::function<bool(const LightDeliveryV67&,bool,world::LightNodeBorrowV53&,std::string&)> construct_light_node;
 std::function<bool(const LightDeliveryV67&,const world::LightNodeBorrowV53&,std::string&)> attach_root;
 std::function<bool(const LightDeliveryV67&,std::uintptr_t,const world::LightNodeBorrowV53&,std::string&)> add_automatic;
 std::function<bool(const LightDeliveryV67&,const world::LightNodeBorrowV53&,world::LightParameterFieldsV53&,std::string&)> borrow_light_parameters;
 std::function<bool(const LightDeliveryV67&,const char*,bool&,std::string&)> debug_switch;
 std::function<bool(const LightDeliveryV67&,const world::LightNodeBorrowV53&,std::uint16_t,std::string&)> set_type;
 std::function<bool(const LightDeliveryV67&,std::uintptr_t,const world::LightNodeBorrowV53&,world::LightSetNameOwnerV3&,std::string&)> refresh_attachment;
 std::function<bool(const LightDeliveryV67&,world::LightPointAttachmentServicesV113&,std::string&)> bind_attachment_v113;
 // Admit the SAME record to Main's existing actual source-release journal
 // BEFORE C1. The record's owner is produced by the factory afterwards. The
 // journal must retain it on every reached InitPost failure/native-node prefix.
 // A passive shared_ptr drop is not native actor120 intrusive release/D0.
 std::function<bool(const LightDeliveryV67&,const std::shared_ptr<CanonicalLightPointRecordV53>&,
  const world::CanonicalSourceObjectRequestV1&,std::string&)> retain_release_record;
 // Actual journal proves native teardown, manager unpublication and transport
 // erased(identity) completed. False retains the factory record for retry.
 std::function<bool(const LightDeliveryV67&,std::uintptr_t,std::string&)> release_completed;
};
class CatalogEnvironmentV67 final : public std::enable_shared_from_this<CatalogEnvironmentV67> {
 ScopeV67 scope_;std::shared_ptr<const LightEnvironmentLeavesV67> leaves_;
 std::unique_ptr<CanonicalLightPointFactoryV53> factory_;
 bool release_busy_{},release_reentered_{},release_started_{},all_prepared_{};
 CatalogEnvironmentV67()=default;
public:
 static bool create(ScopeV67,std::shared_ptr<const LightEnvironmentLeavesV67>,
  std::shared_ptr<CatalogEnvironmentV67>&,PartV67&,std::string&);
 bool construct(const world::CanonicalFactoryEntryV1&,const world::CanonicalSourceObjectRequestV1&,
  world::CanonicalClassReceiverV1&,std::string&);
 // Prepare every authored/failed InitPost receiver before any Flush mutation.
 bool prepare_source_release(std::shared_ptr<world::LightQuiescenceLeaseV67>,std::string&);
 bool execute_source_release(std::uintptr_t,std::shared_ptr<world::LightQuiescenceLeaseV67>,std::string&);
 bool erase_after_source_release(std::uintptr_t,std::string&);
 bool source_frame_update_v113(std::uintptr_t,std::string&);
 bool source_borrow_v113(std::uintptr_t,std::shared_ptr<world::CanonicalLightPointV53>&,std::string&);
};
}


