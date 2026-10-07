#pragma once
#include "campaign_profile_files_v1.hpp"
#include "menu_profile_metadata_v1.hpp"
#include <memory>
namespace dh2::android_ui {
// Immutable selected file receipt, not another Player/Save/Gear authority.
// Carries the exact read after NativeStartGame metadata/class validation.
struct FrontSelectedProfileV50 {
 data::CampaignProfileFileV1 file;
 data::MenuProfileMetadataV1 metadata;
 std::string private_directory;
 std::int32_t requested_difficulty{};
 bool has_gear{},has_properties{},has_quests{},has_skills{},has_faeries{};
};
bool retain_front_selected_profile_v50(data::CampaignProfileFileV1,
 data::MenuProfileMetadataV1,const std::string& actual_private_directory,
 std::int32_t requested_difficulty,std::shared_ptr<const FrontSelectedProfileV50>&,
 std::string&);
}
