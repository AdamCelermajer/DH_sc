#pragma once
#include <memory>
#include <string>
namespace model_renderer {
struct SourceWorldBorrowV61;
struct SourceCampaignCandidateBorrowV55;
//Enrollment is legal at World creation: no V69/class owner is demanded until
//the concrete packet is composed after V62 publication, before the first tick.
bool install_source_campaign_gameplay_inputs_v107(const std::shared_ptr<SourceWorldBorrowV61>&,std::string&);
bool bind_source_campaign_early_gameplay_v107(const SourceCampaignCandidateBorrowV55&,std::string&);
}
