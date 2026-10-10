#pragma once

#include "runtime_companion_follow_v1.hpp"
#include "../../combat_session.hpp"

namespace dh::foundation::companions {

// Authored MGP/CharacterProperties/AI identity already resolved by the caller
// that enrolled actors in this CombatSession. This is an identity receipt, not
// a spawn request or a second actor registry.
struct RuntimeCompanionSessionActorV1 {
    std::string source_object_name;
    std::string profile_id;
    std::string ai_script;
    std::int32_t ai_row = -1;
    std::int32_t animation_table = -1;
    ActorId actor_id = invalid_actor_id;
};

// Same-session bridge for authored Faery follower callbacks. The caller must
// supply the source activation fact from the original campaign condition
// owner; it gates the Priest entry only. Faery has no MGP activation condition.
// This class neither evaluates RENE_FOLLOW nor schedules movement.
class RuntimeCompanionSessionV1 final {
public:
    bool bind(CombatSession&, const std::vector<RuntimeCompanionSessionActorV1>&,
              bool rene_follow_active, std::string& error);

    bool bound_to(const CombatSession&) const noexcept;

    bool resolve_actor(const std::string& source_object_name, ActorId&,
                       std::string& error) const;
    // The actual callback facts include the source master ActorId and the
    // source Faery state. Master identity is accepted only if that ActorId is
    // live in this exact CombatSession. Output remains a typed command plan
    // for the existing Character/controller navigation owner to consume.
    bool plan_event(const std::string& source_object_name, SourceFollowerEventV1,
                    const RuntimeCompanionFollowFactsV1&,
                    RuntimeCompanionFollowDecisionV1&, std::string& error) const;

private:
    CombatSession* session_ = nullptr;
    std::weak_ptr<const void> session_lease_;
    RuntimeCompanionFollowPlanV1 follow_plan_;

    bool validate_current_session(std::string& error) const;
};

} // namespace dh::foundation::companions
