#pragma once
#include <cstdint>
#include <string>
namespace model_renderer {
// Accept one original Application.GoToMainMenu event. The owning native frame
// drains it after authored dispatch unwinds; acceptance is not completion.
bool request_source_main_menu_v114(std::int32_t original_event,std::string&);
// Read-only admission guard for the existing area-transition producer.
bool source_main_menu_transition_owned_v114()noexcept;
// Explicit retry of the same latched command and existing retirement journal.
bool resume_source_main_menu_v114(std::string&);
}
