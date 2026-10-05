#pragma once
#include <android/asset_manager.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <array>
namespace model_renderer {
void reset_context();
void deactivate();
bool active();
std::string load(const std::uint8_t*,std::size_t,AAssetManager*);
// Original main-menu swamp and CreateAvatarCamera values; no orbit controls.
std::string load_menu_background(AAssetManager*);
void draw_menu_background(int width,int height);
bool class_scene_active();
bool class_scene_input_enabled();
std::string load_class_scene(AAssetManager*);
bool select_class_scene(int index,int dt_ms,std::string&);
void draw_class_scene(int width,int height);
std::string load_world(const std::uint8_t*,std::size_t,AAssetManager*,const std::string& files_directory);
void move_axis(float x,float y);
void focus_object(int index);
std::string set_object_state(int index,const std::string& state);
std::string set_combat_target(int index,int target);
std::string player_attack(int target=-1);
std::array<int,6> player_vitals();
// Synchronous GL-thread borrow of the same resolved sheet used by combat.
// Loading another world revokes this view; UI must not cache the data pointer.
struct PlayerHudView {
    const std::int32_t* resolved{};
    std::size_t count{};
    std::uintptr_t character{};
    bool dead{};
};
PlayerHudView player_hud_view();
void orbit(float dx,float dy,float zoom);
void set_time(int milliseconds);
void set_enemy_ai(bool);
void draw(int width,int height);
}
