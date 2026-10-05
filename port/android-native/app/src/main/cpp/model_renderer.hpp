#pragma once
#include <android/asset_manager.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include "enemy_status_hud_v1.hpp"
#include "character_combat_text_v1.hpp"
namespace model_renderer {
void reset_context();
void deactivate();
bool active();
std::string load(const std::uint8_t*,std::size_t,AAssetManager*);
std::string load_world(const std::uint8_t*,std::size_t,AAssetManager*,const std::string& files_directory);
void move_axis(float x,float y);
void focus_object(int index);
std::string set_object_state(int index,const std::string& state);
std::string set_combat_target(int index,int target);
std::string player_attack(int target=-1);
std::array<int,6> player_vitals();
// GL-thread action bridge used by the forthcoming character menu. Operations
// 0 auto-equip, 1 equip to slot, 2 unequip slot, 3 swap weapon set.
std::string player_equipment_action(int operation,int index,int slot);
// Synchronous GL-thread borrow of the same resolved sheet used by combat.
// Loading another world revokes this view; UI must not cache the data pointer.
struct PlayerHudView {
    const std::int32_t* resolved{};
    std::size_t count{};
    std::uintptr_t character{};
    bool dead{};
};
PlayerHudView player_hud_view();
std::vector<int> player_gameplay_hud();
std::vector<std::string> player_gameplay_hud_icon_names();
std::string player_gameplay_action(int operation,int index);
dh2::ui::EnemyHudWorldBorrowV1 enemy_hud_world_borrow(std::string& error);
// GL-thread borrowed UI sink. Requests copy event values before the current
// camera is submitted; the retained UI owner projects them after world draw.
struct CombatTextSinkV1 {
    void* context{};
    int(*localized)(void*,std::int32_t,const char**){};
    int(*enqueue)(void*,const dh2::character::skills::CombatTextRequestV1*){};
};
struct CombatTextFrameV1 {std::uint32_t application_dt{};std::int32_t level_load_phase{};bool debug_disabled{},tick{};};
void connect_combat_text(CombatTextSinkV1);
bool combat_text_project(const float[3],std::int32_t*,std::int32_t*,std::string&);
bool combat_text_frame(CombatTextFrameV1&,std::string&);
void orbit(float dx,float dy,float zoom);
void set_time(int milliseconds);
void set_enemy_ai(bool);
void draw(int width,int height);
}
