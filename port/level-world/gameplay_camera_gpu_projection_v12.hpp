#pragma once
#include "gameplay_camera_matrix_v8.hpp"
namespace dh2::camera {
// Borrow these from the same actual original driver/target owner. Count is
// (driver.cc-driver.c8)/4; flip_y is driver4a0; orientation is driver13c.
struct GpuProjectionFieldsV12 {std::uint32_t render_target_count{};std::uint32_t orientation{};std::uint8_t flip_y{};};
}
extern "C" void dh2_camera_gpu_projection_v12(dh2::camera::MatrixV8*,const dh2::camera::GpuProjectionFieldsV12*);
