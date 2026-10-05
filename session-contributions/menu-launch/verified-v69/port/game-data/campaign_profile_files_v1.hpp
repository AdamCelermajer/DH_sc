#pragma once
#include "data.hpp"
namespace dh2::data {
enum class CampaignProfileOriginV1 {base,backup};
struct CampaignProfileFileV1 {std::vector<std::uint8_t> bytes;CampaignProfileOriginV1 origin=CampaignProfileOriginV1::base;};
// Four source campaign slots, dh2_%03u.savegame and .bak. These readers own
// file access only. Output is atomic; no repair/write or gameplay ownership.
bool campaign_profile_exists_v1(const std::string& directory,std::uint32_t slot,bool&,std::string&);
// Original cache uses backup if base is missing, at most3 bytes or starts with
// corruption markerFFFFFFFF. It does not retry arbitrary malformed sections.
// Native bounded policy rejects nonregular files, IO errors and files>32MiB.
bool read_campaign_profile_v1(const std::string& directory,std::uint32_t slot,
    CampaignProfileFileV1&,std::string&);
}
