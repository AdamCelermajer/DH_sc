#pragma once
#include <cstdint>
#include <string>
#include <memory>
namespace model_renderer {
bool source_main_menu_refresh_hud_v114(const std::shared_ptr<void>& actual_world,std::string&);
// Accept one original Application.GoToMainMenu event. The owning native frame
// drains it after authored dispatch unwinds; acceptance is not completion.
bool request_source_main_menu_v114(std::int32_t original_event,std::string&);
bool source_main_menu_transition_pending_v114()noexcept;
// Read-only admission guard for the existing area-transition producer.
bool source_main_menu_transition_owned_v114()noexcept;
// Explicit retry of the same latched command and existing retirement journal.
bool resume_source_main_menu_v114(std::string&);
}
