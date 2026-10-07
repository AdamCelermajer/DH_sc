#pragma once
#include "gameplay_camera_runtime_v11.hpp"
namespace dh2::camera {
struct CameraBasisServicesV21 {
 std::shared_ptr<void> owner;
 bool camera_present{};
 const PointV2* source_vec3_zero{};
 const PointV2* source_vec3_up{};
 std::function<bool(MatrixV8&,std::string&)> absolute_camera_matrix;
 std::function<bool(PointV2&,std::string&)> absolute_camera_position;
 std::function<bool(float&,std::string&)> camera_fov;
};
bool source_camera_basis_v21(const CameraBasisServicesV21&,PointV2&look,PointV2&up,std::string&);
bool source_camera_center_offset_v21(const CameraBasisServicesV21&,float height,PointV2&out,bool&available,std::string&);
}
extern "C" int dh2_camera_center_offset_v21(float*,const float*);
// input10=directionXYZ,eyeZ,height,FOVread1,FOVread2,sourceUpXYZ.
extern "C" void dh2_camera_basis_v21(float*,float*,const float*);
