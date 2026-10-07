#pragma once
#include "world_loot_canonical_bindings_v44.hpp"
#include "character_state_owner.hpp"
#include "player_save_difficulty_global_v29.hpp"
#include "../engine-ui/character_menu_item_actions_v1.hpp"
#include "../engine-ui/hud_text_v1.hpp"
namespace dh2::character {
// Missing fields only. Existing Save/property/PM/profile inventories stay owned
// by their current receiver. Source byte80 has no constructor store.
class LootPlayerFieldAssociationV47 {
 std::uintptr_t character_{},save14e8_{};
 std::shared_ptr<data::PlayerSavegameV1> save_;
 std::uint8_t visible80_{};bool visible_produced_{};
public:
 explicit LootPlayerFieldAssociationV47(std::uintptr_t);
 bool source_save_store_3b36d8(std::shared_ptr<data::PlayerSavegameV1>,std::string&);
 bool borrow_save(std::shared_ptr<void> actual_character_lease,LootCharacterSaveBorrowV44&,std::string&)const;
 const std::uintptr_t* save_slot14e8()const noexcept{return &save14e8_;}
 // Called only at actual PropertyMap or SetVisible38b0f0 source byte store.
 void source_visible_store(std::uint8_t value)noexcept{visible80_=value;visible_produced_=true;}
 const std::uint8_t* visible80()const noexcept{return visible_produced_?&visible80_:nullptr;}
 std::uintptr_t character()const noexcept{return character_;}
};
bool loot_state_info_borrow_v47(CharacterStateOwner&,std::uintptr_t,const std::int32_t*&,std::string&);
// Same named Character receiver and actual GetCharAI source lookup. Pure whole
// IsPlayer3a49f0: GetCharType !=0 -> type==1; type0 -> NAME30 prefix, not gameType48.
bool loot_character_is_player_v47(const data::AiTables&,const data::PropertyView&,
 const std::string& actual_name30,bool&,std::string&);
struct LootPickupLeavesServicesV47 {
 std::shared_ptr<void> application_lease;
 void* context{};
 bool(*actor)(void*,std::uintptr_t,LootPickupActorV23&,std::string&){};
 bool(*named_is_player)(void*,std::uintptr_t,bool&,std::string&){};
 bool(*is_local_player)(void*,std::uintptr_t,bool&,std::string&){};
 bool(*online)(void*,bool&,std::string&){};
 bool(*save_fields)(void*,std::uintptr_t,LootCharacterSaveBorrowV44&,const std::uintptr_t*&,std::string&){};
 bool(*achievement)(void*,std::uintptr_t,const char*,std::string&){};
 bool(*bind_item_actions)(void*,std::uintptr_t,ui::CharacterMenuItemActionsGraphV1&,std::string&){};
 // Original singleton/cache/environment, supplied by existing UI/renderer.
 ui::HudTextV1* text{};ui::HudTextEnvironmentV1 text_environment;
 data::ItemTextServicesV5 item_text; // SAME existing ItemTextOwnerV5 formatter
 const dh2_script_design_bindings* design{};
 ui::OwnedHudSettingsV1* settings{};
 std::shared_ptr<player::PlayerSaveDifficultyGlobalV29> difficulty_global;
 // Tutorials/net/positive tooltip/glow/FX remain exact actual receivers.
 bool(*remaining)(void*,const LootInteractRequestV8&,LootInteractResponseV8&,std::string&){};
};
// No pool, inventory, profile, text cache or FX manager duplication. Transfers
// remain V23/V10 source bodies; these close actual previously missing leaves.
class LootPickupSourceLeavesV47 {
 LootPickupLeavesServicesV47 services_;
 bool actor(std::uintptr_t,LootPickupActorV23&,std::string&);
 bool constant(const char*,const char*,std::int32_t&,std::string&);
public:
 explicit LootPickupSourceLeavesV47(LootPickupLeavesServicesV47);
 bool route(const LootInteractRequestV8&,LootInteractResponseV8&,bool& handled,std::string&);
};
}
