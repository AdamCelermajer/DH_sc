#pragma once
#include "character_game_design.hpp"
#include "character_design_services.hpp"
#include "character_stance.hpp"
#include "../engine-skinning/visual_skin_owner_v6.hpp"
#include "../engine-ui/item_text_owner_v5.hpp"
#include "../game-data/combat.hpp"
#include <memory>
namespace dh2::ui {struct CharacterMenuItemActionsGraphV1;}

namespace dh2::player {
enum class EquipmentWorldQueryV1 : std::uint32_t {
 online=0x7fd794,online_player_record=0x36eea8,current_player=0x31f594,
 current_difficulty=1,player_count=0x4043a8,remotely_updated=0x33dd10
};
struct EquipmentWorldServicesV1 {
 void* context{};
 // Fresh source query each delivery. identity is full native Character identity;
 // scalar is the source bool/record/difficulty/count. No offline default here.
 bool(*invoke)(void*,EquipmentWorldQueryV1,std::uintptr_t subject,
               std::uintptr_t& identity,std::int32_t& scalar,std::string&){};
};
struct PlayerEquipmentRenderInputsV1 {
 character::CharacterGameDesign::Borrow design;
 std::shared_ptr<data::PropertyState> properties;
 data::LootRandom8V2* random{};
 std::uintptr_t character{};
 // Explicit caller projection of source inventory capacity; no inferred cap.
 std::int8_t potion_capacity{};
 std::int32_t language_pack{-1};
 skinning::VisualSkinResourcesV6::Borrow resources;
 const scene::Scene* live_scene{};
 // Logical data/pydata/... table URIs and data/3d/... weapon URIs. Synchronous
 // owned byte delivery. Missing is genuine absence; failed is required failure.
 skinning::VisualAssetServicesV6 assets;
 character::DebugSwitches* debug{};
 character::DebugFileServices24 debug_files{};
 ui::HudTextEnvironmentV1 text_environment;
 EquipmentWorldServicesV1 world;
 // Reached unimplemented notifications/powered continuations MUST be supplied
 // genuinely by caller or reject. Never a silent successful effect fixture.
 data::OwnedInventoryServicesV4 required;
};

// Portable stable heap owner, no GL dependency. All input service contexts,
// RNG and live Scene outlive binding. Retains the SAME property state, immutable
// design snapshot and authoritative V4 inventory through visual recreation.
class PlayerEquipmentRenderOwnerV1 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 explicit PlayerEquipmentRenderOwnerV1(PlayerEquipmentRenderInputsV1);
 ~PlayerEquipmentRenderOwnerV1();
 PlayerEquipmentRenderOwnerV1(const PlayerEquipmentRenderOwnerV1&)=delete;
 PlayerEquipmentRenderOwnerV1& operator=(const PlayerEquipmentRenderOwnerV1&)=delete;
 PlayerEquipmentRenderOwnerV1(PlayerEquipmentRenderOwnerV1&&)=delete;
 bool initialize(std::string&); // actual _InitEquipment, source prefix on failure
 bool ready()const noexcept;
 const data::FreshInventoryOwnedV4* inventory()const noexcept;
 const std::shared_ptr<data::PropertyState>& properties()const noexcept;
 data::PropertyView* property_view()noexcept; // same backing; live buffs may bind
 bool auto_equip(std::uint32_t,std::int32_t& result,std::string&);
 bool equip(std::uint32_t slot,std::uint32_t index,std::string&);
 bool unequip(std::uint32_t slot,std::string&);
 // NativeSwapEquipment442198's inventory/props/requirements/Skin/vitals
 // prefix. Its later DisplayRightHud/FillActionIcon are caller UI obligations.
 bool swap(std::string&);
 bool refresh_effects(std::string&);
 // Original RemoveOnePotion on the retained inventory; no replacement owner.
 bool remove_one_potion(std::string&);
 // Loot/pickup calls mutate the SAME inventory using its composed source
 // effects. Failed notifications preserve reached storage/gold prefixes;
 // incoming becoming null records actual consumption, even on failure.
 bool loot_add_item_v8(std::unique_ptr<data::ItemInstanceV1>& incoming,
                       bool force,bool convert_gold,std::int32_t& index,std::string&);
 bool loot_add_gold_v8(std::int32_t amount,std::string&);
 bool loot_inventory_full_v10(bool& full,std::string&);
 const data::InventoryGatheringIdsV11* gathering_ids_v11()const noexcept;
 bool register_gathering_id_v11(std::int32_t,std::string&);
 bool unregister_gathering_id_v11(std::int32_t,
  const data::InventoryGatheringAssertServicesV11&,std::string&);
 bool check_item_requirements_v1(std::string&);
 // Supplemental authored item actions borrow the SAME retained inventory,
 // effects and source Skin. World/achievement endpoints remain caller-owned.
 bool bind_menu_item_actions_v4(ui::CharacterMenuItemActionsGraphV1&,std::string&);
 // Borrow the exact immutable cache/text providers and caller-owned RNG used
 // by this Player's inventory. No alternate mutable inventory is exposed.
 bool loot_sources_v8(data::LootTablesV2::Borrow&,
                      data::ItemPowerTablesV5::Borrow&,
                      data::ItemTextServicesV5&,data::LootRandom8V2*&,
                      std::string&)const;
 void project_potion_capacity(std::int8_t)noexcept;
 bool swap_inventory_for_initial_slots(std::string&);
 bool draw_parts(std::vector<skinning::VisualDrawPartV6>&,std::string&)const;
 bool stance_facts(bool is_player,std::int32_t source_count,
                   character::StanceFacts16&,std::string&)const;
 // Supplies source category word37, raw HasTwoHander(true), offhand type22
 // and shield type6 to the existing combat kernel. Preserves state/combo_hits.
 bool combat_view(data::CombatantView&,std::string&)const;
 // Call BEFORE old live Scene is destroyed. Inventory/text/property/RNG stay.
 // Mutations requiring visual effects reject while detached. Rebind atomically
 // replaces resource owner after genuine Skin refresh; fresh Scene is borrowed.
 void detach_visual()noexcept;
 bool rebind_visual(skinning::VisualSkinResourcesV6::Borrow,
                    const scene::Scene&,std::string&);
};
}
