#pragma once
#include "world_click_target_v1.hpp"
namespace dh2::world {
struct WorldTouchFloorBorrowV1 {std::uintptr_t identity{};const std::uint32_t* flags24{};};
struct WorldTouchProjectionServicesV1 {
 // SAME live camera SceneCollisionManager source slot14, camera argumentNULL.
 std::function<bool(std::int32_t,std::int32_t,std::array<float,3>&,std::array<float,3>&,std::string&)> screen_ray;
 // Reload actual PFWorld rooms8..c and PFRoom floors30..34 by source index.
 std::function<bool(std::uint32_t,bool&,std::uintptr_t&,std::string&)> room_at;
 std::function<bool(std::uintptr_t,std::uint32_t,bool&,WorldTouchFloorBorrowV1&,std::string&)> floor_at;
 // Source PFFloor51b874 uses SAME scene node40->triangleSelector virtualb0,
 // actual SceneCollisionManager getCollisionPoint; hit copies returned point.
 std::function<bool(std::uintptr_t,const std::array<float,3>&,const std::array<float,3>&,bool&,std::array<float,3>&,std::string&)> floor_collision;
};
// Whole PFWorld TranslateScreenToWorld525884 + world/room iteration. Supported
// screen float domain is finite signed-int32 representable, exactly source
// truncation toward zero. It returns the FIRST room/floor hit in source order.
bool world_touch_projection_v1(float x,float y,const WorldTouchProjectionServicesV1&,
                               bool& hit,std::array<float,3>& point,std::string&);
}
