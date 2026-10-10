#pragma once
#include "level_destroy_source_v1.hpp"
namespace dh2::application {class ApplicationServicesOwnerV5;}
namespace dh2::data {struct CampaignProfileFileV1;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
class SourceCampaignReleaseV88;
struct SourceCampaignStartupPrefixV114;
struct SourceWorldBorrowV61;
//Root supplies actual native renderer/menu/audio/class teardown primitives.
//The composition supplies shared App/PM/Save/camera/Lua/RNG source receivers;
//the loader journals retain the original whole Unload/D1 ordering.
struct SourceCampaignReleaseServicesV88 {
 dh2::loader::LevelUnloadServicesV1 unload;
 dh2::loader::LevelDestroyServicesV1 destroy;
 std::function<bool(bool&,std::string&)> positive_local_hosting;
};
bool bind_source_campaign_release_v88(const SourceCampaignCandidateBorrowV55&,
 SourceCampaignReleaseServicesV88,std::shared_ptr<SourceCampaignReleaseV88>&,std::string&,
 const SourceCampaignStartupPrefixV114* actual_startup=nullptr);
bool borrow_source_campaign_release_candidate_v115(SourceCampaignCandidateBorrowV55&,
 std::shared_ptr<SourceWorldBorrowV61>&,std::string&);
bool prepare_source_campaign_startup_release_v115(const SourceCampaignStartupPrefixV114&,std::string&);
bool prepare_source_campaign_release_v108(const SourceCampaignCandidateBorrowV55&,std::string&);
bool source_campaign_release_complete_v88(const std::shared_ptr<SourceCampaignReleaseV88>&,bool& unload,bool& destroy,std::string&);
// Loader cancellation consumes the SAME enrolled unload journal. GS remains
// the later HUD-close/Level D1/current-slot destruction authority.
bool source_campaign_cancel_unload_journal_v135(const std::shared_ptr<SourceCampaignReleaseV88>&,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,std::string&);
bool source_campaign_save_all_players_v88(const SourceCampaignCandidateBorrowV55&,
 bool original_disable_flag,std::string&);
//Fresh Continue input: matching jobs flush precedes each base/backup file
//read. Metadata/QEST construction belongs to the actual original front menu.
bool read_source_campaign_profile_v88(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&,
 const std::string& actual_private_directory,std::uint32_t slot,
 dh2::data::CampaignProfileFileV1&,std::string&);
}
