#pragma once
#include "player_gameplay_binding.hpp"
#include <vector>
namespace model_renderer {
std::vector<int> gameplay_hud_snapshot(const PlayerGameplayBinding&);
std::vector<std::string> gameplay_hud_icon_names(const PlayerGameplayBinding&);
std::string gameplay_use_potion(const PlayerGameplayBinding&);
}
