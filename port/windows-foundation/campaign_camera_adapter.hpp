#pragma once
#include "camera.hpp"
#include "original_campaign_runtime.hpp"
#include <functional>

namespace dh::foundation {
struct CampaignCameraProviders {
    // Source name lookup uses the current world's global object namespace,
    // including its registered aliases. The adapter invents no object names.
    std::function<bool(const std::string&,std::uint64_t&,bool& found,std::string&)> named_target;
    std::function<bool(std::uint64_t&,std::string&)> local_player;
    // Same object's original GetCameraAnchorPosition: explicit anchor if one
    // exists, otherwise actual position. Dummy positions include module offset.
    std::function<bool(std::uint64_t,CameraVec3&,std::string&)> anchor;
};
struct CampaignCameraFrame {
    CameraVec3 anchor{};
    // The original transition returns before damping. Preserve the camera's
    // existing damping velocity while assigning this root position directly.
    bool applyDamping = true;
    bool hasTarget = false;
};
class CampaignCameraAdapter {
public:
    explicit CampaignCameraAdapter(CampaignCameraProviders providers = {});
    void bind(CampaignCameraProviders providers);
    bool seed_target(std::uint64_t identity,std::string& error);
    // Handles verified SetCamera(kind4, literal no-op) and SetCameraTarget(kind8).
    // Other kinds return success with handled=false for another source provider.
    bool command(CampaignCommandPhase,const OriginalCampaignCommand&,bool skip,
                 bool& handled,bool& blocking,std::string& error);
    // Call once per actual camera frame, not from script command Update phases.
    bool tick(std::uint32_t deltaMilliseconds,CampaignCameraFrame&,std::string& error);
    std::uint64_t target() const noexcept { return target_; }
    std::int32_t transition_remaining() const noexcept { return remaining_; }
private:
    bool set_target(std::uint64_t,std::int32_t,std::string&);
    CampaignCameraProviders providers_;
    std::uint64_t target_ = 0;
    CameraVec3 transitionStart_{};
    std::int32_t duration_ = 0,remaining_ = 0;
};
} // namespace dh::foundation
