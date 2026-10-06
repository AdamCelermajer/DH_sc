#pragma once
#include <cstdint>
#include <functional>
#include <string>
namespace dh2::camera {
// CameraTargetC2 411cdc: enabled25=1, ratio28=3f333333,
// velocity2c..34=Point3D<float>::ZERO. State belongs to the retained camera.
struct DampingStateV1 {float velocity[3]{};float ratio{0.7f};std::uint8_t enabled{1};};
struct DampingServicesV1 {
 // SAME root SceneNode virtual+a0; source calls twice, preserving order.
 std::function<bool(float[3],std::string&)> root_position;
 std::function<bool(std::uint32_t&,std::string&)> application_dt;
};
bool handle_damping_v1(DampingStateV1&,bool root_present,float target[3],const DampingServicesV1&,bool& applied,std::string&);
}
extern "C" void dh2_gameplay_camera_damping_v1(float velocity[3],float target[3],const float first_position[3],const float second_position[3],const float* ratio,std::uint32_t dt);
