#include "runtime_companion_follow_consumer_v1.hpp"

namespace dh::foundation::companions {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool same_owner(const std::weak_ptr<const void>& left,
                const std::shared_ptr<const void>& right) {
    return !left.owner_before(right) && !right.owner_before(left);
}
} // namespace

bool RuntimeCompanionFollowConsumerV1::bind(
    CombatSession& session, RuntimeCompanionSessionV1& companion_session,
    RuntimeCompanionMovementV1& movement, std::string& error) {
    session_ = nullptr;
    companion_session_ = nullptr;
    movement_ = nullptr;
    session_lease_.reset();
    const auto lease = session.actor_binding_lease().lock();
    if (!lease) return fail(error, "Follow consumer requires an initialized CombatSession");
    if (!companion_session.bound_to(session) || !movement.bound_to(session))
        return fail(error, "Follow policy and movement owners must be bound to this same CombatSession");
    session_ = &session;
    companion_session_ = &companion_session;
    movement_ = &movement;
    session_lease_ = lease;
    error.clear();
    return true;
}

bool RuntimeCompanionFollowConsumerV1::validate_session(std::string& error) const {
    const auto expected = session_lease_.lock();
    const auto current = session_ ? session_->actor_binding_lease().lock() : nullptr;
    if (!session_ || !companion_session_ || !movement_ || !expected || !current ||
        expected != current || !same_owner(session_lease_, current) ||
        !companion_session_->bound_to(*session_) || !movement_->bound_to(*session_)) {
        return fail(error, "Follow consumer belongs to a stale or replaced CombatSession");
    }
    error.clear();
    return true;
}

bool RuntimeCompanionFollowConsumerV1::execute_event(
    const std::string& source_object_name, SourceFollowerEventV1 event,
    const RuntimeCompanionFollowFactsV1& source_facts,
    const dh2::character::ControllerCommandState32& source_gate,
    bool remote_updated, features::SourcePathCommandBindings* path_owner,
    const dh2::character::CharacterControlServices16* stop_owner,
    const RuntimeCompanionWarpResultV1* source_warp_result,
    RuntimeCompanionFollowDecisionV1& decision,
    RuntimeCompanionMovementResultV1& movement_result, std::string& error) {
    decision = {};
    movement_result = {};
    if (!validate_session(error)) return false;
    if (!companion_session_->plan_event(source_object_name, event, source_facts,
                                        decision, error)) return false;
    if (!validate_session(error)) return false;

    // Source callbacks which are unregistered, already have a master, or use
    // a separate AI script have no generic follower command to consume here.
    // In particular, Rene's catch-up/timer/buff logic stays with its owner.
    if (!decision.handled || !decision.source_policy_supported ||
        decision.operation_count == 0) {
        error.clear();
        return true;
    }
    for (std::size_t i = 0; i < decision.operation_count; ++i) {
        if (decision.operations[i] == SourceFollowerOperationV1::set_master)
            return fail(error, "SetMaster publication belongs to the authenticated source Character master-field owner");
    }
    if (!movement_->execute(decision, source_gate, remote_updated, path_owner,
                            stop_owner, source_warp_result, movement_result, error))
        return false;
    return validate_session(error);
}

} // namespace dh::foundation::companions
