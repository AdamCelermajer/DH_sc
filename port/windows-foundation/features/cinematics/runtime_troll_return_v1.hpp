#pragma once

#include "../../combat_session.hpp"
#include "../../original_campaign_runtime.hpp"
#include "../../original_melee_bindings.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_attack_sequence.hpp"
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation {

struct TrollReturnClipNeedV1 {
    std::int32_t animation_id = -1;
    std::size_t command_index = 0;
    bool wait = false;
    std::vector<std::size_t> source_path;
    std::string uri;
    std::string resolved_path;
    std::string alias;
};

// A read-only projection of the original TrollReturn command leaves. Its
// additional_clips must be appended to the same Troll visual before Session
// initialization; the plan carries source hierarchy/rates, not a time schedule.
struct TrollReturnSourceBankV1 {
    const OriginalCampaignRuntime* campaign = nullptr;
    int script_id = -1;
    std::string trigger_key;
    std::string actor_name;
    OriginalCombatVisualPlan plan;
    OriginalSequencePolicies policies;
    std::vector<TrollReturnClipNeedV1> clip_needs;
    std::vector<std::pair<std::string,std::string>> additional_clips;

    bool matches_command(const OriginalCampaignCommand&,std::size_t* index=nullptr) const noexcept;
    bool selection_for(const OriginalCampaignCommand&,OriginalAttackSelection&,
                       TrollReturnClipNeedV1&,std::string&) const;
};

bool build_troll_return_source_bank_v1(const AssetCatalog&,
    const OriginalMeleeBindings&,const OriginalCampaignRuntime&,
    const CharacterVisualConfig& same_troll_visual,const std::string& role_id,
    TrollReturnSourceBankV1&,std::string&);

struct TrollReturnActorBorrowV1 {
    CombatSession* session = nullptr;
    std::string source_object_name;
    int module = -1;
    ActorId actor_id = invalid_actor_id;
    std::shared_ptr<const void> actor_binding_lease;
};

struct TrollReturnRuntimeServicesV1 {
    // Existing source campaign cinematic command owner. Non-TrollReturn
    // records, including authored PlayCamera44 and host mode effects, remain
    // on this exact callback lane.
    std::function<bool(CampaignCommandPhase,const OriginalCampaignCommand&,int,bool,
                       bool&,bool&,std::string&)> fallback_command;
    // Resolve the exact authored source object name in its module to the
    // ActorId already registered in this CombatSession. This generic sequence
    // path needs no canonical native Character object; the retained session
    // binding lease, source name/module, ActorId and pose are its identity.
    std::function<bool(const std::string&,int,CombatSession&,
                       TrollReturnActorBorrowV1&,std::string&)> borrow_actor;
    // Forward the actual retained animation marker through its existing source
    // event owner. The helper invents no damage/contact behavior.
    std::function<bool(CombatSession&,ActorId,const RetainedAnimationEvent&,
                       std::string&)> animation_event;
};

// Additive provider for only the exact PlayActorAnim records in the loaded
// TrollReturn script. Other commands stay with the existing SourceCinematicCommands
// instance. CampaignRuntime and CombatSession remain the only owners of script
// ordering and animation time respectively.
class TrollReturnRuntimeV1 {
public:
    TrollReturnRuntimeV1(OriginalCampaignRuntime&,CombatSession&,
                         const TrollReturnSourceBankV1&,TrollReturnRuntimeServicesV1);
    bool command(CampaignCommandPhase,const OriginalCampaignCommand&,int module,
                 bool skip,bool& handled,bool& blocking,std::string&);
    bool trigger_contact(bool inside,bool qualified,int module,std::string&);
    const std::string& trigger_key() const noexcept { return bank_.trigger_key; }
private:
    struct ActiveCommand {
        ActorId actor_id=invalid_actor_id;
        std::string source_object_name;
        int module=-1;
        std::shared_ptr<const void> actor_binding_lease;
        std::weak_ptr<const void> session_lease;
        const RetainedPosePlayback* pose_owner=nullptr;
        bool skipped=false;
    };
    OriginalCampaignRuntime& campaign_;
    CombatSession& session_;
    const TrollReturnSourceBankV1& bank_;
    TrollReturnRuntimeServicesV1 services_;
    std::weak_ptr<const void> session_lease_;
    std::map<const OriginalCampaignCommand*,ActiveCommand> active_;
    std::string construction_error_;
    bool validate_session(std::string&) const;
    bool validate_active(const ActiveCommand&,std::string&) const;
};

} // namespace dh::foundation
