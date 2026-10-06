#pragma once
#include "canonical_family_fields_v15.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"

namespace dh2::world {
class CanonicalQuestMoveZoneV31;
struct QuestMoveZoneServicesV31 {
 std::shared_ptr<void> owner;
 std::function<bool(CanonicalQuestMoveZoneV31&,std::string&)> whole_zone_init_post;
 std::function<bool(CanonicalQuestMoveZoneV31&,std::uintptr_t,std::string&)> whole_collision_begin;
 std::function<bool(CanonicalQuestMoveZoneV31&,std::string&)> whole_destroy;
};
// Source factory340f50 -> QuestMoveInZone396330 -> Zone397ca0 -> GameObject20.
// One canonical base/runtime; source quest collision behavior is a required
// engine service, never an independent quest implementation.
class CanonicalQuestMoveZoneV31 {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 initialization_services_;
 GameObjectInitializationOwnerV1 initialization_;
 QuestMoveZoneServicesV31 services_;
 std::array<float,3> dimensions374_{};
 bool physical380_{true},trigger381_{false};
 std::uintptr_t colzone384_{};std::shared_ptr<void> colzone_lease_;
 bool destroyed_{};
public:
 CanonicalQuestMoveZoneV31(std::shared_ptr<void>,actor::RuntimeState&,
  GameObjectInitializationServicesV1,QuestMoveZoneServicesV31);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 CanonicalPropertyActorV1 properties()noexcept;
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);
 bool write_bool(std::uint32_t,std::uint8_t,std::string&);
 bool write_int(std::uint32_t,std::int32_t,std::string&);
 bool write_string(std::uint32_t,const std::string&,std::string&);
 bool write_vector3(std::uint32_t,const std::array<float,3>&,std::string&);
 const std::array<float,3>& dimensions()const noexcept{return dimensions374_;}
 bool physical()const noexcept{return physical380_;}bool trigger()const noexcept{return trigger381_;}
 std::uintptr_t& source_colzone384()noexcept{return colzone384_;}
 std::shared_ptr<void>& source_colzone_lease()noexcept{return colzone_lease_;}
 bool game_object_init_post(bool& eligible,std::string& e){return initialization_.init_post(eligible,e);}
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 bool init_post(std::string&);
 bool collision_begin(std::uintptr_t,std::string&);
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalQuestMoveZoneV31>,std::shared_ptr<const void>);
};
}
