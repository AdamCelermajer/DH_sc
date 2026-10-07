#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include <functional>
#include <array>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;struct SourceObjectUpdateLeavesV105;
enum class SourceCharacterRewardTextV114 {gold,xp};
using SourceCharacterRewardTextCallbackV114=std::function<bool(std::uintptr_t,SourceCharacterRewardTextV114,std::int32_t,std::string&)>;
bool bind_source_campaign_character_reward_text_v114(const std::shared_ptr<void>&,std::shared_ptr<void>,SourceCharacterRewardTextCallbackV114,std::string&);
bool source_campaign_character_frame_v111(const std::shared_ptr<void>&,std::uintptr_t,std::string&);
bool source_campaign_character_target_position_v114(const std::shared_ptr<void>&,std::uintptr_t,std::array<float,3>&,std::string&);
bool source_campaign_character_force_position_v111(const std::shared_ptr<void>&,std::uintptr_t,std::string&);
bool source_campaign_character_master_v111(const std::shared_ptr<void>&,std::uintptr_t,std::uintptr_t&,std::string&);
bool source_campaign_character_set_master_v111(const std::shared_ptr<void>&,std::uintptr_t,std::uintptr_t,std::string&);
bool compose_source_campaign_character_frame_leaves_v111(const SourceCampaignCandidateBorrowV55&,SourceObjectUpdateLeavesV105&,std::string&);
}
