#pragma once
#include "../engine-ui/character_menu_faery_actions_v1.hpp"
#include "../engine-ui/character_menu_queries_owner_v1.hpp"
#include "character_player_skills_v6.hpp"
namespace dh2::character {
// Sole missing CharAI+58 / Character+420 field, created at the SAME CharAI
// construction boundary (3cec78 storesNULL). Existing actors must adopt their
// actual field rather than receive a second association/default on restore.
struct PlayerFaeryAssociationV8 {
 std::uintptr_t character{},faery420{};
 explicit PlayerFaeryAssociationV8(std::uintptr_t actual_character):character(actual_character){}
};
struct CharacterMenuFaeryServicesV8 {
 std::shared_ptr<void> owner;
 std::function<bool(std::int32_t&,std::string&)> difficulty;
 // Fresh SAME CharAI+58, including genuine ctorNULL; provider must pin it.
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> faery_character;
 std::function<bool(std::uintptr_t,const char*&,std::string&)> model_name;
 std::function<bool(std::uintptr_t,const char*,const char*,bool,std::string&)> set_visual;
 std::function<bool(std::uintptr_t,std::string&)> add_animation_set;
 // Actual Application.GetCurrentLevel31f594. NULL is an original early exit.
 std::function<bool(std::uintptr_t&,std::string&)> current_level;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> place_faery_followers;
};
// Supplemental query owner for ONE dispatch. Pins prior query providers and
// world through ChangeFaery; it never constructs Save/skills/FSM/spell vectors.
bool bind_character_menu_faery_v8(ui::CharacterMenuActionsOwnerV1&,
 ui::CharacterMenuQueriesGraphV1&,skills::CharacterPlayerSkillsV6&,
 CharacterMenuFaeryServicesV8,std::string&);
}
