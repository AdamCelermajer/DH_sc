#pragma once
#include "campaign_save_profile_v45.hpp"
#include "level_savegame_runtime_v1.hpp"
#include "../game-data/player_save_load_owner_v1.hpp"
namespace dh2::level {
struct PlayerCheckpointServicesV83 {
 std::shared_ptr<void> provider;
 std::function<bool(bool&,std::string&)> online,local_hosting;
 std::function<bool(std::uint8_t&,std::string&)> manager719;
 // Actual positive online _SetupStream468630 and empty stream.C1 path;
 // offline never calls these reached providers.
 std::function<bool(bool,bool,std::string&)> setup_stream;
 std::function<bool(std::string&)> ensure_stream;
};
bool player_save_checkpoint_v83(data::PlayerSaveLoadOwnerV1&,
 CampaignSaveProfileV45*,const PlayerCheckpointServicesV83&,std::string&);
}
