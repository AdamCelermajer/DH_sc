#pragma once

#include "../../combat_session.hpp"
#include "../../original_campaign_runtime.hpp"
#include <map>
#include <memory>

namespace dh::foundation::companions {

struct CompanionCampaignActorPlanV1 {
    std::string source_object_name;
    std::string profile_id;
    const OriginalCombatVisualPlan* plan = nullptr;
};

struct CompanionCampaignAnimationNeedV1 {
    std::size_t command_index = 0;
    const OriginalCampaignCommand* command = nullptr;
    std::string source_object_name, profile_id, state, clip_name, unsupported_reason;
    CombatSessionChoice selection;
    std::uint32_t animation_id = 0, next_id = 0, slot = 0;
    bool wait = false, looping = false, resolved = false;
};

struct CompanionCampaignAnimationBankV1 {
    const OriginalCampaignRuntime* campaign = nullptr;
    int script_id = -1;
    std::string script_name;
    std::vector<CompanionCampaignAnimationNeedV1> commands;

    const CompanionCampaignAnimationNeedV1* find(const OriginalCampaignCommand&) const noexcept;
};

// Read-only composition against exact loaded Script_PlayActorAnim records and
// an already-built actor-profile plan. Only single-clip records with identical
// Anim/Next IDs and supported Idle/Attack slots can be composed. Unresolved
// records remain available to the existing native campaign command provider.
bool build_companion_campaign_animation_bank_v1(
    const OriginalMeleeBindings&, const OriginalCampaignRuntime&, const std::string& script_name,
    const std::vector<CompanionCampaignActorPlanV1>&,
    CompanionCampaignAnimationBankV1&, std::string& error);

struct CompanionCampaignActorBorrowV1 {
    CombatSession* session = nullptr;
    ActorId actor_id = invalid_actor_id;
    std::string source_object_name, profile_id;
    int module = -1;
    std::shared_ptr<const void> session_binding_lease;
};

struct CompanionCampaignAnimationServicesV1 {
    // Resolves the authored name/module to the ActorId already registered in
    // the SAME CombatSession. No actor construction or native Character needed.
    std::function<bool(const std::string&, const std::string&, int,
                       CombatSession&, CompanionCampaignActorBorrowV1&,
                       std::string&)> borrow_actor;
};

// Additive provider for the small source subset that maps exactly to clips in
// the same actor-profile Session. OriginalCampaignRuntime remains the only
// command scheduler; CombatSession remains the only pose clock.
class SourceCompanionCampaignAnimationV1 {
public:
    SourceCompanionCampaignAnimationV1(OriginalCampaignRuntime&, CombatSession&,
        const CompanionCampaignAnimationBankV1&, CompanionCampaignAnimationServicesV1);
    bool command(CampaignCommandPhase, const OriginalCampaignCommand&, int module,
                 bool skip, bool& handled, bool& blocking, std::string& error);
private:
    struct Active {
        ActorId actor_id = invalid_actor_id;
        std::string source_object_name, profile_id;
        int module = -1;
        bool skipped = false, wait = false;
        std::shared_ptr<const void> session_binding_lease;
        std::weak_ptr<const void> session_lease;
        const RetainedPosePlayback* pose_owner = nullptr;
    };
    OriginalCampaignRuntime& campaign_;
    CombatSession& session_;
    const CompanionCampaignAnimationBankV1& bank_;
    CompanionCampaignAnimationServicesV1 services_;
    std::weak_ptr<const void> session_lease_;
    std::map<const OriginalCampaignCommand*, Active> active_;
    std::string construction_error_;
    bool validate_session(std::string&) const;
    bool validate_active(const Active&, std::string&) const;
};

} // namespace dh::foundation::companions
