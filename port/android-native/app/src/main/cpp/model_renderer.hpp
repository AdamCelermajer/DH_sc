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
std::string load_world(const std::uint8_t*,std::size_t,AAssetManager*);
void move_axis(float x,float y);
void focus_object(int index);
std::string set_object_state(int index,const std::string& state);
std::string set_combat_target(int index,int target);
std::string player_attack(int target=-1);
std::array<int,6> player_vitals();
void orbit(float dx,float dy,float zoom);
void set_time(int milliseconds);
void set_enemy_ai(bool);
void draw(int width,int height);
}
