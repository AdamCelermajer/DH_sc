#pragma once
#include <memory>
#include <string>
#include <production_noncharacter_composition_v67.hpp>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// Shared references to the actual factories; no duplicate object registry.
bool borrow_source_campaign_noncharacter_owners_v105(const SourceCampaignCandidateBorrowV55&,
 std::shared_ptr<dh2::loader::ProductionNonCharacterOwnersV67>&,std::string&);
}
