#pragma once
#include "camera.hpp"
#include "gameplay_camera.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation {
class AssetCatalog;
struct OriginalCameraConfig {
    std::string file = "data/3D/camera/CameraTests.bdae";
    std::string animationSet = "Default";
    std::string node = "PlayerCamera_Default";
    float nearPlane = 600.0f;
    float farPlane = 10000.0f;
};
// Parses the selected original LevelConfig. Empty file/name use the original
// InitPost defaults. A nonempty template requires the asset-aware load method.
bool decode_original_camera_config(const std::vector<std::uint8_t>& xml,
                                  OriginalCameraConfig&, std::string& error);

// Small data-driven owner around the recovered camera graph/animation/damping
// kernels. No original World, SceneManager, or callback ownership is imported.
class OriginalGameplayCamera {
public:
    OriginalGameplayCamera();
    ~OriginalGameplayCamera();
    OriginalGameplayCamera(OriginalGameplayCamera&&) noexcept;
    OriginalGameplayCamera& operator=(OriginalGameplayCamera&&) noexcept;
    bool load(const AssetCatalog& assets, const std::string& selectedLevelUri,
              const std::filesystem::path& cameraFallbackRoot, std::string& error);
    bool loaded() const noexcept;
    // Anchor is the original actor camera anchor (actor position when absent).
    // Millisecond dt is shared with actor/application updates.
    bool reset(CameraVec3 actorAnchor, std::string& error);
    bool update(CameraVec3 actorAnchor, std::uint32_t dtMilliseconds, std::string& error);
    // Source CameraTarget transitions bypass damping while retaining velocity.
    bool update_anchor(CameraVec3 anchor,std::uint32_t dtMilliseconds,bool applyDamping,std::string& error);
    const CameraPose& pose() const noexcept;
    const OriginalCameraConfig& config() const noexcept;
    CameraMovementBasis movementBasis() const;
    float authoredDistance() const noexcept;
    float sourceAspect() const noexcept;
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace dh::foundation
