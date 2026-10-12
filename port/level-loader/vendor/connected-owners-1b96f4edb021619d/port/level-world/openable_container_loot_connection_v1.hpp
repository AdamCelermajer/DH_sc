#pragma once
#include "character_loot_drop_v8.hpp"
#include "character_loot_drop_award_v8.hpp"
namespace dh2::world {
struct OpenableContainerLootServicesV1 {
    void* context{};
    // Actual canonical ObjectManager GetHandle/GetObject, then source type0
    // filter. Non-character chest returns empty character borrow successfully.
    bool(*character)(void*,std::uintptr_t,character::skills::WorldLootActorBorrowV8&,std::string&){};
    bool(*is_player)(void*,std::uintptr_t,bool&,std::string&){};
    data::LootCreationServicesV8 creation;
    void* notifications_context{};
    bool(*notifications)(void*,data::LootTemporaryInventoryV8&,data::ItemInstanceV1&,std::string&){};
};
// Shares original LootCreation/RNG and retained ItemManager/145pool drop owner.
// Only source temporary NULL-character inventory is allocated here.
class OpenableContainerLootConnectionV1 {
    data::LootTablesV2::Borrow tables_;
    data::LootCreationV8& creation_;
    character::CharacterLootDropAwardV8& award_;
    OpenableContainerLootServicesV1 services_;
    std::unique_ptr<data::LootTemporaryInventoryV8> pending_;
    bool running_{};
    static bool query(void*,const data::LootCreationQueryV8&,data::LootCreationResponseV8&,std::string&);
    static bool create(void*,std::int32_t,std::unique_ptr<data::ItemInstanceV1>&,std::string&);
    static bool store(void*,std::unique_ptr<data::ItemInstanceV1>&,std::string&);
    bool character(std::uintptr_t,character::skills::WorldLootActorBorrowV8&,std::string&);
public:
    OpenableContainerLootConnectionV1(data::LootTablesV2::Borrow tables,data::LootCreationV8& creation,
      character::CharacterLootDropAwardV8& award,OpenableContainerLootServicesV1 services)
      :tables_(std::move(tables)),creation_(creation),award_(award),services_(services){}
    bool drop_table(std::int32_t table,std::uintptr_t chest,std::uintptr_t opener,
      std::int32_t fixed_powers,bool source_flag,std::string&);
    const data::LootTemporaryInventoryV8* pending_inventory()const{return pending_.get();}
};
}
