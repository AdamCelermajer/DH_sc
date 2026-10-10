#pragma once

#include "../../original_campaign_runtime.hpp"
#include "source_cinematics.hpp"
#include "../progression_barriers/source_door_commands.hpp"
#include "../audio/canonical_source_audio_adapter.hpp"

namespace dh::foundation {

struct SourceCampaignDispatchContextV1 {
    bool received{};
    std::int64_t authored_event_ns{};
};

struct SourceCampaignDispatchServicesV1 {
    // These are the same per-command adapters already owned by the live
    // campaign/application. The dispatch layer creates no scheduler or queue.
    std::function<bool(CampaignCommandPhase,const OriginalCampaignCommand&,int module,
                       bool skip,bool& handled,bool& blocking,std::string&)> cinematics;
    std::function<bool(CampaignCommandPhase,const OriginalCampaignCommand&,int module,
                       bool skip,bool& handled,bool& blocking,std::string&)> doors;
    std::function<bool(CampaignCommandPhase,const OriginalCampaignCommand&,
                       bool received,std::int64_t authored_event_ns,
                       bool& handled,bool& blocking,std::string&)> audio;
    // Remaining already-authored native command bodies, with the same sampled
    // skip argument. Must not start another ScriptManager/runtime.
    std::function<bool(CampaignCommandPhase,const OriginalCampaignCommand&,int module,
                       bool skip,bool& blocking,std::string&)> remaining;
    // Actual ScriptManager receive state plus the caller's sampled source-time
    // pairing for the audio runtime. Required only when kind13/14 is reached.
    std::function<bool(const OriginalCampaignCommand&,
                       SourceCampaignDispatchContextV1&,std::string&)> context;
    // Actual command-source skip state, sampled once by the existing campaign
    // executor before dispatching this command.
    std::function<bool(const OriginalCampaignCommand&,bool& skip,std::string&)> skip;
};

// Binds typed feature providers into the existing OriginalCampaignRuntime
// command lane. Admission and RNG callbacks are preserved from the existing
// service set; the existing runtime remains the only script scheduler. The
// dispatcher and all captured callback owners must outlive that runtime.
class SourceCampaignDispatchV1 {
public:
    explicit SourceCampaignDispatchV1(SourceCampaignDispatchServicesV1);
    bool bind(OriginalCampaignRuntime&,OriginalCampaignServices existing,
              std::string&);
    bool command(CampaignCommandPhase,const OriginalCampaignCommand&,int module,
                 bool& blocking,std::string&);
private:
    SourceCampaignDispatchServicesV1 services_;
};

} // namespace dh::foundation
