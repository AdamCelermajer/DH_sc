#pragma once
#include "area_transition_request_v114.hpp"
#include <menu_stack_v1.hpp>
namespace dh2::ui {
struct MenuFSCommandServicesV114 {
 std::shared_ptr<void> owner;
 std::function<bool(std::string&)> current;
 std::function<bool(bool manager,const char*,const char*,std::string&)> source_trace;
 // Captures an EXISTING roster receiver; not an additional menu directory.
 std::function<bool(std::uintptr_t,std::shared_ptr<void>&,std::string&)> capture_menu;
 std::function<bool(std::uintptr_t,const char*,const char*,std::string&)> virtual30;
 std::function<bool(std::uintptr_t,const char*,const char*,bool&,std::string&)> virtual38;
 std::function<bool(MenuStackRenderV1&,std::string&)> pop_all_render;
 std::function<bool(const loader::ApplicationLoadLevelArgumentsV114&,std::string&)> application_load_level;
 std::function<bool(std::shared_ptr<void>&,const data::LevelTables*&,std::string&)> level_tables;
 std::function<bool(std::uintptr_t,const char*,const char*,bool&,std::string&)> remaining_my_fs;
};
// RenderFX.fc's actual polymorphic handler. NULL is the original no-call path.
bool render_fs_command_v114(std::uintptr_t handler,
 const std::function<bool(std::uintptr_t,const char*,const char*,std::string&)>& virtual4,
 const char*,const char*,std::string&);
bool menu_fx_fs_command_v114(MenuStackRenderV1&,const char*,const char*,MenuFSCommandServicesV114&,std::string&);
bool menu_manager_fs_command_v114(MenuStackV1&,const char*,const char*,MenuFSCommandServicesV114&,std::string&);
bool menu_base_on_fs_command_v114(std::uintptr_t,const char*,const char*,MenuFSCommandServicesV114&,std::string&);
bool menu_base_my_fs_command_v114(MenuStackMenuV1&,const char*,const char*,MenuFSCommandServicesV114&,bool& handled,std::string&);
}
