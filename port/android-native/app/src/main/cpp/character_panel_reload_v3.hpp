#pragma once
#include "character_panel_session_v1.hpp"
namespace dh2::android_ui {
struct CharacterPanelReloadServicesV3 {
 std::shared_ptr<void> owner;
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> player;
 std::function<bool(double,std::int32_t&,std::string&)> player_index;
 // Exact RemoveBuffs/Load0x20/InitSkills/RecalculateStats/CheckItems and
 // current MenuFX/IsSpecTime. Same actors/Save/VM are supplied each call.
 std::function<bool(const model_renderer::PlayerGameplayBinding&,
                    const ui::MenuReloadRequest32V1&,
                    ui::MenuReloadResponse16V1&,std::string&)> invoke;
};
// Builds a transient source reload graph for ONE current dispatch. Its lease
// pins borrowed services; no mutable profile/Save/skill state is cloned.
bool character_panel_reload_binding_v3(const model_renderer::PlayerGameplayBinding&,
 const CharacterPanelReloadServicesV3&,ui::CharacterMenuReloadActionGraphV1&,std::string&);
}
