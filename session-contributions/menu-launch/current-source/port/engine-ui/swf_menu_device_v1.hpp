#pragma once
#include "hud_startup_callbacks.hpp"
#include <string>
namespace gameswf {struct fn_call;}
namespace dh2::ui {
// Exact original device exception flags plus the live port's driver type.
// This is graphics support; it does not assert an online connection.
struct MenuDeviceFactsV1 {
    std::uint8_t sharp{},htc{},no_igp{},multiplayer_mode{};
    std::uint32_t driver_type{};
};
bool swf_menu_multiplayer_enabled_v1(const gameswf::fn_call&,const MenuDeviceFactsV1&,std::string&);
}
