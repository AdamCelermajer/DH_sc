#pragma once
#include "renderer_loot_gpu_v27.hpp"
#include "renderer_native_gslevel_v27.hpp"
#include "application_services_owner_v5.hpp"
#include <android/asset_manager.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include "enemy_status_hud_v1.hpp"
#include "character_combat_text_v1.hpp"
#include "swf_menu_device_v1.hpp"
#include "character_menu_font_palette_v1.hpp"
namespace dh2::android_ui {struct CharacterPanelMovieRuntimeV3;struct CharacterPanelGameplayServicesV2;}
namespace dh2::ui {class HudTextV1;class AuthoredCharacterApplicationV1;class MenuStatusMessagesV26;}
namespace model_renderer {
// Sole retained native Application service receiver; required before camera
// Zoom/Overview attach to Application14. Level has a distinct event base.
bool borrow_actual_application_services_v5(std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
// Actual GL-thread Android surface dimensions. A camera must not use the
// pre-resize sentinel as a source viewport.
bool borrow_actual_camera_viewport_v20(std::int32_t&,std::int32_t&,std::string&);
bool borrow_actual_camera_lg_device_v20(std::uint8_t&,std::string&);
// GL-thread borrow from the sole front session's actual Android JNI inputs.
bool borrow_actual_menu_device_v1(dh2::ui::MenuDeviceFactsV1&,std::string&);
bool borrow_actual_font_palette_v4(const dh2::ui::CharacterMenuFontPaletteV1*&,std::string&);
bool borrow_actual_status_messages_v26(std::shared_ptr<dh2::ui::MenuStatusMessagesV26>&,std::string&);
bool borrow_character_menu_application_v4(dh2::ui::HudTextV1&,std::shared_ptr<void>,std::shared_ptr<dh2::ui::AuthoredCharacterApplicationV1>&,std::string&);
bool draw_character_inventory_preview_v4(int,int,std::string&);
bool character_menu_player_v4(std::int32_t,bool remote_or_include,bool local,std::uintptr_t&,std::string&);
std::string load_menu_background(AAssetManager*);
void draw_menu_background(int width,int height);
bool select_menu_persona(int class_index,AAssetManager*,std::string&);
std::string load_class_scene(AAssetManager*);
bool select_class_scene(int class_index,int dt_ms,std::string&);
bool class_scene_active();
bool class_scene_input_enabled();
void draw_class_scene(int width,int height);
void reset_context();
void deactivate();
bool active();
std::string load(const std::uint8_t*,std::size_t,AAssetManager*);
std::string load_world(const std::uint8_t*,std::size_t,AAssetManager*,const std::string& files_directory,const std::string& selected_mlx_uri);
void move_axis(float x,float y);
void authored_hud_heading(float x,float y,bool active);
bool authored_hud_command(const float* direction,bool stop,std::string& error);
// Raw viewport pixels; HUD owns pointer gestures and calls only world-owned
// press/release phases. Selection mutates the existing player target binding.
bool world_touch(float x,float y,bool released,std::string& error);
// Modern input ownership cancellation; no target mutation or command replay.
void world_touch_cancel();
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
    bool(*format_integer)(void*,const char*,std::int32_t,std::string&,std::string&){};
};
struct CombatTextFrameV1 {std::uint32_t application_dt{};std::int32_t level_load_phase{};bool debug_disabled{},tick{};};
void connect_combat_text(CombatTextSinkV1);
bool combat_text_project(const float[3],std::int32_t*,std::int32_t*,std::string&);
bool combat_text_frame(CombatTextFrameV1&,std::string&);
void orbit(float dx,float dy,float zoom);
void set_time(int milliseconds);
void set_enemy_ai(bool);
// Modern single-player character overlay pauses simulation without changing
// the debug inspection cursor or creating a second player snapshot authority.
void set_character_panel_open(bool);
// Synchronous GL-thread binding to the sole live player/Save/Gear/VM graph.
bool connect_character_panel_runtime(const dh2::android_ui::CharacterPanelMovieRuntimeV3&,
 dh2::android_ui::CharacterPanelGameplayServicesV2&,std::string&);
void draw(int width,int height);
}
