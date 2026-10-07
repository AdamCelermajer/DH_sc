#pragma once
#include "world_loot_gameplay_v23.hpp"
#include "../level-loader/canonical_level_context_v1.hpp"
#include "../game-data/player_savegame_v1.hpp"

namespace dh2::character {
// The existing retained CharacterGameDesign snapshot's actual PyDataConstants
// receiver. A source lookup miss0 remains0; no PickUpType literal is guessed.
bool world_loot_design_constant_v44(const dh2_script_design_bindings*,const char*,const char*,std::int32_t&,std::string&);
// A borrow of the actual Character+14e8 Save, including a genuine NULL Save.
// The callback must not substitute the selected menu slot or a class default.
struct LootCharacterSaveBorrowV44 {
 std::shared_ptr<void> character_lease,save_lease;
 const data::PlayerSavegameV1* save{};
};
// Borrowed fields of a published physical peer; the lease keeps those fields
// alive for the synchronous Box2D callback. Unknown peer addresses fail.
struct LootPhysicalPeerBorrowV44 {
 const world::CanonicalObjectBorrowV1* object{};
 const std::uint8_t* visible80{};
 const std::uintptr_t* character_ooi14a4{};
 std::shared_ptr<void> receiver_lease;
};
struct WorldLootCanonicalServicesV44 {
 std::shared_ptr<void> application_lease;
 void* context{};
 // The same V23 owner may be under PreCache construction. This is a borrow,
 // not another Item factory, registry or manager.
 WorldItemLiveOwnerV5*(*items)(void*){};
 bool(*character_save)(void*,std::uintptr_t,LootCharacterSaveBorrowV44&,std::string&){};
 bool(*condition_is_true)(void*,std::uintptr_t,bool&,std::string&){};
 bool(*null_handle_assertion)(void*,std::string&){};
 bool(*unknown_type_debug)(void*,const char*,std::string&){};
 bool(*physical_peer)(void*,void*,LootPhysicalPeerBorrowV44&,std::string&){};
 bool(*character_fields)(void*,std::uintptr_t,LootPhysicalPeerBorrowV44&,std::string&){};
 // SAME SM.state20 receiver->id; NULL means the actual absent StateInfo,
 // whose original SM_GetState returns -1. Lease must pin its live machine.
 bool(*character_state)(void*,std::uintptr_t,const std::int32_t*&,std::shared_ptr<void>&,std::string&){};
 bool(*destroy_previous_physical)(void*,std::uintptr_t,std::string&){};
};
// Concrete source composition, retaining the existing manager/PM/GS authorities.
// No stage/PM count/Save readiness writes, and no late-pool InitFinal replay.
class WorldLootCanonicalBindingsV44 {
 world::CanonicalObjectManagerV1& manager_;
 player::PlayerManagerOwnerV1& players_;
 loader::CanonicalGSLevelGlobalSlotV1 gs_;
 WorldLootCanonicalServicesV44 services_;
 world::CanonicalItemFactoryServicesV2 upstream_factory_;
 struct ConditionScope {WorldLootCanonicalBindingsV44& owner;RetainedWorldItemObjectV1& item;};
 bool canonical(std::uintptr_t,const world::CanonicalObjectBorrowV1*&,std::string&);
 bool peer(void*,LootPhysicalPeerBorrowV44&,std::string&);
 bool validate_peer(const LootPhysicalPeerBorrowV44&,std::string&);
 static bool resolve(void*,target_providers::Handle16&,bool,const world::CanonicalObjectBorrowV1*&,std::string&);
 static bool condition(void*,const world::CanonicalObjectBorrowV1&,bool,std::string&);
 static bool debug(void*,const char*,std::string&);
 static bool local_profile(void*,bool&,bool&,std::uint8_t&,std::string&);
 static bool level_condition(void*,bool&,std::int32_t&,std::string&);
 static bool condition_truth(void*,std::uintptr_t,bool&,std::string&);
 static bool enabled_event(void*,bool,std::string&);
public:
 WorldLootCanonicalBindingsV44(world::CanonicalObjectManagerV1&,player::PlayerManagerOwnerV1&,
  loader::CanonicalGSLevelGlobalSlotV1,WorldLootCanonicalServicesV44);
 world::CanonicalItemFactoryServicesV2 factory_services(world::CanonicalItemFactoryServicesV2);
 // Call during V23 graph_services. V5 installs its SAME UpdatePF afterwards.
 bool physical_services(RetainedWorldItemObjectV1&,WorldItemPhysicalServicesV2&,std::string&);
 bool current_level(LootCurrentLevelV23&,std::string&);
 static bool current_level_callback(void*,LootCurrentLevelV23&,std::string&);
 bool test_enable_condition(const world::CanonicalObjectBorrowV1&,bool,std::string&);
 bool outer_item(const WorldItemRequestV1&,std::int32_t&,bool& handled,std::string&);
};
}
