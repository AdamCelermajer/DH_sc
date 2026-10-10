#pragma once

#include "camera.hpp"

namespace dh::foundation {

struct CameraMovementBasis {
    CameraVec3 right{1,0,0};
    CameraVec3 forward{0,1,0};
    CameraVec3 up{0,0,1};
};

// Projects the view onto the walking plane. Movement stays planar even when
// the camera looks downward. Degenerate overhead views use a stable fallback.
CameraMovementBasis cameraMovementBasis(const CameraPose& pose,
                                       CameraVec3 worldUp = {0,0,1});

struct CameraFollowConfig {
    // Development settings, not recovered original campaign constants.
    // Prefer configureFromPose with an authored or already chosen camera pose.
    float distance = 1.0f;
    float pitchDegrees = 45.0f;
    // Heading zero looks toward +Y in Z-up content; positive heads toward +X.
    float headingDegrees = 0.0f;
    CameraVec3 targetOffset{0,0,0}; // world-space anchor/centering offset
    // Exponential response per second: zero snaps; positive values smooth.
    float smoothingRate = 0.0f;
    float verticalFovDegrees = 60.0f;
    CameraVec3 worldUp{0,0,1};
    // Explicit opt-in. Original camera facing-follow has not been established.
    bool followFacing = false;
};

struct CameraActorTarget {
    CameraVec3 position{0,0,0};
    CameraVec3 facing{0,1,0};
};

class CameraFollow {
public:
    explicit CameraFollow(CameraFollowConfig config = {});
    void configure(CameraFollowConfig config);
    // Derive distance, pitch, heading and offset while preserving this pose.
    // Does not assert that the input is an original gameplay camera.
    void configureFromPose(const CameraPose& pose, CameraVec3 actorPosition,
                           float smoothingRate = 0.0f);
    const CameraFollowConfig& config() const noexcept { return config_; }
    const CameraPose& pose() const noexcept { return pose_; }
    void reset(const CameraActorTarget& actor);
    void update(const CameraActorTarget& actor, double deltaSeconds);
    CameraMovementBasis movementBasis() const;

private:
    CameraPose desiredPose(const CameraActorTarget& actor) const;
    CameraFollowConfig config_;
    CameraPose pose_;
    bool initialized_ = false;
};

} // namespace dh::foundation
