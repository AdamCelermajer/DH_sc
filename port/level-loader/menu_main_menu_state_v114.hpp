#pragma once
#include "application_go_to_main_menu_v114.hpp"
#include "authored_menu_application_fields_v3.hpp"
#include "menu_stack_owner_v1.hpp"
#include "menu_fx_pop_all_v114.hpp"
#include <array>
namespace dh2::ui {
// SAME GSFlashMenu allocation, retained by FrontUiSession. Source C1 3841b4:
// native-width projections of 4/8/c; empty inline CString10; bytes28/29 zero.
struct GSFlashMenuFieldsV114 {
 std::uintptr_t manager4{},current8{},request_c{};
 std::array<char,16> string10{};
 std::uint8_t source28{},source29{};
};
struct GSFlashMenuServicesV114 {
 std::shared_ptr<void> owner; //independent provider; containing Front/App weak
 std::function<bool(std::string&)> current;
 std::function<bool(std::string&)> debug_load;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(std::shared_ptr<void>&,std::uintptr_t&,std::string&)> menu_manager;
 // Genuine complete MenuManager.LoadMainMenu4324dc, NOT slot-presence success.
 std::function<bool(std::uintptr_t,std::string&)> load_main_menu;
 std::function<bool(std::uintptr_t,const char*,std::uintptr_t&,std::string&)> get_menu;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> push_menu;
 // MultiMenu.virtual3c receives old MenuBase.name8 and false, not a C++ delete.
 std::function<bool(std::uintptr_t,std::uintptr_t,bool,std::string&)> pop_menu;
};
class GSFlashMenuEntryV114 {
 unsigned phase_{};bool busy_{},complete_{};std::string failure_;
public:
 bool enter(GSFlashMenuFieldsV114&,const GSFlashMenuServicesV114&,std::string&);
 bool resume(GSFlashMenuFieldsV114&,const GSFlashMenuServicesV114&,std::string&);
 bool arm_resume(std::string&);
 const std::string& failure()const noexcept{return failure_;}
};
// Original Update384460 request prefix only. Sole existing MenuManager frame
// update/timeline remains with native frame owner; this never advances a movie.
bool gs_flash_menu_request_v114(GSFlashMenuFieldsV114&,const GSFlashMenuServicesV114&,std::string&);
struct MenuManagerResetServicesV114 {
 std::shared_ptr<void> owner;
 std::function<bool(std::string&)> current;
 // Return true/NULL for genuine absent MultiMenu slot134[i].
 std::function<bool(unsigned,MenuStackRenderV1*&,std::shared_ptr<void>&,std::string&)> render_slot;
 std::function<bool(MenuStackRenderV1&,std::string&)> pop_all;
 std::function<bool(std::int32_t&,std::string&)> registry_count;
 std::function<bool(unsigned,MenuStackMenuV1*&,std::shared_ptr<void>&,std::string&)> registry_at;
 std::function<bool(MenuStackMenuV1&,bool&,std::string&)> is_visible;
 std::function<bool(MenuStackMenuV1&,std::string&)> hide_virtual10;
 std::function<bool(std::string&)> store_listener88;
};
// Genuine Reset42d73c..42d7d0, not listener-tail42d7c8 alone. Completion journal
// retains reached prefix on host failure; explicit resume retries failed leaf.
class MenuManagerResetV114 {
 unsigned phase_{},slot_{},index_{};std::int32_t count_{};
 bool busy_{},complete_{},resume_receipt_{};std::string failure_;
public:
 bool reset(const MenuManagerResetServicesV114&,std::string&);
 bool resume(const MenuManagerResetServicesV114&,std::string&);
 bool arm_resume(std::string&);
 const std::string& failure()const noexcept{return failure_;}
};
}
