#pragma once
#include "canonical_room_zone_v3.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
namespace dh2::world {
struct CanonicalRoomZoneRecordV3 {
 actor::RuntimeState runtime;
 std::unique_ptr<CanonicalRoomZoneV3> receiver;
 bool constructor_started_v91{},constructor_completed_v91{},constructor_failed_v91{};
};
struct CanonicalRoomZoneConstructionV3 {
 std::shared_ptr<void> world;
 // Actual release journal retains SAME record before C1; failure is retained.
 std::function<bool(const std::shared_ptr<CanonicalRoomZoneRecordV3>&,const CanonicalFactoryEntryV1&,std::string&)> admit_before_c1_v91;
 std::function<bool(const std::shared_ptr<CanonicalRoomZoneRecordV3>&,const CanonicalClassReceiverV1&,std::string&)> admit_after_c1_v91;
 // Services capture weak record, then borrow its receiver after construction.
 std::function<bool(const std::shared_ptr<CanonicalRoomZoneRecordV3>&,RoomZoneServicesV3&,std::string&)> services;
};
// Constructor adapter for SAME CanonicalSpawnAttemptV1. Registration,
// defaults, name/condition and pending publication remain that source owner.
class CanonicalRoomZoneFactoryV3 {
 CanonicalRoomZoneConstructionV3 construction_;
 std::map<std::uintptr_t,std::shared_ptr<CanonicalRoomZoneRecordV3>> records_;
public:
 explicit CanonicalRoomZoneFactoryV3(CanonicalRoomZoneConstructionV3 c):construction_(std::move(c)){}
 bool construct(const CanonicalFactoryEntryV1&,CanonicalClassReceiverV1&,std::string&);
 CanonicalRoomZoneV3* find(std::uintptr_t)noexcept;
 std::shared_ptr<CanonicalRoomZoneRecordV3> record_v91(std::uintptr_t id)const {auto p=records_.find(id);return p==records_.end()?nullptr:p->second;}
 // Call only after SAME manager/pending/Room membership unpublication.
 void erased(std::uintptr_t id){records_.erase(id);}
};
}
