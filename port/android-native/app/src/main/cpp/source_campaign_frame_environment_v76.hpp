#pragma once
#include <navigation_controller.hpp>
#include <memory>
#include <string>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
struct SourceCampaignFrameEnvironmentV76 {
 std::shared_ptr<void> actual_scope;
 const dh2::navigation::CollisionWorld* geometry{};
 const dh2::navigation::Graph* graph{};
 dh2::navigation::ObstacleRegistry* registry{};
 const dh2::navigation::MotionPolicy* motion_policy{};
 dh2::navigation::ControllerWorkspace* workspace{};
 std::uint32_t dt_ms{};
};
// Environmental resources only. The actual class still supplies its native
// body/visual, virtual policies, subobject methods, auxiliary and target node.
// This borrower neither executes a frame nor advances physics or animation.
bool borrow_source_campaign_frame_environment_v76(const SourceCampaignCandidateBorrowV55&,
 std::uint32_t actual_path_count,std::uint32_t actual_avoidance_count,
 SourceCampaignFrameEnvironmentV76&,std::string&);
}
