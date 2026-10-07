#pragma once
#include "canonical_family_fields_v15.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "canonical_trigger_zone_v22.hpp" // same source398da4 network projection
#include <optional>
#include "zone_startup_v76.hpp"
#include "source_startup_sound_v87.hpp"
#include "condition_data_init_v3.hpp"
#include "../game-data/game_object_dictionary_v11.hpp"
#include <script_manager_owner_v52.hpp>
namespace dh2::data {class PlayerSavegameV1;}
namespace dh2::world {
class CanonicalTriggerObjectV28;
struct TriggerObjectLocalPlayerV91 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};const std::uintptr_t* character660{};
};
struct TriggerObjectSavedCharacterV91 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};
 const std::uintptr_t* save14e8{};std::shared_ptr<data::PlayerSavegameV1> save;
};
struct TriggerObjectRestoreServicesV91 {
 std::shared_ptr<void> owner,condition_owner; // genuine independent native authorities
 std::function<bool(std::int32_t,bool,TriggerObjectLocalPlayerV91&,std::string&)> local_player;
 std::function<bool(std::uintptr_t,TriggerObjectSavedCharacterV91&,std::string&)> character_save;
 std::function<bool(std::uintptr_t,bool&,std::string&)> evaluate_condition;
};
// Only consumed fields from the SAME decoded source24-byte TriggerObjects
// row. Its native CStringc is not Door's signed soundc field or an ARM pointer.
struct TriggerObjectDeclarationRowV78 {std::int32_t raw4{};std::string external_script_c;std::int32_t sound10{},visual14{};};
struct TriggerObjectTableBorrowV78 {
 std::shared_ptr<const void> receiver;
 const std::vector<std::string>* names{};
 const std::vector<TriggerObjectDeclarationRowV78>* rows{};
 std::shared_ptr<const data::GameObjectDictionaryV11> objects;
};
struct TriggerObjectStartupServicesV78 {
 std::shared_ptr<void> owner;ZoneStartupServicesV76 zone;
 std::function<bool(TriggerObjectTableBorrowV78&,std::string&)> borrow_tables;
 // SAME App ScriptManager with loaded native names; no scheduler/VM/Execute
 // implementation is introduced here. GetIDFromName(false) consumes it only.
 std::function<bool(std::shared_ptr<loader::ScriptManagerOwnerV52>&,std::string&)> borrow_script_manager;
 // SAME Main arena's RETURNED V3 services, used for custom compiled788.
 std::function<bool(ConditionDataInitServicesV3&,std::string&)> borrow_conditions;
 std::function<bool(CanonicalTriggerObjectV28&,std::uintptr_t,const char*,bool,std::int32_t,std::uint32_t,std::string&)> play_idle_animation;
 // Actual40-byte46f2f0 resource C1 +derived dispatch+394bf8(false), arguments
 // App44 world, THIS,1,stack0/0/0/0/2/0xffff/1; journal every failed prefix.
 std::function<bool(CanonicalTriggerObjectV28&,std::string&)> create_physical;
 std::function<bool(StartupSoundManagerBorrowV87&,std::string&)> borrow_sound_manager;
 // GameObject38ef60 with actual rowCString and exact path prefix. This is
 // existing object Lua-resource setup, not the ScriptManager80 scheduler.
 std::function<bool(CanonicalTriggerObjectV28&,const char*,const char*,std::string&)> load_external_script;
 std::function<bool(CanonicalTriggerObjectV28&,bool,std::string&)> source_delete;
};
struct TriggerObjectServicesV28 {
 std::shared_ptr<void> owner;TriggerObjectStartupServicesV78 startup;
 std::function<bool(CanonicalTriggerObjectV28&,std::string&)> whole_update;
 // Original inherited TriggerD1399214 AFTER custom788 D1/free and four local
 // CStrings. Main owns actual network/Zone/native-resource/GameObjectD2 order
 // and its retry journal; it must never destroy custom788 a second time.
 std::function<bool(CanonicalTriggerObjectV28&,std::string&)> destroy_trigger_base;
 std::function<bool(CanonicalTriggerObjectV28&,std::uintptr_t,std::string&)> whole_interact;
};
// Distinct source340f0c -> TriggerObject399df0 -> Trigger398fd4 ->
// ZoneEx397f28 -> Zone397ca0 -> GameObject20. No TriggerZone substitution.
class CanonicalTriggerObjectV28 {
 CanonicalGameObjectBaseOwnerV1 base_;
 GameObjectInitializationServicesV1 initialization_services_;
 GameObjectInitializationOwnerV1 initialization_;
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
 // Host lifetime bookkeeping only; no additional native ConditionData field.
 std::function<bool(std::uintptr_t,std::string&)> destroy_condition788_;
 bool destroyed_{},destruction_started_{},destroying_{};
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
 std::array<float,3>& source_dimensions374()noexcept{return dimensions374_;}
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
 bool source_test_interactive_condition_v91(const TriggerObjectRestoreServicesV91&,std::string&);
 bool source_can_activate_v91(bool&,std::string&);
 bool set_position(const std::array<float,3>&,bool,std::string&);
 bool destroy(std::string&);
 static CanonicalClassReceiverV1 factory_receiver(std::shared_ptr<CanonicalTriggerObjectV28>,std::shared_ptr<const void>);
};
}
