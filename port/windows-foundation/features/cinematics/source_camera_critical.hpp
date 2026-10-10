#pragma once
#include "source_camera_admission.hpp"
#include "../../../level-world/application_player_manager_bootstrap_v59.hpp"
namespace dh::foundation {
struct SourceCameraCriticalProviders {
    SourceCameraAdmissionProviders authority;
    // Actual Application.GetCurrentLevel()->field128. Missing current Level
    // or actual NULL128 returns0; failure is distinct from those source no-ops.
    std::function<bool(std::uintptr_t&,std::string&)> currentCamera128;
    std::function<bool(std::uintptr_t,bool&,std::string&)> isPlayer;
    std::function<bool(std::uintptr_t,dh2::camera::PointV2&,std::string&)> lookAt;
};
// Only called at the original F_ApplyResult critical-camera delivery prefix.
// No new hit rule, amplitude, duration or screen-space impulse is introduced.
class SourceCameraCriticalEffect {
    SourceCameraCriticalProviders providers_;
public:
    explicit SourceCameraCriticalEffect(SourceCameraCriticalProviders p):providers_(std::move(p)){}
    bool apply(std::uintptr_t actualCharacter,std::string&)const;
};
}
