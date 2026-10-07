#pragma once
#include "player_level_quicksave_v29.hpp"
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
//Actual Level QuickSave body/typed Save_ec receiver, also reusable by source
//script/checkpoint/unload callers. It creates no Save/Gear/player authority.
bool borrow_source_campaign_quicksave_v88(const SourceCampaignCandidateBorrowV55&,
 std::shared_ptr<dh2::player::PlayerLevelQuickSaveV29>&,std::string&,
 std::function<bool(bool&,std::string&)> actual_positive_hosting={});
}
