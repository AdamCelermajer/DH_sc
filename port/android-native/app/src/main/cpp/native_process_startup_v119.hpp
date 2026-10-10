#pragma once
#include "application_services_owner_v5.hpp"
#include "event_manager_owner_v12.hpp"
namespace model_renderer {
bool native_process_menu_base_hide_v119(std::uintptr_t,std::string&);
bool native_process_derived_menu_show_v119(std::uintptr_t,std::uintptr_t,std::string&);
bool native_process_derived_menu_focus_v119(std::uintptr_t,std::uintptr_t,std::string&);
bool native_process_derived_menu_blur_v119(std::uintptr_t,std::uintptr_t,std::string&);
bool native_selected_menu_hide_v119(std::uintptr_t,std::string&);
bool native_process_derived_menu_callback_v119(std::uintptr_t,std::uintptr_t,const char*,std::string&);
bool native_process_derived_menu_animation_v119(std::uintptr_t,std::uintptr_t,std::uintptr_t,const char*,bool&,std::string&);
}
