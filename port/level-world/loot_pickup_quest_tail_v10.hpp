#pragma once
#include <cstdint>
#include <string>
namespace dh2::character {
struct LootPickupQuestEventV10 {
 std::int32_t objective_type{};
 std::uintptr_t character{};
 std::int32_t network_id{-1},subject_id{-1},item_id{};
 std::uint8_t flag0{},flag1{};
};
struct LootPickupQuestServicesV10 {
 void* context{};
 bool(*is_player)(void*,std::uintptr_t,bool&,std::string&){};
 // Source ItemInventory+30 registered gathering list, not bag item count.
 bool(*registered_gathering_id)(void*,std::uintptr_t,std::int32_t,bool&,std::string&){};
 //Legacy name: actual Application.GetCurrentLevel31f594. Capture that SAME
 //Level through immediate RaiseAsync; never an inherited GS dispatcher.
 bool(*current_game_state)(void*,std::uintptr_t&,std::string&){};
 // Forward literal arguments r1/r2 exactly. Do not swap them by API naming.
 bool(*constant)(void*,const char* r1,const char* r2,std::int32_t&,std::string&){};
 //Immediate original Level.RaiseAsync339090 over this actual stack event.
 //No queue/copy; flags10/11=0, quantity14=-1, item18 is item_id.
 bool(*raise_async)(void*,std::uintptr_t,const LootPickupQuestEventV10&,std::string&){};
};
// Complete reached quest tail of TransferInventoryTo, original3ffafc..be4.
bool loot_pickup_quest_tail_v10(std::uintptr_t actual_inventory_character,
 std::int32_t actual_item_id,const LootPickupQuestServicesV10&,std::string&);
}
