#pragma once
#include "menu_fs_command_v114.hpp"
#include "../level-world/character_design_services.hpp"
namespace dh2::ui {
inline constexpr const char* menu_manager_trace_key_v114="isTracingMenuManager";
inline constexpr const char* menu_base_trace_key_v114="isTracingMenuBase";
// Existing actual Debug owner and filesystem domain. No map/state/singleton is
// constructed here. files_owner retains the real files/context pointer.
struct MenuFSTraceServicesV114 {
 std::shared_ptr<character::DebugSwitches> debug;
 std::shared_ptr<void> files_owner;
 const character::DebugFileServices24* files{};
 // Actual Main menu/process delivery guard; usable before a World is started.
 std::function<bool(std::string&)> current;
};
// Native sequence is Load -> temporary std::string -> GetSwitch -> string D1.
// The returned switch is intentionally ignored, including true. There is no
// Print/format/set-variable/restore operation in these two original bodies.
bool menu_fs_source_trace_v114(bool manager,const char* command,const char* arguments,
 const MenuFSTraceServicesV114&,std::string&);
// Caller retains this independent process packet. Closure retains it weakly
// and preserves an already supplied genuine source_trace callback unchanged.
bool bind_menu_fs_source_trace_v114(const std::shared_ptr<const MenuFSTraceServicesV114>&,
 MenuFSCommandServicesV114&,std::string&);
}
