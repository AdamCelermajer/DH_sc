#pragma once
#include "character_game_design.hpp"
#include "character_design_services.hpp"
#include "character_stance.hpp"
#include "../engine-skinning/visual_skin_owner_v6.hpp"
#include "../engine-ui/item_text_owner_v5.hpp"
#include "../game-data/combat.hpp"
#include <memory>
namespace dh2::ui {struct CharacterMenuItemActionsGraphV1;}
namespace dh2::data {struct InventoryLoadReceiptV1;}

namespace dh2::player {
class PlayerEquipmentRenderOwnerV1;
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
 //Actual campaign Arrays already produced before XML/Item manager. These
 //immutable borrows share row identities with source145 and avoid decoding a
 //second Loot/Power authority during the later selected Character Gear init.
 data::LootTablesV2::Borrow immutable_loot_v88;
 data::ItemPowerTablesV5::Borrow immutable_powers_v88;
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
 // Source selected profile GEAR read BEFORE _InitEquipment/InitialGrant.
 // Absent section is distinct from empty/truncated present section. Positive
 // bytes remain pinned by an actual cache/profile lease through the load.
 std::function<bool(bool& present,data::Bytes&,std::shared_ptr<const void>&,
  std::string&)> saved_gear_v50;
 // Source campaign bootstrap executes the actual SG_Load block after SAME
 // Inventory/Gear/Skin creation and before InitialGrant. Mutually exclusive
 // with saved_gear_v50; no second GEAR append or successful skipped reader.
 std::function<bool(PlayerEquipmentRenderOwnerV1&,std::string&)> source_profile_load_v59;
 // Actual embedded Character C1 inventory37c, moved into this ownership
 // capsule once. No vector/slot copy or second inventory C1 is permitted.
 std::unique_ptr<data::FreshInventoryOwnedV4> constructed_inventory_v60;
 const std::uintptr_t* source_visual2d8_v62{};
 // Pins actual platform/cache/text service storage through VM/Gear teardown.
 // The storage keeps actor/World references weak to avoid a service cycle.
 std::shared_ptr<void> services_lease_v62;
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
 bool initialize(std::string&); // prepare/restore + actual _InitEquipment
 // SG_Load4 phase followed by source class/stats/capacity/marker stores,
 // then _InitEquipment. SAME owner/inventory; failed prefixes cannot retry.
 bool prepare_restore_v60(std::string&);
 bool finish_initial_grants_v60(std::string&);
 bool prepared_v60()const noexcept;
 bool source_initpost_gold_store_v61(std::int32_t,std::string&);
 bool source_reset_gear_properties_v61(std::string&);
 bool source_load_gear_properties_v61(std::string&);
 // Only accepted inside source_profile_load_v59. Parser/effects/PowerNames
 // are borrowed from this SAME Gear owner. One attempt per fresh inventory.
 bool load_saved_section_v59(data::Bytes,data::InventoryLoadReceiptV1&,std::string&);
 // Later original SG_Load4 merges/appends into this SAME live inventory.
 bool load_source_inventory_v122(data::Bytes,data::InventoryLoadReceiptV1&,std::string&);
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
 bool try_consuming_source_v108(std::int32_t id,std::int32_t quantity,bool& consumed,std::string&);
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
 bool draw_views(const std::vector<skinning::VisualDrawViewV32>*&,std::string&)const;
 skinning::SkinPoseCountersV32 pose_counters()const noexcept;
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
