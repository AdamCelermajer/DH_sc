#pragma once
#include "world_item_loot_creation_v10.hpp"
#include "character_design_services.hpp"
#include "player_manager_owner_v1.hpp"
namespace dh2::character {
struct CharacterLootCreationBackendV22 {
 std::shared_ptr<void> provider_lease;
 void* context{};
 bool(*query)(void*,const data::LootCreationQueryV8&,data::LootCreationResponseV8&,std::string&){};
 bool(*assertion)(void*,const data::LootEntryRequestV8&,std::int32_t&,std::string&){};
 bool(*notifications)(void*,data::LootTemporaryInventoryV8&,data::ItemInstanceV1&,std::string&){};
};
// Reuses actual Debug singleton/files, source PM class counters and the SAME
// ItemPresentation owner. Its item identity maps are never a Gear inventory.
// Caller must deliver presentation.forget BEFORE instance destruction/merge.
class CharacterLootCreationBindingsV22 {
 player::PlayerManagerOwnerV1& players_;DebugSwitches& debug_;
 DebugFileServices24 files_;data::ItemPresentationOwnerV5& presentation_;
 data::ItemTextServicesV5 text_;CharacterLootCreationBackendV22 backend_;
 bool debug(bool,const char*,std::int32_t&,std::string&);
 static bool entry(void*,const data::LootEntryRequestV8&,std::int32_t&,std::string&);
 static bool power(void*,const data::LootPowerRequestV7&,std::int32_t&,std::string&);
 static bool query(void*,const data::LootCreationQueryV8&,data::LootCreationResponseV8&,std::string&);
 static bool notifications(void*,data::LootTemporaryInventoryV8&,data::ItemInstanceV1&,std::string&);
public:
 CharacterLootCreationBindingsV22(player::PlayerManagerOwnerV1& p,DebugSwitches& d,
  DebugFileServices24 f,data::ItemPresentationOwnerV5& presentation,data::ItemTextServicesV5 t,
  CharacterLootCreationBackendV22 b):players_(p),debug_(d),files_(f),presentation_(presentation),text_(t),backend_(std::move(b)){}
 WorldItemLootCreationServicesV10 services()noexcept;
};
}
