#pragma once
#include "menu_manager_unload_v58.hpp"
namespace dh2::ui {
struct MenuLevelBorrowV58 {
 std::shared_ptr<void> actual_owner;
 std::uintptr_t identity{};
 const std::uint8_t* byte198{};
};
struct MenuManagerUpdateServicesV58 {
 std::shared_ptr<void> actual_manager;
 // Required source prefix42ea04..42ecac: tracing, actual local-player/OOI
 // action icon, wireframe/debug early exits, Application dt and listener88
 // cleanup. A true early_exit skips every movie/HUD/Flash/menu update below.
 // This is a provider boundary, never a successful empty replacement.
 std::function<bool(bool& early_exit,std::int32_t& dt,std::string&)> source_prefix;
 std::function<bool(std::uint32_t,MenuMovieBorrowV58&,std::string&)> movie_slot;
 std::function<bool(MenuLevelBorrowV58&,std::string&)> current_level;
 std::function<bool(const MenuMovieBorrowV58&,std::int32_t,bool,std::string&)> movie_virtual10;
 std::function<bool(bool&,std::string&)> currently_in_game_view;
 std::function<bool(std::string&)> info_hud_update,hud_controls_update,flash_anim_update;
 std::function<bool(std::int32_t&,std::string&)> get_num_menus;
 std::function<bool(std::uint32_t,std::uintptr_t&,std::string&)> registry_at;
 std::function<bool(std::uintptr_t,bool&,std::string&)> menu_is_visible;
 std::function<bool(std::uintptr_t,std::string&)> menu_virtual1c;
};
// Whole42ea04 call ordering, with its original prefix supplied by the actual
// Application/Debug/PM owner. bool true selects HUD3; false selects all0..3.
class MenuManagerUpdateV58 {
 MenuManagerUpdateServicesV58 services_;
 bool busy_{};
public:
 explicit MenuManagerUpdateV58(MenuManagerUpdateServicesV58);
 bool update(bool hud_only,std::string&);
};
}
