#include "runtime_session_contact_v1.hpp"

#include <exception>
#include <stdexcept>

namespace dh::foundation::physics {
namespace {
constexpr std::uintptr_t default_begin = 0x3dbf08u;
constexpr std::uintptr_t default_persist = 0x3dbfa0u;
constexpr std::uintptr_t default_end = 0x3dbf40u;

struct ContactServiceFailure final : std::runtime_error {
    explicit ContactServiceFailure(const std::string& message) : std::runtime_error(message) {}
};

bool same_lease(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.expired() && !b.expired() &&
        !a.owner_before(b) && !b.owner_before(a);
}

struct Invocation {
    CombatSession* session{};
    RuntimeSessionContactServicesV1* services{};
    const RuntimeSessionContactSnapshotV1* snapshot{};
    RuntimeSessionContactAISFieldsV1* fields{};
    dh2::character::ScriptCollisionState72* state{};
    ActorId owner{}, peer{};
};

std::uint32_t invoke(void* opaque, dh2::character::ScriptCollisionState72*,
                     dh2::character::ScriptCollisionObject16*,
                     const dh2::character::ScriptCollisionRequest32* request) {
    auto& call = *static_cast<Invocation*>(opaque);
    if (!request) throw ContactServiceFailure("Missing source collision request");
    const auto* owner = call.session->actor(call.owner);
    const auto* peer = call.session->actor(call.peer);
    const auto* ownerTraits = call.session->world()->traits(call.owner);
    const auto* peerTraits = call.session->world()->traits(call.peer);
    if (!owner || !peer || !ownerTraits || !peerTraits)
        throw ContactServiceFailure("Source collision actor left the same live CombatSession");
    auto sync_after_service = [&]() {
        RuntimeSessionContactSnapshotV1 live{};
        std::string error;
        if (!call.services->snapshot || !call.services->snapshot(call.owner, live, error))
            throw ContactServiceFailure(error.empty() ?
                "Source collision callback could not refresh same-ActorState owners" : error);
        if (!live.character || !live.char_ai || !live.controller ||
            live.controller_owner != call.owner || live.paused > 255) {
            throw ContactServiceFailure("Source collision callback replaced its Session Character/controller owner");
        }
        call.state->collision_ms = call.fields->collision_ms;
        call.state->last_collision_frame = call.fields->last_collision_frame;
        const auto* refreshedOwner = call.session->actor(call.owner);
        if (!refreshedOwner)
            throw ContactServiceFailure("Source collision callback removed its same ActorState owner");
        call.state->current_target = refreshedOwner->target_id;
        call.state->preferred_target = live.preferred_target;
        call.state->paused = live.paused;
    };
    auto complete = [&](std::uint32_t value) {
        sync_after_service();
        return value;
    };

    switch (request->service) {
    case dh2::character::script_collision_is_character:
        if (request->subject != call.peer)
            throw ContactServiceFailure("Source IsCharacter request was not the registered physical peer");
        return complete(1); // Both IDs are current registered PlayableActorBodies actors.

    case dh2::character::script_collision_owner_is_player:
        if (request->subject != call.snapshot->character)
            throw ContactServiceFailure("Source owner IsPlayer request changed Character identity");
        return complete(ownerTraits->is_player ? 1u : 0u);

    case dh2::character::script_collision_is_enemy: {
        if (request->subject != call.snapshot->char_ai || request->target != call.peer)
            throw ContactServiceFailure("Source AI_IsEnemy request changed CharAI/peer identity");
        const auto* tables = call.session->original_ai_tables();
        const auto* ownerProperties = call.session->world()->combat_properties(call.owner);
        const auto* peerProperties = call.session->world()->combat_properties(call.peer);
        if (!tables || !ownerProperties || !peerProperties)
            throw ContactServiceFailure("Source AI faction/property table unavailable for IsEnemy");
        return complete(dh2::data::ai_enemy(*tables,
            ownerProperties->sheets.resolved[0], peerProperties->sheets.resolved[0],
            ownerTraits->is_player, peerTraits->is_player) ? 1u : 0u);
    }

    case dh2::character::script_collision_cancel_sneaking:
        if (request->subject != call.snapshot->character)
            throw ContactServiceFailure("Source CancelSneaking request changed Character identity");
        if (!call.services->cancel_sneaking)
            throw ContactServiceFailure("Required actual source CancelSneaking continuation");
        {
            std::string error;
            const bool succeeded = call.services->cancel_sneaking(call.owner, error);
            sync_after_service();
            if (!succeeded)
                throw ContactServiceFailure(error.empty() ?
                    "Actual source CancelSneaking continuation failed" : error);
        }
        return 0;

    case dh2::character::script_collision_set_target:
        if (request->subject != call.snapshot->char_ai || request->target != call.peer)
            throw ContactServiceFailure("Source AI_SetTarget request changed CharAI/peer identity");
        if (!call.services->set_target)
            throw ContactServiceFailure("Required actual same-Session source AI_SetTarget continuation");
        {
            std::string error;
            const bool succeeded = call.services->set_target(call.owner, call.peer, request->arg0, error);
            sync_after_service();
            if (!succeeded)
                throw ContactServiceFailure(error.empty() ?
                    "Actual same-Session source AI_SetTarget continuation failed" : error);
            const auto* updated = call.session->actor(call.owner);
            if (!updated || updated->target_id != call.peer)
                throw ContactServiceFailure("Accepted source AI_SetTarget did not publish through same ActorState.target_id");
        }
        return 0;
    }
    throw ContactServiceFailure("Unknown source AIS collision service");
}

} // namespace

RuntimeSessionContactOwnerV1::RuntimeSessionContactOwnerV1(
    CombatSession& session, RuntimeSessionContactServicesV1 services)
    : session_(session), lifetime_(session.lifetime_lease()),
      lease_(session.actor_binding_lease()), services_(std::move(services)) {}

bool RuntimeSessionContactOwnerV1::owns_binding() const noexcept {
    // The actor-binding token can be pinned independently of CombatSession.
    // Prove the owning Session object still lives before reading through the
    // stored reference to ask for its current actor-binding generation.
    const auto lifetime = lifetime_.lock();
    if (!lifetime || !lifetime->alive()) return false;
    return same_lease(lease_, session_.actor_binding_lease());
}

bool RuntimeSessionContactOwnerV1::validate_actor(ActorId id, std::string& error) const {
    if (!owns_binding()) {
        error = "Source contact owner belongs to a stale/detached CombatSession binding";
        return false;
    }
    if (!id || !session_.actor(id) || !session_.world() ||
        !session_.world()->traits(id) || !session_.world()->combat_properties(id)) {
        error = "Source contact ActorId is not registered in the current CombatSession";
        return false;
    }
    error.clear();
    return true;
}

bool RuntimeSessionContactOwnerV1::bind_body(ActorId id, std::string& error) {
    if (!validate_actor(id, error)) return false;
    if (ais_fields_.count(id)) {
        error = "Duplicate same-Session physical body registration";
        return false;
    }
    if (!services_.snapshot) {
        error = "Required actual source frame/FSM/AIS/controller snapshot provider";
        return false;
    }
    RuntimeSessionContactSnapshotV1 snapshot{};
    if (!services_.snapshot(id, snapshot, error)) {
        if (error.empty()) error = "Actual source contact snapshot provider failed";
        return false;
    }
    if (!snapshot.character || !snapshot.char_ai || !snapshot.controller ||
        snapshot.controller_owner != id || snapshot.paused > 255) {
        error = "Contact snapshot lacks coherent same-ActorState Character/CharAI/controller ownership";
        return false;
    }
    ais_fields_.emplace(id, RuntimeSessionContactAISFieldsV1{});
    error.clear();
    return true;
}

bool RuntimeSessionContactOwnerV1::unbind_body(ActorId id, std::string& error) {
    if (!owns_binding()) {
        error = "Cannot unregister a body through a stale CombatSession binding";
        return false;
    }
    if (!ais_fields_.erase(id)) {
        error = "Same-Session physical body registration is absent";
        return false;
    }
    error.clear();
    return true;
}

bool RuntimeSessionContactOwnerV1::physical_event(
    ActorId ownerId, RuntimeSessionContactEventV1 event, ActorId peerId,
    std::uint32_t sourceSide, std::string& error) {
    error.clear();
    if (!validate_actor(ownerId, error)) return false;
    const auto ownerFields = ais_fields_.find(ownerId);
    if (ownerFields == ais_fields_.end()) {
        error = "POCharacter contact owner is not registered in this body pool";
        return false;
    }
    if (event == RuntimeSessionContactEventV1::result) {
        // POCharacter::onCollisionResults is the exact source empty body.
        return true;
    }
    if (event != RuntimeSessionContactEventV1::begin &&
        event != RuntimeSessionContactEventV1::persist &&
        event != RuntimeSessionContactEventV1::end) {
        error = "Unknown source POCharacter contact event";
        return false;
    }
    if (!validate_actor(peerId, error)) return false;
    if (!ais_fields_.count(peerId)) {
        error = "Physical peer ActorId is not registered in the same PlayableActorBodies owner";
        return false;
    }
    if (!services_.snapshot) {
        error = "Required actual source frame/FSM/AIS/controller snapshot provider";
        return false;
    }
    RuntimeSessionContactSnapshotV1 snapshot{};
    if (!services_.snapshot(ownerId, snapshot, error)) {
        if (error.empty()) error = "Actual source contact snapshot provider failed";
        return false;
    }
    if (!snapshot.character || !snapshot.char_ai || !snapshot.controller ||
        snapshot.controller_owner != ownerId || snapshot.paused > 255) {
        error = "Contact callback lost its same-ActorState Character/CharAI/controller owner";
        return false;
    }
    auto& fields = ownerFields->second;
    const auto slotBegin = snapshot.collision_methods[0];
    const auto slotPersist = snapshot.collision_methods[1];
    const auto slotEnd = snapshot.collision_methods[2];
    auto* owner = session_.actor(ownerId);
    if (!owner) {
        error = "Contact owner left the CombatSession during callback preparation";
        return false;
    }
    dh2::character::ScriptCollisionState72 state{
        snapshot.active_ais, snapshot.character, snapshot.char_ai,
        snapshot.controller, owner->target_id, snapshot.preferred_target,
        fields.collision_ms, fields.last_collision_frame, snapshot.paused,
        snapshot.movement_state, snapshot.application_frame, snapshot.dt_ms};
    dh2::character::ScriptCollisionObject16 object{peerId, 0, 0};
    Invocation context{&session_, &services_, &snapshot, &fields, &state, ownerId, peerId};
    const dh2::character::ScriptCollisionServices16 services{&context, invoke};

    try {
        // POCharacter begins/ends run Debug before handle→Character; persists
        // resolve the handle first and then load/query Debug. In this typed
        // Session API the live body registration above is that handle result.
        if (event != RuntimeSessionContactEventV1::persist &&
            (!services_.debug_load || !services_.debug_switch)) {
            error = "Required original POCharacter collision Debug prefix";
            return false;
        }
        bool tracing = false;
        const auto debug = [&]() {
            if (!services_.debug_load || !services_.debug_switch) {
                error = "Required original POCharacter collision Debug prefix";
                return false;
            }
            if (!services_.debug_load(error) ||
                !services_.debug_switch("isTracingPlayersCollision", tracing, error)) {
                if (error.empty()) error = "Original POCharacter collision Debug prefix failed";
                return false;
            }
            if (tracing) {
                const auto* traits = session_.world()->traits(ownerId);
                if (!traits) { error = "Tracing collision IsPlayer owner left Session"; return false; }
                (void)traits->is_player; // Source query executes; this profile is its exact result.
            }
            return true;
        };
        if (event == RuntimeSessionContactEventV1::persist) {
            // The registered, live peer above is the typed equivalent of the
            // source Handle→Character conversion, which precedes Debug on Persist.
            if (!debug()) return false;
        } else if (!debug()) return false;

        if (!snapshot.active_ais) return true; // CharAI has no active AIS callback.
        if (event == RuntimeSessionContactEventV1::begin) {
            if (slotBegin != default_begin) {
                error = "Unsupported source AIS OnCollisionBegin override";
                return false;
            }
            ++collisions_; // Exact AISDefault Begin prefix, before its virtual Persist call.
            if (slotPersist != default_persist) {
                error = "Source AIS Begin reached an unsupported OnCollisionPersist override";
                return false; // Preserve the already-applied source common-counter prefix.
            }
        } else if (event == RuntimeSessionContactEventV1::persist) {
            if (slotPersist != default_persist) {
                error = "Unsupported source AIS OnCollisionPersist override";
                return false;
            }
        } else if (event == RuntimeSessionContactEventV1::end) {
            if (slotEnd != default_end) {
                error = "Unsupported source AIS OnCollisionEnd override";
                return false;
            }
            --collisions_; // Source unsigned wrap is retained; no pair dedup/clamp.
            return true;
        }

        const auto sourcePersist = event == RuntimeSessionContactEventV1::begin ? sourceSide :
            event == RuntimeSessionContactEventV1::persist ? sourceSide : 0u;
        const auto status = dh2_character_script_collision(
            &state, &object, sourcePersist, &services);
        if (status < 0) {
            error = "Source AISDefault collision kernel rejected malformed live owner fields";
            return false;
        }
        fields.collision_ms = state.collision_ms;
        fields.last_collision_frame = state.last_collision_frame;
        if (status != 1) {
            error = "Source AISDefault collision continuation rejected its current movement state";
            return false;
        }
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
    return true;
}

bool RuntimeSessionContactOwnerV1::process_collision_pause_threshold(
    ActorId id, const CollisionPauseContinuation& pauseTimerStop,
    bool& triggered, std::string& error) {
    triggered = false;
    if (!validate_actor(id, error)) return false;
    auto fields = ais_fields_.find(id);
    if (fields == ais_fields_.end()) {
        error = "AIS collision-time update owner is not registered in this Session contact pool";
        return false;
    }
    if (fields->second.collision_ms <= 199u) {
        error.clear();
        return true;
    }
    if (!pauseTimerStop) {
        error = "AIS collision-time threshold requires actual pause/timer/Stop continuation";
        return false;
    }
    // The source AI_PauseUpdate/Timer/Stop services execute before the source
    // counter reset point. Failure preserves their actual prefix and leaves
    // the collision counter untouched; callers must not replay/rollback it.
    if (!pauseTimerStop(id, error)) {
        if (error.empty()) error = "AIS collision-time pause/timer/Stop continuation failed";
        return false;
    }
    if (!owns_binding()) {
        error = "CombatSession became stale during AIS collision-time continuation";
        return false;
    }
    fields = ais_fields_.find(id);
    if (fields == ais_fields_.end()) {
        error = "AIS collision-time counter owner disappeared during continuation";
        return false;
    }
    fields->second.collision_ms = 0;
    triggered = true;
    error.clear();
    return true;
}

const RuntimeSessionContactAISFieldsV1* RuntimeSessionContactOwnerV1::ais_fields(
    ActorId id) const noexcept {
    const auto found = ais_fields_.find(id);
    return found == ais_fields_.end() ? nullptr : &found->second;
}

} // namespace dh::foundation::physics
