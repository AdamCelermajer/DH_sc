#pragma once
#include "native_quest_runtime_v76.hpp"
#include "quest_talk_marker_v76.hpp"
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
struct SourceWorldBorrowV61;
struct SourceConditionDependenciesV70;
struct SourceQuestMarkerDependenciesV76 {
 std::shared_ptr<void> provider;
 //Fresh campaign instance only, never Crypt or pre-player libraries.
 std::function<bool(std::shared_ptr<void>&,dh2::fx::CharacterMeshFxOwnerV4*&,std::string&)> actual_fx;
 std::function<bool(dh2::fx::CharacterMeshFxOwnerV4&,std::uintptr_t,std::uintptr_t,std::string&)> visual_owner204;
 std::function<bool(dh2::fx::CharacterMeshFxOwnerV4&,std::uintptr_t,bool,std::string&)> animator_loop;
};
bool bind_source_campaign_quest_markers_v76(const SourceCampaignCandidateBorrowV55&,SourceQuestMarkerDependenciesV76,std::string&);
bool borrow_source_campaign_quest_runtime_v76(const SourceCampaignCandidateBorrowV55&,
 const std::shared_ptr<dh2::character::CharacterMenuQuestsV51>&,std::shared_ptr<dh2::world::NativeQuestRuntimeV76>&,std::string&);
bool source_campaign_character_sg_update_v108(const std::shared_ptr<void>& actual_world,std::uintptr_t,bool force,std::string&);
bool source_native_quest_completed_dialog_v108(const std::shared_ptr<void>& actual_world,std::int32_t title,std::int32_t style,const std::vector<dh2::data::QuestRewardDefinitionV51>&,std::string&);
void source_campaign_condition_dependencies_v76(const std::shared_ptr<SourceWorldBorrowV61>&,SourceConditionDependenciesV70&);
}
