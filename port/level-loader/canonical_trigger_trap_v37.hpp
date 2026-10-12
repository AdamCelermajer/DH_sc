#pragma once
#include "canonical_family_fields_v15.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include <optional>
#include <set>
namespace dh2::world {
class CanonicalTriggerTrapV37;
struct TriggerTrapServicesV37 {
 std::shared_ptr<void> owner;
 // Must execute original whole methods on this SAME receiver graph.
 // InitPost includes actual application RNG, data lookup, Zone/PF, condition,
 // retained Visual timeline callbacks, sound and external Lua script services.
 std::function<bool(CanonicalTriggerTrapV37&,std::string&)> whole_init_post,whole_init_final,whole_update,whole_destroy;
};
// Source factory340e7c -> TriggerTrapC1 39ecd4 -> ZoneEx397f28 ->
// Zone397ca0 -> GameObject(GO_ID15). Distinct from Trigger/TriggerZone.
// Native fields are semantic borrows, never a claim of ARM32 ABI layout.
class CanonicalTriggerTrapV37 {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 initialization_services_;
 TriggerTrapServicesV37 services_;
 std::array<float,3> dimensions374_{};
 bool physical380_{true},trigger381_{};
 std::uintptr_t colzone384_{};std::shared_ptr<void> colzone_lease_;
 std::set<std::uintptr_t> contacts388_;
 std::int32_t zone_count3a0_{},zone_count3a4_{};
 std::string data3a8_;
 std::int32_t data_id3c0_{-1};std::uint8_t activated3c4_{};
 std::set<std::uintptr_t> victims3c8_,previous_victims3e0_;
 std::optional<std::int32_t> timer3f4_; // C1 does not produce this field.
 std::uintptr_t owner3f8_{};std::shared_ptr<void> owner_lease_;
 std::int32_t damager3fc_{-1};std::uint8_t byte400_{},byte401_{};
 bool destroyed_{};
public:
 CanonicalTriggerTrapV37(std::shared_ptr<void>,actor::RuntimeState&,GameObjectInitializationServicesV1,TriggerTrapServicesV37);
 CanonicalTriggerTrapV37(const CanonicalTriggerTrapV37&)=delete;
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
 std::set<std::uintptr_t>& source_contacts388()noexcept{return contacts388_;}
 std::set<std::uintptr_t>& source_victims3c8()noexcept{return victims3c8_;}
 std::set<std::uintptr_t>& source_previous_victims3e0()noexcept{return previous_victims3e0_;}
 std::int32_t* source_integer(std::uint32_t)noexcept;
 std::uint8_t* source_byte(std::uint32_t)noexcept;
 std::string* source_string(std::uint32_t)noexcept;
 std::optional<std::int32_t>& source_timer3f4()noexcept{return timer3f4_;}
 std::uintptr_t& source_owner3f8()noexcept{return owner3f8_;}
 std::shared_ptr<void>& source_owner_lease()noexcept{return owner_lease_;}
 bool init_post(std::string&);bool init_final(std::string&);bool update(std::string&);
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalTriggerTrapV37>,std::shared_ptr<const void>);
};
}
