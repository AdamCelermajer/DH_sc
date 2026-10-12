#pragma once
#include "canonical_family_fields_v15.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "canonical_trigger_zone_v22.hpp" // same source398da4 network projection
#include <optional>
namespace dh2::world {
class CanonicalTriggerObjectV28;
struct TriggerObjectServicesV28 {
 std::shared_ptr<void> owner;
 std::function<bool(CanonicalTriggerObjectV28&,std::string&)> whole_init_post,whole_init_final,whole_update,whole_destroy;
 std::function<bool(CanonicalTriggerObjectV28&,std::uintptr_t,std::string&)> whole_interact;
};
// Distinct source340f0c -> TriggerObject399df0 -> Trigger398fd4 ->
// ZoneEx397f28 -> Zone397ca0 -> GameObject20. No TriggerZone substitution.
class CanonicalTriggerObjectV28 {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 initialization_services_;
 TriggerObjectServicesV28 services_;
 std::array<float,3> dimensions374_{};
 bool physical380_{},trigger381_{true};
 std::uintptr_t colzone384_{};std::shared_ptr<void> colzone_lease_;
 std::set<std::uintptr_t> contacts_;
 std::int32_t characters3a0_{},players3a4_{},count3a8_{1},delay3ac_{},activated3b4_{},timer3b8_{},touching3c0_{};
 std::uint8_t reset3b0_{},local_only3bc_{};
 std::array<TriggerNetworkOwnerV22,2> network_;
 std::array<std::string,4> names_;
 std::int32_t data_id730_{-1},script_id74c_{-1};
 std::optional<std::int32_t> script_id768_; // original C1 leaves this unproduced
 std::uint8_t byte784_{};
 std::uintptr_t condition788_{};std::shared_ptr<void> condition_lease_;
 bool destroyed_{};
public:
 CanonicalTriggerObjectV28(std::shared_ptr<void>,actor::RuntimeState&,GameObjectInitializationServicesV1,TriggerObjectServicesV28);
 CanonicalTriggerObjectV28(const CanonicalTriggerObjectV28&)=delete;
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
 std::set<std::uintptr_t>& source_contacts()noexcept{return contacts_;}
 std::int32_t* source_integer(std::uint32_t)noexcept;
 std::uint8_t* source_byte(std::uint32_t)noexcept;
 std::string* source_string(std::uint32_t)noexcept;
 std::optional<std::int32_t>& source_script_id768()noexcept{return script_id768_;}
 std::uintptr_t& source_condition788()noexcept{return condition788_;}
 std::shared_ptr<void>& source_condition_lease()noexcept{return condition_lease_;}
 TriggerNetworkOwnerV22& source_network(unsigned index){return network_.at(index);}
 bool init_post(std::string&);bool init_final(std::string&);bool update(std::string&);
 bool interact(std::uintptr_t,std::string&);
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalTriggerObjectV28>,std::shared_ptr<const void>);
};
}
