#pragma once
#include "hud_player_infos.hpp"
#include <string>
namespace gameswf {struct fn_call;}
namespace dh2::ui {
// Invoke only within caller's protected retained movie scope. The adapter pins
// the exact AS output object and synchronously dispatches source producer writes;
// it adds no global native registration and owns no actor/session provider.
bool hud_player_infos_callback_v1(const gameswf::fn_call&,const HudInfosServices16&,std::string&);
}
