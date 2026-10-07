#pragma once
#include <cstdint>
#include <string>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
bool source_campaign_noncharacter_is_zonable_v105(const SourceCampaignCandidateBorrowV55&,std::uintptr_t,bool&,std::string&);
bool source_campaign_noncharacter_remote_v105(const SourceCampaignCandidateBorrowV55&,std::uintptr_t,bool&,std::string&);
bool source_campaign_noncharacter_sync_visibility_v105(const SourceCampaignCandidateBorrowV55&,std::uintptr_t,std::string&);
bool source_campaign_noncharacter_virtual_v105(const SourceCampaignCandidateBorrowV55&,std::uintptr_t,std::uint32_t,std::string&);
}
