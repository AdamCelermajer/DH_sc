#pragma once
#include "area_transition_request_v114.hpp"
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// Main/Menu supplies the actual confirmation and independent current profile
// snapshot. This helper borrows the current PM player/Character/Save once and
// forms only the source request. It does not begin retirement or LoadLevel.
bool project_source_campaign_area_transition_v114(const SourceCampaignCandidateBorrowV55&,
 const dh2::loader::AreaTransitionConfirmationV114&,
 dh2::loader::AreaTransitionRestoreReceiptV114,
 dh2::loader::AreaTransitionProjectionServicesV114,
 std::shared_ptr<const dh2::loader::AreaTransitionRequestV114>&,std::string&);
}
