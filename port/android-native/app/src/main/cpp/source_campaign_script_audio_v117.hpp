#pragma once
#include <cstdint>
#include <string>
namespace dh2::loader {struct CheckedCommandBorrowV59;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
bool execute_source_campaign_script_audio_v117(const SourceCampaignCandidateBorrowV55&,
 const dh2::loader::CheckedCommandBorrowV59&,bool skip,std::int32_t module,
 bool& handled,std::string&);
}
