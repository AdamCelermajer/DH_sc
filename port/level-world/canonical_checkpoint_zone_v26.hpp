#pragma once
#include "canonical_family_fields_v15.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "zone_startup_v76.hpp"
#include <set>

namespace dh2::world {
class CanonicalCheckpointZoneV26;
struct CheckpointZoneServicesV26 {
 std::shared_ptr<void> owner;
 ZoneStartupServicesV76 startup;
 std::function<bool(CanonicalCheckpointZoneV26&,std::uintptr_t,std::string&)> whole_collision_begin;
 std::function<bool(CanonicalCheckpointZoneV26&,std::string&)> whole_destroy;
};
// Source factory340f2c -> Checkpoint39594c -> Zone397ca0 -> GameObject12.
// One canonical base/runtime; source checkpoint/save behavior is a required
// engine service, never an independent campaign/save implementation.
class CanonicalCheckpointZoneV26 {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 initialization_services_;
 GameObjectInitializationOwnerV1 initialization_;
 CheckpointZoneServicesV26 services_;
 std::array<float,3> dimensions374_{};
 bool physical380_{true},trigger381_{true};
 std::uintptr_t colzone384_{};std::shared_ptr<void> colzone_lease_;
 // Actual Checkpoint C1 39594c constructs the empty receiver tree388.
 std::set<std::uintptr_t> checkpoint388_;
 bool destroyed_{};
public:
 CanonicalCheckpointZoneV26(std::shared_ptr<void>,actor::RuntimeState&,
  GameObjectInitializationServicesV1,CheckpointZoneServicesV26);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 CanonicalPropertyActorV1 properties()noexcept;
 bool read_bool(std::uint32_t,std::uint8_t&,std::string&);
 bool write_bool(std::uint32_t,std::uint8_t,std::string&);
 bool write_int(std::uint32_t,std::int32_t,std::string&);
 bool write_string(std::uint32_t,const std::string&,std::string&);
 bool write_vector3(std::uint32_t,const std::array<float,3>&,std::string&);
 std::array<float,3>& source_dimensions374()noexcept{return dimensions374_;}
 const std::array<float,3>& dimensions()const noexcept{return dimensions374_;}
 bool physical()const noexcept{return physical380_;}bool trigger()const noexcept{return trigger381_;}
 std::uintptr_t& source_colzone384()noexcept{return colzone384_;}
 std::shared_ptr<void>& source_colzone_lease()noexcept{return colzone_lease_;}
 std::set<std::uintptr_t>& source_checkpoint388()noexcept{return checkpoint388_;}
 bool game_object_init_post(bool& eligible,std::string& e){return initialization_.init_post(eligible,e);}
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 bool init_post(std::string&);
 bool collision_begin(std::uintptr_t,std::string&);
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalCheckpointZoneV26>,std::shared_ptr<const void>);
};
}
