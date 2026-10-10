#pragma once

#include "runtime_companion_session_v1.hpp"
#include "../navigation/source_commands.hpp"
#include "../../original_controller_commands.hpp"

namespace dh::foundation::companions {

struct RuntimeCompanionControllerGateV1 {
    std::uint32_t global_blocked = 0;
    std::uint32_t locked = 0;
    std::uint32_t forced = 0;
};

struct RuntimeCompanionWarpResultV1 {
    ActorId actor_id = invalid_actor_id;
    ActorId master_id = invalid_actor_id;
    std::array<float, 3> position{};
    bool update_destination = false;
    // Must be an output of the source WarpBehind owner. This feature does not
    // calculate a behind offset or reinterpret the master's transform.
    bool source_result_ready = false;
};

struct RuntimeCompanionMovementResultV1 {
    OriginalCommandAdmission admission = OriginalCommandAdmission::failed;
    bool command_dispatched = false;
    bool remote_noop = false;
    bool path_published = false;
    bool path_found = false;
    bool target_cleared = false;
    bool stop_dispatched = false;
    bool warp_applied = false;
};

// Executes only the authored follower commands against actors already owned
// by one CombatSession. PathTo reuses the caller's PlayableActorBodies PF and
// source path owner; Stop reuses the complete original Character control
// service; WarpBehind requires an actual source-produced destination.
class RuntimeCompanionMovementV1 final {
public:
    bool bind(CombatSession&, PlayableActorBodies&, std::string& error);

    bool execute(const RuntimeCompanionFollowDecisionV1&,
                 const dh2::character::ControllerCommandState32& source_gate,
                 bool remote_updated,
                 features::SourcePathCommandBindings* path_owner,
                 const dh2::character::CharacterControlServices16* stop_owner,
                 const RuntimeCompanionWarpResultV1* warp_result,
                 RuntimeCompanionMovementResultV1&, std::string& error);

private:
    CombatSession* session_ = nullptr;
    PlayableActorBodies* bodies_ = nullptr;
    std::weak_ptr<const void> session_lease_;

    bool validate_session(std::string& error) const;
};

} // namespace dh::foundation::companions
