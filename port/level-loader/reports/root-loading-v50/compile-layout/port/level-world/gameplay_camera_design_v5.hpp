#pragma once
#include "../game-data/design_settings.hpp"
namespace dh2::camera {
bool source_zoom_bounds_v5(const data::DesignSettingsOwner::Borrow&,bool minimap,float& lower,float& upper,std::string&);
struct AutoZoomDesignV5 {float reference{},step{},bottom{},sides{},top{};};
bool source_autozoom_design_v5(const data::DesignSettingsOwner::Borrow&,AutoZoomDesignV5&,std::string&);
}
