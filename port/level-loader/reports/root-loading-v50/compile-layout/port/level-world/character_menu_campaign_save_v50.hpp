#pragma once
#include "campaign_save_profile_v45.hpp"
#include "player_save_metadata_writer_v45.hpp"
#include "player_save_inventory_writer_v45.hpp"
#include "player_save_collections_writer_v45.hpp"
namespace dh2::character {
struct CharacterMenuSaveBorrowV50 {
 std::shared_ptr<void> receiver;
 data::PropertyView* properties{};
 const data::FreshInventoryOwnedV4* inventory{};
};
struct CharacterMenuCampaignSaveServicesV50 {
 std::shared_ptr<void> tables;
 const data::CharacterTable* characters{};
 data::SkillTables::Borrow skills;
 const std::vector<std::string>* power_names{};
 const std::vector<std::string>* level_names{};
 const std::vector<std::string>* map_names{};
 std::function<bool(std::int32_t&,std::string&)> current_difficulty;
 // Reread SAME live actor/Gear at each section. Provider must pin receiver.
 std::function<bool(CharacterMenuSaveBorrowV50&,std::string&)> actor;
 std::function<bool(std::uintptr_t,level::SavegameStreamV2&,std::string&)> quest_save_data;
 // Actual offline/online getter and reached network/checkpoint endpoints.
 data::PlayerSaveWriteServicesV1 remaining;
};
// Full15-section registration over an ALREADY published same profile. This
// does not create/adopt a fake profile for slot-1 or replace the native Save.
class CharacterMenuCampaignSaveV50:public std::enable_shared_from_this<CharacterMenuCampaignSaveV50> {
 std::shared_ptr<data::PlayerSaveLoadOwnerV1> authority_;
 std::shared_ptr<level::CampaignSaveProfileV45> profile_;
 CharacterMenuCampaignSaveServicesV50 services_;
 std::shared_ptr<data::PlayerSaveNamedWriterV1> named_;
 std::shared_ptr<level::PlayerSaveMetadataWriterV45> metadata_;
 bool attempted_{},ready_{};
 bool write(const char*,level::SavegameStreamV2&,std::string&);
 bool coherent(std::string&)const;
public:
 CharacterMenuCampaignSaveV50(std::shared_ptr<data::PlayerSaveLoadOwnerV1>,
  std::shared_ptr<level::CampaignSaveProfileV45>,CharacterMenuCampaignSaveServicesV50);
 bool bind(std::string&);
 data::PlayerSaveWriteServicesV1 write_services();
 // Exact menu reload20 PROP reader from this SAME profile cache. Other Load
 // masks still need their source initialization/named-reader providers.
 bool load_property_section(const data::PlayerSaveLoadRequestV1&,
  data::PlayerSaveLoadResponseV1&,std::string&);
 bool ready()const noexcept{return ready_;}
};
}
