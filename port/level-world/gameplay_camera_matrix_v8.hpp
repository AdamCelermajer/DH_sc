#pragma once
#include <cstdint>
namespace dh2::camera {
struct MatrixV8 {float values[16]{};std::uint8_t identity_flag{};};
}
extern "C" {
void dh2_camera_look_at_v8(dh2::camera::MatrixV8*,const float eye[3],const float target[3],const float up[3]);
// input4 = source FOV radians, aspect, near, far. Preserves original 0..1
// depth / positive-W engine matrix; GL submission conversion is separate.
void dh2_camera_perspective_v8(dh2::camera::MatrixV8*,const float input4[4]);
}
