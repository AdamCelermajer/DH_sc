#pragma once
#include <game_object_source_frame_v74.hpp>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// Scoped service loan only; borrowing never executes inherited Update.
bool borrow_source_campaign_frame_services_v106(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t,std::shared_ptr<void>&,dh2::loader::GameObjectSourceFrameServicesV74&,std::string&);
}
