#pragma once
#include "character_loot_interact_v8.hpp"
#include "player_equipment_render_owner_v1.hpp"
namespace dh2::character {
struct WorldItemPickupInventoryServicesV10 {
 void* context{};
 // Fresh borrow of SAME live player Gear; never constructs an inventory.
 bool(*player)(void*,std::uintptr_t,player::PlayerEquipmentRenderOwnerV1*&,std::string&){};
 // Whole source destination quest set/current GS/constant/AsyncCall tail.
 // It must execute the source empty-set test too; not an accepted no-op.
 bool(*after_add_all)(void*,std::uintptr_t,std::int32_t,std::string&){};
};
class WorldItemPickupInventoryV10 {
 WorldItemPickupInventoryServicesV10 services_;
 struct Transfer {WorldItemPickupInventoryV10* self;player::PlayerEquipmentRenderOwnerV1* gear;std::uintptr_t character;};
 static bool add(void*,std::unique_ptr<data::ItemInstanceV1>&,bool,bool,std::int32_t&,std::string&);
 static bool after(void*,std::int32_t,std::string&);
 static bool gold(void*,std::int32_t,std::string&);
public:
 explicit WorldItemPickupInventoryV10(WorldItemPickupInventoryServicesV10 s):services_(s){}
 // Unhandled operations MUST continue to actual outer world service.
 bool route(const LootInteractRequestV8&,LootInteractResponseV8&,bool& handled,std::string&);
};
}
