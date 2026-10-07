#pragma once
#include "gameplay_camera_damping_v1.hpp"
#include <array>
#include <memory>
namespace dh2::camera {
using PointV2=std::array<float,3>;
struct TargetStateV2 {
 std::uintptr_t target{};PointV2 transition_start{};
 std::int32_t transition_duration{},transition_remaining{};
 bool offset_enabled{};DampingStateV1 damping;PointV2 ghost{};
};
struct TargetServicesV2 {
 // All callbacks borrow the SAME retained scene and World actor; no scene copy.
 std::shared_ptr<void> scene_lease;
 bool root_present{};
 const PointV2* source_origin{}; // actual Point3D ZERO, used when prior target NULL
 std::function<bool(std::uintptr_t,PointV2&,std::string&)> actor_anchor;
 std::function<bool(PointV2&,std::string&)> root_position;
 std::function<bool(const PointV2&,std::string&)> root_set_position;
 std::function<bool(std::uint32_t&,std::string&)> application_dt;
 std::function<bool(float,PointV2&,std::string&)> center_offset;
};
class GameplayCameraTargetV2 {
 TargetStateV2 state_;TargetServicesV2 services_;
public:
 explicit GameplayCameraTargetV2(TargetServicesV2 s):services_(std::move(s)){}
 TargetStateV2& fields()noexcept{return state_;}
 const TargetStateV2& fields()const noexcept{return state_;}
 // Source SetTarget4119c4 ignores NULL; immediate target resets transition
 // and ghost, but intentionally preserves follow velocity.
 bool set_target(std::uintptr_t,std::int32_t transition_ms,std::string&);
 // EnableDamping41161c always resets the actual follow velocity to source
 // Point3D ZERO, including repeated calls with the same enabled value.
 bool enable_damping(bool,std::string&);
 bool transition(bool& handled,std::string&);
 bool offset(PointV2&,std::string&);
 bool ghost(PointV2&)noexcept;
 bool damping(PointV2&,std::string&);
 bool update(std::string&);
};
}
extern "C" void dh2_gameplay_camera_transition_v2(float out[3],const float start[3],const float end[3],std::int32_t remaining,std::int32_t duration);
