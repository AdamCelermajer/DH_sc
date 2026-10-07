#pragma once
#include "menu_main_menu_state_v114.hpp"
#include "menu_manager_unload_v58.hpp"
namespace dh2::ui {
enum class MainMenuSingletonV114 {debug,main_menu,character_select,enter_name,lobby,leaderboard};
struct MainMenuLoadFactsV114 {
 std::shared_ptr<void> owner;
 const std::int32_t* width_screen{};
 const std::uint8_t* htc_devices{};
 const std::uint8_t* no_igp{};
};
struct MainMenuReceiverV114 {std::shared_ptr<void> owner;std::uintptr_t identity{};};
struct MenuMainLoadServicesV114 {
 std::shared_ptr<void> owner; //independent, containing process/App/Front weak
 std::function<bool(std::string&)> current;
 std::function<bool(MainMenuLoadFactsV114&,std::string&)> facts;
 std::function<bool(std::int32_t&,std::string&)> save_language;
 // Exact MultiMenu.LoadSWFFile(slot2), including Game_Text write BEFORE slot
 // lookup, retained positive slot return, actual missing RenderFX/C1/Load/camera.
 std::function<bool(const char*,MenuMovieBorrowV58&,std::string&)> load_slot2;
 std::function<bool(const MenuMovieBorrowV58&,std::uint32_t,std::string&)> set_behavior;
 std::function<bool(const MenuMovieBorrowV58&,std::int32_t,bool,std::string&)> update_render;
 std::function<bool(MainMenuSingletonV114,MainMenuReceiverV114&,std::string&)> singleton;
 // Actual weak.get_ptr clears a dead proxy before returning false.
 std::function<bool(const MainMenuReceiverV114&,bool&,std::string&)> weak_live;
 std::function<bool(MainMenuSingletonV114,const MainMenuReceiverV114&,std::string&)> initialize;
 std::function<bool(const MainMenuReceiverV114&,std::string&)> register_menu;
 // Store0 into SAME actual gameswf::character.visible9b, not MenuBase74.
 std::function<bool(const MainMenuReceiverV114&,std::string&)> hide_debug_character;
 std::function<bool(std::uint32_t&,std::string&)> registry_count;
 std::function<bool(std::string&)> post_load;
 std::function<bool(std::uint32_t,MainMenuReceiverV114&,std::string&)> registry_at;
 std::function<bool(const MainMenuReceiverV114&,std::string&)> register_drag_and_drops;
 std::function<bool(const MainMenuReceiverV114&,std::string&)> hide_virtual10;
};
class MenuMainLoadV114 {
 unsigned phase_{},singleton_index_{};
 std::string uri_,failure_;bool busy_{},complete_{},weak_live_{};
 MenuMovieBorrowV58 movie_;MainMenuReceiverV114 receiver_;
 std::weak_ptr<void> movie_storage_,receiver_storage_;
 std::uint32_t suffix_index_{};
public:
 bool load(const MenuMainLoadServicesV114&,std::string&);
 bool resume(const MenuMainLoadServicesV114&,std::string&);
 bool arm_resume(std::string&);
 bool complete()const noexcept{return complete_;}
 const std::string& failure()const noexcept{return failure_;}
};
}
