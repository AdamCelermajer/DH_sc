#pragma once
#include "hud_text_v1.hpp"
namespace dh2::ui {
// Complete reached integer ^d formatting via original508ef4 over the SAME
// StringManager cache/environment. Localized format is supplied by that owner,
// not rebuilt with to_string or an English XP label.
bool progression_xp_format_v23(HudTextV1&,const HudTextEnvironmentV1&,
 const char* actual_localized_format,std::int32_t amount,std::string&,std::string&);
}
