#pragma once

#include <array>
#include <vector>

namespace dh::foundation {

// Deliberately independent of graphics APIs and renderer vertex types.
struct CameraVec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct CameraPose {
    CameraVec3 position{0.0f, 2.0f, 5.0f};
    CameraVec3 target{0.0f, 0.0f, 0.0f};
    CameraVec3 up{0.0f, 1.0f, 0.0f};
    float verticalFovDegrees = 60.0f;
};

// Matrices are column-major, right-handed, with OpenGL [-1, 1] clip depth.
std::array<float, 16> cameraViewMatrix(const CameraPose& pose);
std::array<float, 16> cameraProjectionMatrix(float verticalFovDegrees,
                                           float aspectRatio,
                                           float nearPlane, float farPlane);

class FreeCamera {
public:
    explicit FreeCamera(CameraPose pose = {});
    const CameraPose& pose() const noexcept { return pose_; }
    void setPose(CameraPose pose);
    // Distances in world units; positive forward moves toward the target.
    void move(float localRight, float localUp, float localForward);
    // Positive yaw turns right; positive pitch looks up. Pitch is constrained
    // to avoid a degenerate view at the up-axis poles.
    void rotate(float yawDegrees, float pitchDegrees);

private:
    CameraPose pose_;
};

struct CameraKeyframe {
    double timeSeconds = 0.0;
    CameraPose pose{};
    // On this destination keyframe, hold the preceding shot until this time,
    // then jump. Otherwise linearly interpolate position, target, up and FOV.
    bool cut = false;
};

class CameraTimeline {
public:
    CameraTimeline() = default;
    explicit CameraTimeline(std::vector<CameraKeyframe> keyframes);
    // Times must be finite, nonnegative and unique; input is sorted by time.
    // The first pose is held from time zero to the first keyframe if necessary.
    void setKeyframes(std::vector<CameraKeyframe> keyframes);
    void play() noexcept;
    void reset() noexcept;
    void update(double deltaSeconds);
    CameraPose sample(double timeSeconds) const;
    CameraPose sample() const { return sample(elapsed_); }
    bool finished() const noexcept;
    bool playing() const noexcept { return playing_; }
    double duration() const noexcept;
    double elapsed() const noexcept { return elapsed_; }
    const std::vector<CameraKeyframe>& keyframes() const noexcept { return keyframes_; }

private:
    std::vector<CameraKeyframe> keyframes_;
    double elapsed_ = 0.0;
    bool playing_ = false;
};

} // namespace dh::foundation
