#pragma once
#include <cstdint>
#include <string>
namespace dh2::loader {struct CheckedCommandBorrowV59;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
bool execute_source_campaign_script_environment_v120(const SourceCampaignCandidateBorrowV55&,
 const dh2::loader::CheckedCommandBorrowV59&,bool,std::int32_t,bool&,std::string&);
bool blocking_source_campaign_script_environment_v120(const SourceCampaignCandidateBorrowV55&,
 const dh2::loader::CheckedCommandBorrowV59&,bool&,bool&,std::string&);
}
