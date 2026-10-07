#pragma once
#include "character_combat_text_v1.hpp"
#include <string>
namespace dh2::character::skills {
struct PlayerXPTextServicesV1 {
 CombatTextServicesV1 common;
 // Same StringManager::format508ef4. It must interpret the actual localized
 // format; this is not std::to_string nor a guessed numeric Flash request.
 int(*format)(void*,const char* actual_localized,std::int32_t,std::string*){};
};
// F_ApplyScrollingCombatTextXP3af0c8 after source distribution's actual
// HudManager display lookup anim_sct_xp and XPColor constant. Caller pins
// that manager/style receipt. Position is victim source +160 plus real bbox.
int player_xp_text_v1(std::uintptr_t victim,std::int32_t modified_integer,
 std::int32_t actual_xp_color,const PlayerXPTextServicesV1&);
}
