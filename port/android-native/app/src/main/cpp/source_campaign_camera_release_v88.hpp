#pragma once
#include <functional>
#include <memory>
#include <string>
#include <cstdint>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
struct SourceCampaignCameraReleaseBorrowV88 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 std::uint32_t source_slot{}; //actual128/12c, not native owner layout
 std::function<bool(std::string&)> deleting_destructor;
};
bool borrow_source_campaign_camera_release_v88(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t actual_camera,SourceCampaignCameraReleaseBorrowV88&,std::string&);
}
