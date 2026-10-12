#pragma once
#include "retained_character_actor_v1.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "npc_inventory_owner_v1.hpp"
namespace dh2::world {
struct CanonicalCharacterRecordV4;
struct CanonicalCharacterFamilyServicesV4 {
 std::shared_ptr<void> world;
 character::CharacterGameDesign* design{};
 const data::Dictionary* models{};
 data::LootTablesV2* loot_tables{};
 data::LootRandom8V2* loot_random{};
 std::function<bool(CanonicalCharacterRecordV4&,character::WorldNpcStateServicesV1&,std::string&)> state_services;
 std::function<bool(CanonicalCharacterRecordV4&,const character::NpcInitPostRequestV1&,character::NpcInitPostResponseV1&,std::string&)> remaining_init;
 std::function<bool(CanonicalCharacterRecordV4&,const std::array<float,3>&,bool,std::string&)> set_position;
};
// Same native receiver identity, sheets, inventory, controller/FSM and InitPost
// fields. Source tables are pinned by the borrowed actual GameDesign snapshot.
struct CanonicalCharacterRecordV4 {
 std::shared_ptr<character::RetainedCharacterActorV1> actor;
 character::CharacterGameDesign::Borrow design;
 std::shared_ptr<data::PropertyState> properties;
 std::shared_ptr<data::CombatActorState> life;
 data::PropertyView view{};
 std::unique_ptr<character::NpcInventoryOwnerV1> inventory;
 std::unique_ptr<character::CharacterNpcInitPostOwnerV1> init;
 character::NpcInitPostBorrowV1 fields{};
 CanonicalCharacterFamilyServicesV4 services;
 bool init_attempted{};
 bool initialize(std::string&);
 static bool invoke(void*,const character::NpcInitPostRequestV1&,character::NpcInitPostResponseV1&,std::string&);
};
class CanonicalCharacterFamilyFactoryV4 {
 CanonicalCharacterFamilyServicesV4 services_;
 std::map<std::uintptr_t,std::shared_ptr<CanonicalCharacterRecordV4>> records_;
public:
 explicit CanonicalCharacterFamilyFactoryV4(CanonicalCharacterFamilyServicesV4 s):services_(std::move(s)){}
 bool construct(const CanonicalFactoryEntryV1&,const CanonicalSourceObjectRequestV1&,CanonicalClassReceiverV1&,std::string&);
 std::shared_ptr<CanonicalCharacterRecordV4> find(std::uintptr_t)const;
 void erase_after_unpublication(std::uintptr_t identity){records_.erase(identity);}
};
}
