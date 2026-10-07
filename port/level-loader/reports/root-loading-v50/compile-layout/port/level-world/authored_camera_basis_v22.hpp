#pragma once
#include "gameplay_camera_basis_v21.hpp"
namespace dh2::camera {
// Borrow the SAME retained CameraLevel scene, selected instance and actual
// camera FOV service. No view-matrix basis substitution or second node fields.
bool borrow_authored_camera_basis_v22(std::shared_ptr<GameplayCameraSceneV3>,std::uint32_t,
 std::function<bool(float&,std::string&)>,const PointV2* actual_zero,const PointV2* actual_up,
 CameraBasisServicesV21&,std::string&);
}
