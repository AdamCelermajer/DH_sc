#include "runtime_companion_movement_v1.hpp"

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

struct MoveContext {
    CombatSession* session = nullptr;
    PlayableActorBodies* bodies = nullptr;
    ActorId owner = invalid_actor_id;
    ActorId master = invalid_actor_id;
    bool remote_updated = false;
    features::SourcePathCommandBindings* path = nullptr;
    RuntimeCompanionMovementResultV1* result = nullptr;
    std::string* error = nullptr;
};

int invoke_move_control(void* raw,
                        const dh2::character::CharacterControlRequest32* request,
                        dh2::character::CharacterControlResponse16* response) {
    auto& context = *static_cast<MoveContext*>(raw);
    using namespace dh2::character;
    if (!request || !response || !context.session || !context.bodies ||
        !context.result || !context.error) return -1;
    *response = {};
    if (request->subject != context.owner && request->service != control_target_position) {
        *context.error = "Companion source command owner differs from its Session ActorId";
        return -1;
    }
    switch (request->service) {
    case control_is_remotely_updated:
        response->word = context.remote_updated ? 1u : 0u;
        context.result->remote_noop = context.remote_updated;
        return 1;
    case control_target_position: {
        if (request->subject != context.master) {
            *context.error = "MoveTo target handle differs from the reached same-Session master ActorId";
            return -1;
        }
        const auto* master = context.session->actor(context.master);
        if (!master) {
            *context.error = "MoveTo master ActorId became stale before target-position read";
            return -1;
        }
        for (unsigned i = 0; i < 3; ++i) response->position[i] = master->transform.position[i];
        return 1;
    }
    case control_path_to: {
        if (request->subject != context.owner) {
            *context.error = "PathTo receiver differs from the same-Session companion ActorId";
            return -1;
        }
        // The original controller wrapper reaches this only after the source
        // admission gate, remote-update query, target validation and position
        // read. Keep the PF/path-owner requirement at this reached service so
        // blocked and remote no-op commands do not require navigation state.
        if (!context.path || !context.path->controller || !context.path->result ||
            !context.path->controller->controller_lease ||
            !context.path->controller->controller || !context.path->controller->path) {
            *context.error = "Reached PathTo requires the same actor's existing PF/path command owner";
            return -1;
        }
        auto* actor = context.session->actor(context.owner);
        OriginalTriggerActorBorrow body{};
        if (!actor || !context.bodies->actor_borrow(context.owner, body, *context.error)) return -1;
        const auto* navigation = context.bodies->navigation(context.owner);
        if (body.identity != context.owner || body.position160 != actor->transform.position.data() ||
            !navigation || navigation->user != context.owner) {
            *context.error = "MoveTo PF/body owner is not the same live CombatSession ActorId";
            return -1;
        }
        const Vec3 destination{request->position[0], request->position[1], request->position[2]};
        if (!features::find_source_destination(*context.bodies, context.owner, destination,
                                               *context.path, *context.error)) return -1;
        context.result->path_published = true;
        context.result->path_found = context.path->result && context.path->result->found != 0;
        return 1;
    }
    default:
        *context.error = "MoveTo reached an unsupported Character control service";
        return -1;
    }
}

struct ControllerContext {
    MoveContext* move = nullptr;
};

int invoke_move_controller(void* raw,
                           const dh2::character::ControllerCommandRequest24* request) {
    auto& context = *static_cast<ControllerContext*>(raw);
    if (!request || !context.move || request->command != dh2::character::controller_move_object ||
        request->owner != context.move->owner || request->target != context.move->master) {
        if (context.move && context.move->error)
            *context.move->error = "Companion controller reached an unexpected MoveTo request";
        return -1;
    }
    context.move->result->command_dispatched = true;
    const dh2::character::CharacterControlServices16 services{context.move, invoke_move_control};
    return dh2_character_control(request->owner, request->command, request->target, &services);
}

bool clear_target(CombatSession& session, ActorId actor, RuntimeCompanionMovementResultV1& result,
                  std::string& error) {
    auto* current = session.actor(actor);
    if (!current) return fail(error, "ClearTarget actor became stale before source target clear");
    current->target_id = invalid_actor_id;
    result.target_cleared = true;
    error.clear();
    return true;
}
}

bool RuntimeCompanionMovementV1::bind(CombatSession& session, PlayableActorBodies& bodies,
                                      std::string& error) {
    session_ = nullptr;
    bodies_ = nullptr;
    session_lease_.reset();
    auto lease = session.actor_binding_lease().lock();
    if (!lease) return fail(error, "Companion movement requires an initialized CombatSession");
    session_ = &session;
    bodies_ = &bodies;
    session_lease_ = lease;
    error.clear();
    return true;
}

bool RuntimeCompanionMovementV1::bound_to(const CombatSession& session) const noexcept {
    const auto expected = session_lease_.lock();
    const auto current = session.actor_binding_lease().lock();
    return session_ == &session && bodies_ && expected && current && expected == current &&
        same_owner(session_lease_, current);
}

bool RuntimeCompanionMovementV1::validate_session(std::string& error) const {
    const auto expected = session_lease_.lock();
    const auto current = session_ ? session_->actor_binding_lease().lock() : nullptr;
    if (!session_ || !bodies_ || !expected || !current || expected != current ||
        !same_owner(session_lease_, current))
        return fail(error, "Companion movement belongs to a stale or replaced CombatSession");
    error.clear();
    return true;
}

bool RuntimeCompanionMovementV1::execute(
    const RuntimeCompanionFollowDecisionV1& decision,
    const dh2::character::ControllerCommandState32& source_gate,
    bool remote_updated, features::SourcePathCommandBindings* path_owner,
    const dh2::character::CharacterControlServices16* stop_owner,
    const RuntimeCompanionWarpResultV1* warp_result,
    RuntimeCompanionMovementResultV1& output, std::string& error) {
    output = {};
    if (!validate_session(error)) return false;
    if (!decision.handled || !decision.source_policy_supported ||
        decision.actor_id == invalid_actor_id || !session_->actor(decision.actor_id))
        return fail(error, "Companion movement requires a supported reached event for a live Session ActorId");
    const auto expected = session_lease_.lock();
    if (!expected) return fail(error, "Companion Session lease expired before command dispatch");

    for (std::size_t i = 0; i < decision.operation_count; ++i) {
        const auto operation = decision.operations[i];
        const ActorId argument = decision.arguments[i];
        if (operation == SourceFollowerOperationV1::move_to_master) {
            if (argument == invalid_actor_id || !session_->actor(argument))
                return fail(error, "Reached MoveTo has no live same-Session master ActorId");
            if (source_gate.owner != decision.actor_id || source_gate.controller == 0 ||
                source_gate.reserved || source_gate.global_blocked > 255 ||
                source_gate.locked > 255 || source_gate.forced > 255)
                return fail(error, "MoveTo requires fresh source controller gate facts for this Session ActorId");

            MoveContext move{session_, bodies_, decision.actor_id, argument, remote_updated,
                             path_owner, &output, &error};
            ControllerContext controller{&move};
            const dh2::character::ControllerCommandServices16 services{&controller, invoke_move_controller};
            const int status = dh2_character_controller_command(&source_gate,
                dh2::character::controller_move_object, argument, &services);
            if (status != 1) {
                if (error.empty()) error = "Original MoveTo controller command failed at a reached service";
                return false;
            }
            output.admission = output.command_dispatched ? OriginalCommandAdmission::admitted :
                OriginalCommandAdmission::blocked;
            // follower.luac continues to ClearTarget after both admitted and
            // source-blocked MoveTo; a reached service failure returns above.
        } else if (operation == SourceFollowerOperationV1::clear_target) {
            if (!clear_target(*session_, decision.actor_id, output, error)) return false;
        } else if (operation == SourceFollowerOperationV1::stop) {
            if (source_gate.owner != decision.actor_id || source_gate.controller == 0 ||
                source_gate.reserved || source_gate.global_blocked > 255 ||
                source_gate.locked > 255 || source_gate.forced > 255)
                return fail(error, "Stop requires fresh source controller gate facts for this Session ActorId");
            const int status = dh2_character_controller_character(&source_gate,
                dh2::character::controller_stop, 0, stop_owner);
            if (status != 1) {
                if (error.empty()) error = "Original Stop failed at a reached same-owner service";
                return false;
            }
            output.admission = (source_gate.forced ||
                (!source_gate.global_blocked && !source_gate.locked)) ?
                OriginalCommandAdmission::admitted : OriginalCommandAdmission::blocked;
            output.stop_dispatched = output.admission == OriginalCommandAdmission::admitted;
        } else if (operation == SourceFollowerOperationV1::warp_behind_master) {
            if (argument == invalid_actor_id || !session_->actor(argument))
                return fail(error, "Reached WarpBehind has no live same-Session master ActorId");
            if (!warp_result || !warp_result->source_result_ready ||
                warp_result->actor_id != decision.actor_id || warp_result->master_id != argument)
                return fail(error, "WarpBehind requires the actual same-owner source-produced destination");
            OriginalTriggerActorBorrow body{};
            if (!bodies_->actor_borrow(decision.actor_id, body, error)) return false;
            if (body.position160 != session_->actor(decision.actor_id)->transform.position.data())
                return fail(error, "WarpBehind body owner differs from the same CombatSession actor");
            if (!bodies_->set_position(decision.actor_id, warp_result->position,
                                       warp_result->update_destination, error)) return false;
            output.warp_applied = true;
        } else {
            return fail(error, "Source follower operation is not executable by this movement consumer");
        }
        if (!validate_session(error)) return false;
    }
    error.clear();
    return true;
}
} // namespace dh::foundation::companions
