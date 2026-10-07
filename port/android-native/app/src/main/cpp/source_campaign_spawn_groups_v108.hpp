#pragma once
#include <spawn_group_manager_v108.hpp>
#include <canonical_spawn_spot_v108.hpp>
namespace dh2::android_ui {class SourceProcessArraysV101;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
bool borrow_process_spawn_arrays_v108(std::shared_ptr<dh2::android_ui::SourceProcessArraysV101>&,std::string&);
bool compose_source_campaign_spawn_groups_v108(const SourceCampaignCandidateBorrowV55&,dh2::world::SpawnGroupServicesV108&,std::string&);
bool bind_source_campaign_spawn_spot_v108(const std::shared_ptr<void>&,dh2::world::CanonicalSpawnSpotV108&,dh2::world::SpawnSpotServicesV108&,std::string&);
bool source_campaign_character_init_spawned_v108(const std::shared_ptr<void>&,std::uintptr_t,std::int32_t,const float*,std::string&);
}
