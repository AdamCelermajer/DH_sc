#pragma once
#include "character_menu_actions_owner_v1.hpp"
namespace dh2::ui {
struct CharacterMenuSaveActionsGraphV1 {
 std::shared_ptr<void> owner;CharacterMenuActionsOwnerV1* actions{};
 // Complete genuine Character::SG_Save against the SAME saved Character.
 std::function<bool(data::PlayerSavegameV1&,const CharacterMenuSkillAuthorityV1&,std::uintptr_t,data::PropertyView&,std::string&)> save;
 std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_player;
 std::function<bool(std::uintptr_t,const char*,std::string&)> achievement;
};
class CharacterMenuSaveActionsV1 {
 CharacterMenuSaveActionsGraphV1 graph_;bool running_{};
 bool award(std::uintptr_t,const char*,std::string&);
public:
 explicit CharacterMenuSaveActionsV1(CharacterMenuSaveActionsGraphV1 graph):graph_(std::move(graph)){}
 const CharacterMenuSaveActionsGraphV1& bindings()const noexcept{return graph_;}
 bool save(std::string&);
};
}
