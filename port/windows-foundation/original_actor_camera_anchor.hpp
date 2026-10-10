#pragma once
#include "camera.hpp"
#include <cstdint>
#include <memory>
#include <string>

namespace dh::foundation {
class AssetCatalog;
// Whole original GameObject.GetLookAtVec393ae4 from the current Euler Z.
CameraVec3 original_actor_camera_look_at(float eulerZRadians);
struct OriginalActorCameraAnchorConfig {
    bool useStaticCamera = false; // Caller supplies actual Debug switch result.
    float maximumDistance = 0, distancePerUpdate = 0, threshold = 0;
};
bool load_original_actor_camera_anchor_config(const AssetCatalog&,bool useStaticCamera,
                                              OriginalActorCameraAnchorConfig&,std::string& error);
struct OriginalActorCameraAnchorFrame {
    CameraVec3 position{}; // Actual GameObject position160, not a visual bone.
    bool headingActive = false;
    CameraVec3 heading{}; // Actual controller HeadingState direction1b8.
    CameraVec3 lookAt{};  // Actual GameObject GetLookAtVec, no guessed model axis.
    bool moving = false; // Source SM_IsMoving(false): state4 or19.
    bool attacking = false; // Source SM_IsAttacking: state5.
};
// Owns the recovered AnchorBase/AnchorForward receiver, with a small typed
// provider boundary instead of original Character/World callback ownership.
class OriginalActorCameraAnchor {
public:
    OriginalActorCameraAnchor();
    ~OriginalActorCameraAnchor();
    OriginalActorCameraAnchor(OriginalActorCameraAnchor&&) noexcept;
    OriginalActorCameraAnchor& operator=(OriginalActorCameraAnchor&&) noexcept;
    bool initialize(OriginalActorCameraAnchorConfig,const OriginalActorCameraAnchorFrame&,
                    std::string& error);
    bool update(const OriginalActorCameraAnchorFrame&,std::uint32_t dtMilliseconds,
                std::string& error);
    bool reset(const OriginalActorCameraAnchorFrame&,std::string& error);
    bool initialized() const noexcept;
    CameraVec3 position() const noexcept;
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dh::foundation
