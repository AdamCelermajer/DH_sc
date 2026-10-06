#pragma once
#include "character_menu_queries_owner_v1.hpp"
#include "hud_initialization_owned_v1.hpp"
namespace dh2::ui {
// Direct synchronous adapter inside CharacterMenuAsBridge's active fn_call;
// never reenters SwfMovie.action_script. Remaining services retain their texts.
bool authored_hud_options_bridge_v1(const char* name,CharacterMenuCallV1&,
 const OwnedHudSettingsV1&,const HudInitServices16& remaining,std::string&);
}
