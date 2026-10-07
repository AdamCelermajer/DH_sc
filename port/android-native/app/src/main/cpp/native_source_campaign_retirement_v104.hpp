#pragma once
#include "source_campaign_retirement_v88.hpp"
namespace model_renderer {
// Actual process UI/audio providers bind these leaves. No source destruction
// is delegated here: callbacks run only after original GS/Level release.
struct NativeCampaignRetirementLeavesV104 {
 std::shared_ptr<void> owner;
 // Optional explicit captured-audio leaf. If absent, request captures the
 // actual V46/V67 providers and their SAME process manager through V102.
 std::function<SourceCampaignRetirementResultV88(const std::shared_ptr<void>&,std::string&)> audio;
 std::function<bool(const std::shared_ptr<void>&,std::string&)> menu;
};
bool request_native_source_campaign_retirement_v104(const std::shared_ptr<void>& actual_world,
 NativeCampaignRetirementLeavesV104,std::string&);
bool request_native_source_campaign_retirement_v104(const std::shared_ptr<void>& actual_world,std::string&);
}

namespace model_renderer {
//Actual failed-start request, including genuinely absent pre-World C1 domains.
//Uses existing outer drain; completion permits a new confirmed source start.
bool request_native_source_campaign_recovery_v115(std::string&);
//Explicitly resumes the SAME retained failed release, never rebuilds its prefix.
bool retry_native_source_campaign_recovery_v115(std::string&);
}
