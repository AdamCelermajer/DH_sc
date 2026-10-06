#pragma once
#include "canonical_room_zone_v3.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
namespace dh2::world {
struct CanonicalRoomZoneRecordV3 {
 actor::RuntimeState runtime;
 std::unique_ptr<CanonicalRoomZoneV3> receiver;
};
struct CanonicalRoomZoneConstructionV3 {
 std::shared_ptr<void> world;
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
 // Call only after SAME manager/pending/Room membership unpublication.
 void erased(std::uintptr_t id){records_.erase(id);}
};
}
