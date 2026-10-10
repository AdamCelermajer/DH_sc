#include "runtime_combat_effects_v1.hpp"

#include <limits>
#include <utility>

namespace dh::foundation::effects {
namespace {
bool same_owner(const std::weak_ptr<const void>& left,
                const std::weak_ptr<const void>& right) noexcept {
    return !left.owner_before(right) && !right.owner_before(left);
}
bool fail(std::string& error, const char* reason) {
    error = reason;
    return false;
}
}

RuntimeCombatEffectsV1::RuntimeCombatEffectsV1(CombatSession& session,
    dh2::fx::CharacterMeshFxOwnerV4& manager, RuntimeCombatEffectsBindingsV1 bindings)
    : session_(session), manager_(manager), render_services_(std::move(bindings.render)),
      binding_lease_(session.actor_binding_lease()) {
    events_ = std::make_unique<RetainedEffectsAdapter>(manager_, RetainedEffectBindings{
        [this](std::uintptr_t raw_actor, ActorAttachment& output, std::string& error) {
            const auto id = static_cast<ActorId>(raw_actor);
            if (static_cast<std::uintptr_t>(id) != raw_actor)
                return fail(error, "FX actor identity does not fit CombatSession ActorId");
            const RetainedPosePlayback* pose{};
            const CharacterVisual* visual{};
            const ActorState* state{};
            if (!current_actor(id, pose, visual, state, error)) return false;
            (void)pose;
            (void)visual;
            output.actor = static_cast<std::uintptr_t>(id);
            output.socket = 0; // Original CharAI::_OnAnimEvent fx_ path passes NULL anchor.
            for (unsigned axis = 0; axis != 3; ++axis) {
                output.target_position[axis] = state->transform.position[axis];
                output.source_rotation[axis] = state->transform.rotation[axis];
            }
            if (output.actor != static_cast<std::uintptr_t>(id))
                return fail(error, "FX attachment changed the stable CombatSession ActorId");
            actors_seen_.insert(id);
            error.clear();
            return true;
        }});
    renderer_ = std::make_unique<EffectsRenderBridge>(manager_, render_services_);
}

bool RuntimeCombatEffectsV1::current_actor(ActorId actor,
    const RetainedPosePlayback*& pose, const CharacterVisual*& visual,
    const ActorState*& state, std::string& error) const {
    pose = nullptr;
    visual = nullptr;
    state = nullptr;
    const auto current_lease = session_.actor_binding_lease();
    if (current_lease.expired() || !same_owner(current_lease, binding_lease_))
        return fail(error, "FX requires the current CombatSession binding lease");
    pose = session_.retained_actor_pose(actor);
    if (!pose) return fail(error, "FX requires the current CombatSession retained actor pose");
    state = session_.actor(actor);
    if (!state)
        return fail(error, "FX actor is absent from the same CombatSession");
    visual = session_.retained_actor_visual_borrow(actor);
    if (!visual) return fail(error, "Required same-session CombatSession Entry.visual borrow");
    if (!visual->loaded() || !visual->retained_scene_borrow())
        return fail(error, "Same-session retained CharacterVisual has no live source Scene");
    error.clear();
    return true;
}

DispatchResult RuntimeCombatEffectsV1::event(ActorId actor,
    const RetainedAnimationEvent& event, std::uint64_t ordinal, std::string& error) {
    error.clear();
    if (event.name.compare(0, 3, "fx_") != 0) return DispatchResult::ignored;
    if (!actor || static_cast<ActorId>(static_cast<std::uintptr_t>(actor)) != actor) {
        error = "Invalid or unrepresentable stable FX ActorId";
        return DispatchResult::required_failure;
    }
    if (!synchronize_session_binding(error)) return DispatchResult::required_failure;
    const RetainedPosePlayback* pose{};
    const CharacterVisual* visual{};
    const ActorState* state{};
    if (!current_actor(actor, pose, visual, state, error)) return DispatchResult::required_failure;
    (void)pose;
    (void)visual;
    (void)state;
    actors_seen_.insert(actor);
    return events_->event(static_cast<std::uintptr_t>(actor), event, ordinal, error);
}

bool RuntimeCombatEffectsV1::update(std::uint64_t frame,
    std::int32_t absolute_ms, std::int32_t app_dt, std::string& error) {
    error.clear();
    if (!synchronize_session_binding(error)) return false;
    if (!frame || (update_consumed_ && frame <= last_source_frame_))
        return fail(error, "FX manager update requires a fresh source frame token");
    // Consume before source callbacks: a reached manager prefix is not replayed.
    update_consumed_ = true;
    last_source_frame_ = frame;
    auto& executor = events_->executor();
    if (!executor.scene_phase(absolute_ms, app_dt, error)) return false;
    if (!executor.manager_phase(app_dt, error)) return false;
    return true;
}

void RuntimeCombatEffectsV1::prune_frame_loans() const {
    for (auto it = frame_loans_.begin(); it != frame_loans_.end();) {
        if (it->expired()) it = frame_loans_.erase(it);
        else ++it;
    }
}

bool RuntimeCombatEffectsV1::prepare_render_frame(
    std::shared_ptr<const EffectRenderFrame>& output, std::string& error) {
    output.reset();
    if (!synchronize_session_binding(error)) return false;
    if (!renderer_->prepare(output, error)) return false;
    frame_loans_.push_back(output);
    return true;
}

bool RuntimeCombatEffectsV1::socket_world(ActorId actor, const std::string& name,
    const Mat4& local_offset, Mat4& output, std::string& error) {
    if (!synchronize_session_binding(error)) return false;
    const RetainedPosePlayback* pose{};
    const CharacterVisual* visual{};
    const ActorState* state{};
    if (!current_actor(actor, pose, visual, state, error)) return false;
    (void)pose;
    (void)state;
    return visual->socket_world(name, local_offset, output, error);
}

bool RuntimeCombatEffectsV1::reset_for_renewed_binding(
    const std::weak_ptr<const void>& lease, std::string& error) {
    prune_frame_loans();
    if (!frame_loans_.empty())
        return fail(error, "Drain root FX render-frame loans before renewing the actor binding");
    for (ActorId actor : actors_seen_) events_->release_actor(static_cast<std::uintptr_t>(actor));
    actors_seen_.clear();
    binding_lease_ = lease;
    update_consumed_ = false;
    last_source_frame_ = 0;
    error.clear();
    return true;
}

bool RuntimeCombatEffectsV1::synchronize_session_binding(std::string& error) {
    const auto current = session_.actor_binding_lease();
    if (same_owner(current, binding_lease_) && !current.expired()) {
        error.clear();
        return true;
    }
    if (!reset_for_renewed_binding(current, error)) return false;
    if (current.expired()) return fail(error, "CombatSession actor binding is detached or expired");
    return true;
}

void RuntimeCombatEffectsV1::release_actor(ActorId actor) noexcept {
    if (!actor || static_cast<ActorId>(static_cast<std::uintptr_t>(actor)) != actor) return;
    if (events_) events_->release_actor(static_cast<std::uintptr_t>(actor));
    actors_seen_.erase(actor);
}

} // namespace dh::foundation::effects
