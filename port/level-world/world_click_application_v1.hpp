#pragma once
#include "../engine-ui/owned_hud_settings_v1.hpp"
namespace dh2::world {
// Whole original IsUsingDPad320e74 normalizes GetSavedOption("DPad")!=0.
// Borrow the SAME existing Application+4c private settings owner.
inline bool world_click_using_dpad_v1(const ui::OwnedHudSettingsV1* same_settings,bool& out,std::string& error){
 if(!same_settings){error="required same Application private settings owner";return false;}
 out=same_settings->saved_option("DPad")!=0;return true;
}
}
