#pragma once
#include "character_loot_item_manager_v8.hpp"
namespace dh2::character {
struct LootItemPickupBorrowV8 {
 data::ItemInstanceV1* item{};
 const data::Item* metadata{};
 const std::int16_t* pickup_override58{}; // genuine C1 stores -1; not inferred
};
struct LootDropAwardServicesV8 {
 void* context{};
 bool(*position)(void*,std::uintptr_t,const float*&,std::string&){};
 bool(*random_drop_position)(void*,std::uintptr_t source,std::uintptr_t killer,float[3],std::string&){};
 // Source PlayerManager.GetLocalPlayer(0,true), then its actual +660 Character.
 bool(*local_player_character)(void*,std::int32_t,bool,std::uintptr_t&,std::string&){};
 bool(*item)(void*,std::uintptr_t,LootItemPickupBorrowV8&,std::string&){};
 bool(*constant)(void*,const char* group,const char* key,std::int32_t&,std::string&){};
 bool(*interact)(void*,std::uintptr_t item_object,std::uintptr_t killer,std::string&){};
};
class CharacterLootDropAwardV8 {
 CharacterLootItemManagerV8& manager_;LootDropAwardServicesV8 services_;bool running_{};
public:
 CharacterLootDropAwardV8(CharacterLootItemManagerV8& m,LootDropAwardServicesV8 s):manager_(m),services_(s){}
 // Whole source DropAndAwardLoot3ec8a0 + _DoAutoPickupHack3ec474 order.
 // Every surviving item is spawned into the same real pool. No generated
 // scatter, player record, pickup radius or successful empty receiver exists.
 bool drop(data::LootTemporaryInventoryV8&,std::uintptr_t source,
           std::uintptr_t killer,std::string&);
};
}
