#pragma once

#include "runtime_companion_movement_v1.hpp"

namespace dh::foundation::companions {

// Connects a reached source callback to the existing same-CombatSession
// policy and movement owners. Source master/state facts and command services
// remain borrowed from the actual caller; this class creates no scheduler or
// synthetic source state.
class RuntimeCompanionFollowConsumerV1 final {
public:
    bool bind(CombatSession&, RuntimeCompanionSessionV1&,
              RuntimeCompanionMovementV1&, std::string& error);

    bool execute_event(const std::string& source_object_name,
                       SourceFollowerEventV1 event,
                       const RuntimeCompanionFollowFactsV1& source_facts,
                       const dh2::character::ControllerCommandState32& source_gate,
                       bool remote_updated,
                       features::SourcePathCommandBindings* path_owner,
                       const dh2::character::CharacterControlServices16* stop_owner,
                       const RuntimeCompanionWarpResultV1* source_warp_result,
                       RuntimeCompanionFollowDecisionV1& decision,
                       RuntimeCompanionMovementResultV1& movement_result,
                       std::string& error);

private:
    CombatSession* session_ = nullptr;
    RuntimeCompanionSessionV1* companion_session_ = nullptr;
    RuntimeCompanionMovementV1* movement_ = nullptr;
    std::weak_ptr<const void> session_lease_;

    bool validate_session(std::string& error) const;
};

} // namespace dh::foundation::companions
