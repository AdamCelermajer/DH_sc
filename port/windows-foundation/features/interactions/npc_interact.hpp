#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh::foundation::interactions {
struct NpcInteractBorrow {
    std::shared_ptr<void> receiver;
    std::uintptr_t identity{};
    const std::int32_t* room64{};
    const std::int16_t* data13c8{};
    const std::uint8_t* talk_flag2fa{};
    const std::string* display_name44{};
};
struct NpcTalkEvent {
    std::int32_t objective_type{},room{},source_index=-1,data_id{};
    std::uintptr_t actor{};
    bool byte18{},byte19{};
};
struct MerchantRowBorrow {
    std::shared_ptr<const void> table;
    // Each entry is the original MerchantTable row's loot word+8. Existing
    // immutable declaration producer owns the vector, not this coordinator.
    const std::vector<std::int32_t>* loot_ids{};
};
struct NpcAsValue {
    enum class Kind { number,string } kind=Kind::number;
    double number{};
    std::string string;
};
struct NpcInteractServices {
    std::shared_ptr<void> world;
    std::function<bool(std::uintptr_t,bool&,std::string&)> is_interacting;
    std::function<bool(std::uintptr_t&,std::string&)> current_level;
    std::function<bool(std::string&)> assert_missing_level;
    std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
    // Must construct/deliver actual event through SAME EventManager RaiseAsync.
    std::function<bool(std::uintptr_t,const NpcTalkEvent&,std::string&)> raise_async;
    // Complete SAME SM_SetInteractState; source acceptance return is ignored.
    std::function<bool(std::uintptr_t,std::int32_t,bool,std::uintptr_t,bool,std::string&)> set_interact_state;
    std::function<bool(std::uintptr_t,std::int32_t,std::uintptr_t,std::string&)> raise_character_event;
    std::function<bool(std::uintptr_t,bool&,std::string&)> is_merchant,has_loot,is_cleaner;
    std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> handle_as_character;
    std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> get_loot,num_items;
    std::function<bool(std::int32_t,MerchantRowBorrow&,std::string&)> merchant_table;
    // SAME NPC ItemInventory::AddLoot(id,0,0,-1,false), including RNG/effects.
    std::function<bool(std::uintptr_t,std::int32_t,std::string&)> add_merchant_loot;
    std::function<bool(std::uintptr_t&,std::shared_ptr<void>&,std::string&)> hud_root,merchant_root;
    std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> player_info_id;
    // Actual RenderFX InvokeASCallback; false source method return is ignored,
    // but this callback must fail when the bridge/receiver service is absent.
    std::function<bool(std::uintptr_t,const char*,const char*,const std::vector<NpcAsValue>&,std::string&)> invoke_as;
};
// Whole Character.Interact3a4d78 coordinator. Borrows FSM, inventory, Level and
// UI; each reached source operation is mandatory. Failure preserves prefixes.
bool npc_interact(const NpcInteractBorrow&,std::uintptr_t actor,const NpcInteractServices&,std::string&);
}
