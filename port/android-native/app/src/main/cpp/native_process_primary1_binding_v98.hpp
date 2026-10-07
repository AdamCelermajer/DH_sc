#pragma once
#include "renderer_native_menu_load_v98.hpp"
namespace model_renderer {
bool enroll_native_process_primary1_v98(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,std::string&);
// Enroll once after the SAME process PostMovie directory has been published.
// Layout pointers must be the process source cells, never a SWF/view rectangle.
bool enroll_native_process_primary1_v98(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 std::shared_ptr<void> actual_layout_owner,const std::int32_t* width_screen,
 const std::int32_t* height_screen,std::string&);
}
