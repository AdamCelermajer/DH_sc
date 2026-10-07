#pragma once
#include <string>
#include <memory>
#include <vector>
#include <cstdint>
namespace dh2::data {struct QuestRewardDefinitionV51;}
namespace dh2::android_ui {struct SourceScriptUiLeavesV96;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// Enroll actual UI/actor leaves after source script parsing and before HUD C1.
// Does not start scripts, execute frames or construct another runtime.
bool bind_native_source_script_ui_v98(const SourceCampaignCandidateBorrowV55&,std::string&);
bool build_native_source_script_ui_leaves_v99(const SourceCampaignCandidateBorrowV55&,
 dh2::android_ui::SourceScriptUiLeavesV96&,std::string&);
bool source_native_quest_completed_dialog_v108(const std::shared_ptr<void>& actual_world,std::int32_t title,std::int32_t style,const std::vector<dh2::data::QuestRewardDefinitionV51>&,std::string&);
bool source_native_igm_opened_v98(const std::shared_ptr<void>& actual_world,bool&,std::string&);
bool source_native_cutscene_popmenu_v99(const std::shared_ptr<void>& actual_world,bool all,std::string&);
}
