#pragma once
#include "gameplay_camera_level_v4.hpp"
#include "gameplay_camera_design_v5.hpp"
namespace dh2::camera {
struct CameraPartyActorV14 {std::uintptr_t character{};bool dead{};PointV2 position160{};};
struct CameraLayoutServicesV14 {
 std::shared_ptr<void> world;
 std::function<bool(std::int32_t&,std::string&)> source_party_count6c4;
 std::function<bool(std::uint8_t&,std::string&)> source_online5;
 // Actual PlayerManager.GetPlayer(index,false) must deliver its PlayerInfo;
 // an existing empty Character660 is legal, missing PlayerInfo is failure.
 std::function<bool(unsigned,CameraPartyActorV14&,std::string&)> get_player;
 std::function<bool(const PointV2&,std::array<float,2>&,std::string&)> get_screen_coord;
 std::function<bool(bool&,std::string&)> has_current_level;
 std::function<bool(std::int32_t&,std::string&)> num_local_players;
 std::function<bool(std::array<std::uint8_t,4>&,std::string&)> level_slots190;
 // Original GetWorldCoord(normalized screen,height) includes viewport int
 // quantization, real collision-manager ray and limited-plane intersection.
 std::function<bool(const std::array<float,2>&,float,PointV2&,std::string&)> get_world_coord;
};
bool source_camera_autozoom_v14(float& automatic8c,const data::DesignSettingsOwner::Borrow&,const CameraLayoutServicesV14&,std::string&);
bool source_camera_centering_v14(PointV2&,const CameraLayoutServicesV14&,std::string&);
}
extern "C" float dh2_camera_margin_v14(const float* screen,const float* bounds,std::uint32_t count);
// bounds5=sides,top,bottom,reference,step; whole source arithmetic tail.
extern "C" void dh2_camera_autozoom_arithmetic_v14(float*,const float*,const float*,std::uint32_t);
extern "C" float dh2_camera_centering_shift_v14(std::int32_t local_players,const std::uint8_t slots[4]);
