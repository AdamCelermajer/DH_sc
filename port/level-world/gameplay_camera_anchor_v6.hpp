#pragma once
#include "gameplay_camera_target_v2.hpp"
namespace dh2::camera {
struct CameraAnchorBorrowV6 {
 std::shared_ptr<void> actor_lease;
 std::uintptr_t identity{};
 std::uintptr_t* anchor2e0{};
 const float* position160{};
 // Resolve source AnchorBase+c through its real retained receiver. No raw
 // native pointer dereference or layout classification occurs in this helper.
 std::function<bool(std::uintptr_t,const float*&,std::string&)> anchor_position;
 std::function<bool(std::uintptr_t,std::string&)> delete_anchor;
};
bool source_camera_anchor_v6(const CameraAnchorBorrowV6&,PointV2&,std::string&);
bool source_set_camera_anchor_v6(CameraAnchorBorrowV6&,std::uintptr_t,std::string&);
}
