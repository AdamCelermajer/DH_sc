#pragma once
#include "source_campaign_script_execution_v96.hpp"
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// Enrich the actual UI97 leaves immediately before V96 enrollment. No script
// start, clock, manager, Character, controller, or menu is constructed here.
bool enrich_source_campaign_script_runtime_v99(const SourceCampaignCandidateBorrowV55&,
 dh2::android_ui::SourceScriptUiLeavesV96&,std::string&);
// Initial scheduler enrollment before any Level loading tick. Real SpawnPoint
// placement can StartScript before HUD26; Stage26 later rebinds real parsed
// command actors/HUD without resetting those actual running contexts.
bool prepare_source_campaign_script_start_v99(const SourceCampaignCandidateBorrowV55&,std::string&);
}
