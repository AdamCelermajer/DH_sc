#pragma once
#include <source_item_resources_v88.hpp>
#include <world_loot_gameplay_v23.hpp>
#include <world_loot_canonical_bindings_v44.hpp>
#include <memory>
#include <functional>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
class SourceCampaignItemsV88;
//Only leaves whose actual source receivers belong to other native subsystems.
//Core factory/145/physical/pickup/storage/quest/frame control is composed here.
//Absent positive UI/network/platform branches fail when reached, not at C1.
struct SourceCampaignItemLeavesV88 {
 std::shared_ptr<void> owner;
 std::shared_ptr<dh2::world::LightSetNameOwnerV3> light_names;
 dh2::sound::VoxPlay3DServicesV2 sound;
 std::uintptr_t vox_identity{};
 std::function<bool(const char*,std::uint32_t,std::string&)> enqueue_status;
 std::function<bool(dh2::data::LootTemporaryInventoryV8&,dh2::data::ItemInstanceV1&,std::string&)> full_notifications;
 std::function<bool(const dh2::data::LootEntryRequestV8&,std::int32_t&,std::string&)> assertion;
 std::function<bool(const dh2::character::WorldItemRequestV1&,std::int32_t&,std::string&)> item_outer;
 std::function<bool(const dh2::character::LootInteractRequestV8&,dh2::character::LootInteractResponseV8&,std::string&)> pickup;
 std::function<bool(dh2::character::WorldItemGraphV3&,dh2::character::WorldItemFrameServicesV5&,std::string&)> frame;
 std::function<bool(std::uintptr_t,std::string&)> release_item_ui;
};
bool prepare_source_campaign_items_v88(const SourceCampaignCandidateBorrowV55&,SourceCampaignItemLeavesV88,std::string&);
//Original caller owns stage29. This never executes InitFinal on late145 items.
bool precache_source_campaign_items_v88(const SourceCampaignCandidateBorrowV55&,std::string&);
bool update_source_campaign_items_v88(const std::shared_ptr<void>& actual_world,
 std::uint32_t actual_absolute_ms,std::uint32_t actual_dt_ms,std::string&);
bool source_campaign_item_update_v104(const std::shared_ptr<void>& actual_world,std::uintptr_t actual_item,std::string&);
bool source_campaign_reward_drop_v108(const std::shared_ptr<void>& actual_world,std::uintptr_t character,std::int32_t table,std::string&);
bool source_campaign_container_drop_v104(const std::shared_ptr<void>& actual_world,std::uintptr_t container,std::int32_t table,std::uintptr_t opener,std::int32_t fixed_powers,bool source_flag,std::string&);
bool borrow_source_campaign_item_owner_v107(const std::shared_ptr<void>& actual_world,std::uintptr_t actual_item,std::shared_ptr<void>& receiver_lease,const std::uintptr_t*& owner3bc,std::string&);
bool source_campaign_item_interactive_v107(const std::shared_ptr<void>& actual_world,std::uintptr_t actual_item,std::uintptr_t character,bool&,std::string&);
bool source_campaign_item_interaction_type_v107(const std::shared_ptr<void>& actual_world,std::uintptr_t actual_item,std::uintptr_t character,std::int32_t&,std::string&);
bool capture_source_campaign_item_draws_v104(const std::shared_ptr<void>& actual_world,std::string&);
bool capture_source_campaign_item_visuals_v104(const std::shared_ptr<void>& actual_world,
 std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>>&,std::string&);
bool release_source_campaign_items_v88(const SourceCampaignCandidateBorrowV55&,std::string&);
bool bind_native_item_presentation_v88(const std::shared_ptr<void>&,SourceCampaignItemLeavesV88&,std::string&);
bool borrow_native_item_localization_v88(const std::shared_ptr<void>& actual_world,
 dh2::ui::HudTextV1*&,dh2::ui::HudTextEnvironmentV1&,std::shared_ptr<void>&,std::string&);
bool prepare_source_campaign_item_resources_v88(const SourceCampaignCandidateBorrowV55&,std::string&);
bool borrow_source_campaign_item_resources_v88(const std::shared_ptr<void>& actual_world,
 std::shared_ptr<dh2::character::SourceItemResourcesV88>&,std::string&);
bool borrow_source_campaign_inventory_creation_v114(const std::shared_ptr<void>& actual_world,
 dh2::character::WorldItemLootCreationServicesV10&,std::shared_ptr<void>& provider,std::string&);
}
