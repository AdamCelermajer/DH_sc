#pragma once
#include "world_item_live_owner_v5.hpp"
#include "world_item_pickup_inventory_v10.hpp"
#include "loot_pickup_quest_tail_v10.hpp"
#include "item_color_lookup_v2.hpp"
#include "player_manager_owner_v1.hpp"
#include "../engine-ui/owned_hud_settings_v1.hpp"
namespace dh2::character {
struct LootPickupActorV23 {
 std::uintptr_t identity{};std::shared_ptr<void> receiver_lease;
 data::PropertyView* properties{};std::uintptr_t* object_of_interest14a4{};
 player::PlayerEquipmentRenderOwnerV1* gear{};
 bool source_is_player{}; // actual virtual28 result from the canonical class
};
struct WorldLootPickupServicesV23 {
 std::shared_ptr<void> provider_lease;
 void* context{};
 bool(*actor)(void*,std::uintptr_t,LootPickupActorV23&,std::string&){};
 // Optional virtual source handle cast: nullable non-Character must remain NULL.
 bool(*character_cast)(void*,std::uintptr_t,std::uintptr_t&,std::string&){};
 ui::OwnedHudSettingsV1* settings{};
 // SAME Application/UI StatusMsg4 singleton (also used by level-up), never
 // allocate a second queue for loot. Metadata18 is source message data.
 bool(*enqueue_status)(void*,const char*,std::uint32_t,std::string&){};
 ItemColorLookupServicesV2 colors;
 LootPickupQuestServicesV10 quests;
 sound::VoxPlay3DOwnerV2* vox{};std::uintptr_t vox_identity{};
 // Full remaining text/tutorial/transmute/trophy/net/positive glow/loot FX
 // services. Returning false preserves the actual reached transfer prefix.
 bool(*remaining)(void*,const LootInteractRequestV8&,LootInteractResponseV8&,std::string&){};
};
class WorldLootPickupV23 {
 player::PlayerManagerOwnerV1& players_;WorldLootPickupServicesV23 services_;
 WorldItemLiveOwnerV5* items_{};
 WorldItemPickupInventoryV10 inventory_;
 bool actor(std::uintptr_t,LootPickupActorV23&,std::string&);
 static bool gear(void*,std::uintptr_t,player::PlayerEquipmentRenderOwnerV1*&,std::string&);
 static bool after(void*,std::uintptr_t,std::int32_t,std::string&);
 static bool quest_player(void*,std::uintptr_t,bool&,std::string&);
 static bool gathering(void*,std::uintptr_t,std::int32_t,bool&,std::string&);
public:
 WorldLootPickupV23(player::PlayerManagerOwnerV1& p,WorldLootPickupServicesV23 s)
  :players_(p),services_(std::move(s)),inventory_({this,gear,after}){}
 bool bind_items(WorldItemLiveOwnerV5&,std::string&);
 bool route(const LootInteractRequestV8&,LootInteractResponseV8&,std::string&);
};
}
