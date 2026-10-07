#pragma once
#include "gameplay_camera_target_v2.hpp"
namespace dh2::camera {
struct LevelStateV4 {
 bool shake84{},mode85{},zoom_locked86{},hold_zoom_a4{};
 float requested_zoom88{},automatic_zoom8c{1.f},applied_zoom90{},default_distance94{};
 PointV2 offset98{};
};
struct LevelServicesV4 {
 TargetServicesV2 target;
 std::function<bool(bool&,std::string&)> active_is_this;
 bool camera_present{};
 std::function<bool(std::uintptr_t,PointV2&,std::string&)> actor_position;
 std::function<bool(PointV2&,std::string&)> handle_centering;
 std::function<bool(bool&,std::string&)> debug_infinite_zoom;
 std::function<bool(bool,float&,float&,std::string&)> zoom_bounds;
 std::function<bool(const PointV2&,std::string&)> camera_set_position;
 std::function<bool(std::uintptr_t,bool&,std::string&)> actor_disabled81;
 // Source AnimSetController same Scene graph: write clip indexc then byte10=0
 // before virtual Play+1c. Absence is the original no-controller branch.
 bool controller_present{};
 std::int32_t* controller_clip_indexc{};std::uint8_t* controller_byte10{};
 std::function<bool(std::int32_t,bool&,std::string&)> controller_play;
};
class GameplayCameraLevelV4 {
 GameplayCameraTargetV2 target_;LevelStateV4 state_;LevelServicesV4 services_;
public:
 explicit GameplayCameraLevelV4(LevelServicesV4 s):target_(s.target),services_(std::move(s)){}
 GameplayCameraTargetV2& target()noexcept{return target_;}
 LevelStateV4& fields()noexcept{return state_;}
 bool play_animation(std::int32_t id,std::int32_t clip_index,bool hold_zoom,std::string&);
 void source_animation_callback()noexcept{state_.shake84=false;}
 bool handle_zoom(std::string&);
 bool update(std::string&);
};
}
