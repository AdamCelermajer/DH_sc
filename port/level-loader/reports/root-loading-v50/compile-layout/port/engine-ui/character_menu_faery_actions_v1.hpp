#pragma once
#include "character_menu_actions_owner_v1.hpp"
namespace dh2::ui {
bool character_menu_store_current_faery_v1(data::PlayerSavegameV1&,std::uintptr_t character,std::uint32_t faery,std::int32_t difficulty,std::string&);
struct CharacterMenuFaeryActionsGraphV1 {
 std::shared_ptr<void> owner;CharacterMenuActionsOwnerV1* actions{};
 std::function<bool(std::int32_t&,std::string&)> difficulty;
 // Fresh actual Character+420 identity; genuine NULL is permitted.
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> faery_character;
 // Complete reached GetPosition -> SetMoveTarget(false,true) -> effect.
 std::function<bool(std::uintptr_t,std::string&)> retarget_effect;
 std::function<bool(std::uintptr_t&,std::string&)> current_hud_player;
 std::function<bool(std::uintptr_t,bool,std::string&)> set_faery_interface;
};
class CharacterMenuFaeryActionsV1 {
 CharacterMenuFaeryActionsGraphV1 graph_;bool running_{};
public:
 explicit CharacterMenuFaeryActionsV1(CharacterMenuFaeryActionsGraphV1 graph):graph_(std::move(graph)){}
 const CharacterMenuFaeryActionsGraphV1& bindings()const noexcept{return graph_;}
 bool set_active(std::uint32_t,std::string&);
};
}
