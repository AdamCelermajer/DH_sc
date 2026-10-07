#pragma once
#include "player_manager_owner_v1.hpp"
#include <functional>
namespace dh2::player {
struct PlayerLocalControllerBorrowV70 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 const std::uint8_t* connected758{};
};
struct PlayerLocalControllersServicesV70 {
 std::shared_ptr<void> provider;
 std::function<bool(std::int32_t&,std::string&)> gamepad_count;
 std::function<bool(std::int32_t,PlayerLocalControllerBorrowV70&,std::string&)> gamepad;
 std::function<bool(bool&,std::string&)> online;
 // Genuine source tail for an existing local record with slot664==-1:
 // state240==2 invokes _UpdateJoiningController; other states inspect actual
 // controller1d8/1e0/1e4/1e8 and may invoke the source HUD profile prompt.
 // The selected-profile path never reaches this tail.
 std::function<bool(std::int32_t,PlayerInfoFieldsV1&,const PlayerLocalControllerBorrowV70&,std::string&)> unselected_profile;
};
// Whole ordinary offline _CheckLocalControllers378c80 loop. Count is captured
// once, source controller0 is queried even when native gamepad count is zero,
// and only its connected decision is forced true by the original branch.
class PlayerLocalControllersOwnerV70 {
 PlayerManagerOwnerV1& manager_;PlayerLocalControllersServicesV70 services_;
 bool delivering_{};
public:
 PlayerLocalControllersOwnerV70(PlayerManagerOwnerV1& manager,PlayerLocalControllersServicesV70 services):manager_(manager),services_(std::move(services)){}
 bool update(std::string&);
};
}
