#pragma once
#include "campaign_save_profile_v45.hpp"
#include "character_menu_profile_load_v51.hpp"
#include "application_save_files_owner_v61.hpp"
#include "../engine-ui/owned_hud_settings_v1.hpp"
namespace dh2::player {
// DISTINCT original PlayerInfo680 preview save, never Character14e8. Mask1
// consumes the actual seven metadata sections and their existing readers.
class PlayerInfoProfileOwnerV70 {
 std::shared_ptr<data::PlayerSavegameV1> save_;
 std::shared_ptr<data::PlayerSaveLoadOwnerV1> load_;
 std::shared_ptr<level::CampaignSaveProfileV45> profile_;
 std::shared_ptr<character::CharacterMenuProfileLoadV51> reader_;
 bool attempted_{},ready_{};
public:
 bool construct(std::int32_t slot,
  const std::shared_ptr<application::ApplicationSaveFilesOwnerV61>&,
  const std::shared_ptr<ui::OwnedHudSettingsV1>&,
  std::shared_ptr<void> actual_table_lease,const data::CharacterTable*,
  std::shared_ptr<const void> selected_file_lease,data::Bytes selected_file,
  std::string&);
 const std::shared_ptr<data::PlayerSavegameV1>& save()const noexcept{return save_;}
 bool ready()const noexcept{return ready_;}
};
}
