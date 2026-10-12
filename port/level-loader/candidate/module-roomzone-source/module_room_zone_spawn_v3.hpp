#pragma once
#include "canonical_room_zone_factory_v3.hpp"
#include "canonical_spawn_owner_v1.hpp"
namespace dh2::world {
// Owns attempts/receiver dispatch only; SAME canonical manager owns publication.
// Caller supplies genuine Handle resolver and outer TestEnableCondition services.
class ModuleRoomZoneSpawnV3 {
 CanonicalObjectManagerV1& manager_;CanonicalPropertyMapV1& properties_;
 CanonicalRoomZoneFactoryV3& factory_;CanonicalSpawnServicesV1 source_;
 std::map<std::uintptr_t,CanonicalClassReceiverV1> receivers_;
 std::vector<std::unique_ptr<CanonicalSpawnAttemptV1>> attempts_;
 static bool construct(void*,const CanonicalFactoryEntryV1&,CanonicalClassReceiverV1&,std::string&);
 static bool resolve(void*,target_providers::Handle16&,bool,const CanonicalObjectBorrowV1*&,std::string&);
 static bool condition(void*,const CanonicalObjectBorrowV1&,bool,std::string&);
 static bool updatable(void*,const CanonicalObjectBorrowV1&,bool&,std::string&);
 static bool pending(void*,const CanonicalObjectBorrowV1&,std::string&);
 static bool receiver(void*,const CanonicalObjectBorrowV1&,const CanonicalClassReceiverV1*&,std::string&);
public:
 ModuleRoomZoneSpawnV3(CanonicalObjectManagerV1& m,CanonicalPropertyMapV1& p,
  CanonicalRoomZoneFactoryV3& f,CanonicalSpawnServicesV1 s):manager_(m),properties_(p),factory_(f),source_(s){}
 void erased_after_source_release_v91(std::uintptr_t);
 bool spawn(const std::string&,target_providers::Handle16&,std::uint32_t&,std::string&);
 bool init(target_providers::Handle16&,const std::array<float,6>&,std::uintptr_t,std::string&);
};
}
