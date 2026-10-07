#pragma once
#include <zone_collision_runtime_v83.hpp>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// The incoming services carry genuine native mesh/other-body/save leaves.
// This binder replaces the shared PM/Character/Level/constants/assertion
// transport with the actual campaign owners, retaining no containing World.
bool bind_source_campaign_zone_services_v83(const SourceCampaignCandidateBorrowV55&,
 dh2::world::ZoneCollisionServicesV83&,std::string&);
// Fill actual shared trigger queries/scheduler routes. Other reached native
// services (sound, Door state, network, physical and release) are preserved.
bool bind_source_campaign_trigger_services_v83(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t zone,const dh2::world::ZoneCollisionServicesV83&,
 dh2::world::TriggerZoneServicesV22&,std::string&);
bool bind_source_campaign_exit_services_v83(const SourceCampaignCandidateBorrowV55&,
 const dh2::world::ZoneCollisionServicesV83&,const dh2::world::TriggerZoneServicesV22&,
 dh2::world::ExitZoneRuntimeServicesV83&,std::string&);
// Implemented by native_zone_presentation_v83.inc after the actual native
// menu resource owner, using the process's existing OriginalUiSession.
bool bind_native_zone_presentation_v83(const std::shared_ptr<void>& actual_world,
 dh2::world::ExitZoneRuntimeServicesV83&,std::string&);
}
