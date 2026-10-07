#pragma once
#include "character_menu_campaign_save_v50.hpp"
#include "character_menu_quests_v51.hpp"
namespace dh2::character {
// Exact initializer callback for PlayerEquipmentRenderInputsV1.saved_gear_v50.
// Pins the immutable source cache Borrow, not merely the mutable profile owner.
bool profile_saved_gear_v51(const std::shared_ptr<level::CampaignSaveProfileV45>&,
 bool& present,data::Bytes&,std::shared_ptr<const void>& snapshot_lease,std::string&);
struct CharacterMenuProfileLoadServicesV51 {
 std::shared_ptr<void> tables;
 const data::CharacterTable* characters{};
 data::SkillTables::Borrow skills;
 std::function<bool(std::int32_t,std::string&)> store_current_difficulty;
 std::function<bool(std::uintptr_t,const std::vector<std::int32_t>*&,std::string&)> character_skill_list;
 const std::vector<std::string>* level_names{};
 const std::vector<std::string>* map_names{};
 const std::vector<std::int32_t>* level_defaults28{};
 const std::vector<std::int32_t>* map_defaults8{};
 // Source Quest initialization/load, GEAR receiver and network/volatile log
 // continue through real owners. Absent tags do not fabricate these calls.
 data::PlayerSaveLoadServicesV1 remaining;
 std::shared_ptr<CharacterMenuCampaignSaveV50> campaign;
 std::shared_ptr<CharacterMenuQuestsV51> quests;
};
// Named read dispatch over SAME already constructed/published profile cache.
// Does not manufacture a slot17 C1, original GS readiness, or starting armor.
class CharacterMenuProfileLoadV51:public std::enable_shared_from_this<CharacterMenuProfileLoadV51> {
 std::shared_ptr<data::PlayerSavegameV1> save_;
 std::shared_ptr<level::CampaignSaveProfileV45> profile_;
 CharacterMenuProfileLoadServicesV51 services_;
 bool invoke(const data::PlayerSaveLoadRequestV1&,data::PlayerSaveLoadResponseV1&,std::string&);
public:
 CharacterMenuProfileLoadV51(std::shared_ptr<data::PlayerSavegameV1>,
  std::shared_ptr<level::CampaignSaveProfileV45>,CharacterMenuProfileLoadServicesV51);
 data::PlayerSaveLoadServicesV1 load_services();
};
}
