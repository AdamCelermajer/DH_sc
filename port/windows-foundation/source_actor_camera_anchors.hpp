#pragma once
#include "camera.hpp"
#include <functional>
#include <memory>
#include <string>
#include <cstdint>
namespace dh::foundation {
// Borrow actual admitted owner fields, including its known source2e0 slot.
// Known zero is original constructor/no-anchor state, not lookup failure.
struct SourceActorCameraAnchorInput {
    std::shared_ptr<void> actor_lease;
    std::uintptr_t actor_identity=0;
    bool position_admitted=false;
    CameraVec3 position160{};
    bool anchor_slot_admitted=false;
    std::uintptr_t anchor2e0=0;
    std::function<bool(std::uintptr_t,CameraVec3&,std::string&)> anchor_position_c;
};
// Whole original getter3943b8: attached+c if nonnull, otherwise actor160.
// Does not create an NPC AnchorForward or derive a point from visual bones.
bool source_actor_camera_anchor(const SourceActorCameraAnchorInput&,
                               CameraVec3& output,std::string& error);
}
