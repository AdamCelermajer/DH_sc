#pragma once
#include "character_player_skills_v6.hpp"
#include "player_equipment_render_owner_v1.hpp"
#include "../engine-ui/character_menu_queries_owner_v1.hpp"
#include "../engine-ui/character_menu_item_actions_v1.hpp"
#include "../engine-ui/character_menu_save_actions_v1.hpp"
#include "../game-data/player_save_write_owner_v1.hpp"
namespace dh2::character {
struct CharacterMenuMutationServicesV4 {
 std::shared_ptr<void> owner;
 std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_local_player,is_player;
 std::function<bool(bool&,std::string&)> online;
 std::function<bool(std::uintptr_t,const char*,std::string&)> achievement;
 std::function<bool(std::uintptr_t,const data::ItemInstanceV1&,std::string&)> drop_packet;
 std::function<bool(data::FreshInventoryOwnedV4&,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::string&)> drop_world;
 data::PlayerSaveWriteServicesV1 save;
};
// Full reached positive SetGold tail; actual gold store belongs to inventory.
// Rereads SAME gold after every original achievement callback.
bool character_menu_gold_notifications_v4(data::FreshInventoryOwnedV4&,
 const CharacterMenuMutationServicesV4&,std::string&);
// Per-dispatch bindings retain each source action owner while the Queries
// owner calls it. No stale pointer to a stack CharacterMenuActions owner is
// retained after this dispatch. Save/profile and Gear inventory are borrowed.
bool bind_character_menu_mutations_v4(ui::CharacterMenuActionsOwnerV1&,
 ui::CharacterMenuQueriesGraphV1&,skills::CharacterPlayerSkillsV6&,
 std::shared_ptr<data::PlayerSaveLoadOwnerV1>,
 CharacterMenuMutationServicesV4,std::string&);
}
