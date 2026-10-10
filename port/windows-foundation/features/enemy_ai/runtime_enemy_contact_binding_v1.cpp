#include "runtime_enemy_contact_binding_v1.hpp"
#include "runtime_enemy_controller_v1.hpp"

#include <stdexcept>

namespace dh::foundation::enemy_ai {
namespace {
constexpr std::uintptr_t default_begin = 0x3dbf08u;
constexpr std::uintptr_t default_persist = 0x3dbfa0u;
constexpr std::uintptr_t default_end = 0x3dbf40u;
constexpr std::uintptr_t default_result = 0x3dbf68u;

bool same_lease(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) noexcept {
    return !a.expired() && !b.expired() && !a.owner_before(b) && !b.owner_before(a);
}
}

struct RuntimeEnemyContactBindingV1::Impl {
    CombatSession* session{};
    std::weak_ptr<const CombatSessionLifetime> lifetime;
    std::weak_ptr<const void> lease;
    RuntimeEnemyControllerV1* enemy_controller{};
    RuntimeEnemyContactProjectionServicesV1 source;

    Impl(CombatSession& owner, RuntimeEnemyContactProjectionServicesV1 services,
         RuntimeEnemyControllerV1* controller = nullptr)
        : session(&owner), lifetime(owner.lifetime_lease()),
          lease(owner.actor_binding_lease()), enemy_controller(controller),
          source(std::move(services)) {}

    bool current(std::string& error) const {
        // An actor lease may be held after the Session itself is destroyed.
        // Only query the raw Session pointer after the independent lifetime
        // witness has been locked and checked.
        const auto lifetimeWitness = lifetime.lock();
        if (!session || !lifetimeWitness || !lifetimeWitness->alive()) {
            error = "Enemy contact projection belongs to a destroyed CombatSession";
            return false;
        }
        if (lease.expired() || !same_lease(lease, session->actor_binding_lease())) {
            error = "Enemy contact projection belongs to a stale or detached CombatSession";
            return false;
        }
        error.clear();
        return true;
    }

    bool project(ActorId id, physics::RuntimeSessionContactSnapshotV1& out,
                 std::string& error) const {
        if (!current(error)) return false;
        ActorState* actor = session->actor(id);
        auto* world = session->world();
        const auto* traits = world ? world->traits(id) : nullptr;
        const auto* properties = world ? world->combat_properties(id) : nullptr;
        const auto* tables = session->original_ai_tables();
        if (!actor || !traits || !properties || !tables) {
            error = "Enemy contact projection requires the current same-Session actor, traits, properties, and AI tables";
            return false;
        }
        if (!source.controller) {
            error = "Required actual current controller/frame/pause/AIS projection provider";
            return false;
        }
        RuntimeEnemyContactControllerProjectionV1 controller{};
        if (!source.controller(id, controller, error)) {
            if (error.empty()) error = "Current enemy controller projection failed";
            return false;
        }
        if (!controller.controller_projection || !controller.preferred_target_known ||
            controller.paused > 255) {
            // The source preferred target is a separate CharAI field. Reusing
            // target_id here would collapse two source authorities.
            error = "Enemy contact projection lacks a live controller, byte pause, or source preferred-target value";
            return false;
        }
        // The same-world facts are refreshed by CombatSession's ordinary
        // update/facts phase and are the source FSM-state projection. Do not
        // derive the ABI state from generic CharacterAction.
        const std::int32_t movement = properties->facts.original_state;
        if (movement < 0) {
            error = "Enemy contact projection has no current source FSM movement state";
            return false;
        }

        const auto* ai = dh2::data::ai_props(*tables, properties->sheets.resolved[1]);
        if (controller.ais_policy == RuntimeEnemyContactAISPolicyV1::unknown) {
            error = "Enemy contact projection has unknown selected AIS collision policy";
            return false;
        }
        if (controller.ais_policy == RuntimeEnemyContactAISPolicyV1::inherited_default && !ai) {
            error = "Inherited AISDefault collision policy requires this actor's resolved original AI row";
            return false;
        }
        if (controller.ais_policy != RuntimeEnemyContactAISPolicyV1::absent &&
            controller.ais_policy != RuntimeEnemyContactAISPolicyV1::inherited_default) {
            error = "Unsupported selected AIS collision policy";
            return false;
        }

        // These identity tokens are actual retained modern projections: the
        // Session ActorState, its same-world traits, and caller's current
        // controller/selected-AIS projection. The source kernel compares
        // identity only; it never dereferences native object layouts.
        out = {};
        out.character = reinterpret_cast<std::uintptr_t>(actor);
        out.char_ai = reinterpret_cast<std::uintptr_t>(traits);
        out.controller = reinterpret_cast<std::uintptr_t>(controller.controller_projection);
        out.controller_owner = id;
        out.preferred_target = controller.preferred_target;
        out.movement_state = static_cast<std::uint32_t>(movement);
        out.paused = controller.paused;
        out.application_frame = controller.application_frame;
        out.dt_ms = controller.dt_ms;
        if (controller.ais_policy == RuntimeEnemyContactAISPolicyV1::inherited_default) {
            out.active_ais = reinterpret_cast<std::uintptr_t>(controller.controller_projection);
            out.collision_methods = {default_begin, default_persist, default_end, default_result};
        }
        error.clear();
        return true;
    }

    bool set_target(ActorId owner, ActorId target, std::uint32_t flags,
                    std::string& error) const {
        if (!current(error)) return false;
        if (flags != 0) {
            error = "Source AISDefault contact SetTarget flags must be zero";
            return false;
        }
        auto* world = session->world();
        ActorState* source_actor = session->actor(owner);
        ActorState* target_actor = session->actor(target);
        if (!world || !source_actor || !target_actor || !world->traits(owner) ||
            !world->traits(target) || !world->combat_properties(owner) ||
            !world->combat_properties(target)) {
            error = "Contact SetTarget requires both actors in this current CombatSession";
            return false;
        }
        if (enemy_controller)
            return enemy_controller->set_contact_target(*session, owner, target, flags, error);
        // This modern adapter reproduces only the current-target effect that
        // the enemy controller needs. Source AI_SetTarget also writes the
        // preferred target and performs owner/debug/last-target/alive/sight
        // work; those source-owned fields and providers are not projected by
        // this CombatSession. Do not mistake this assignment for full setter
        // parity. No parallel target state or synthetic event is created.
        source_actor->target_id = target;
        error.clear();
        return true;
    }
};

RuntimeEnemyContactBindingV1::RuntimeEnemyContactBindingV1(
    CombatSession& session, RuntimeEnemyContactProjectionServicesV1 services)
    : impl_(std::make_shared<Impl>(session, std::move(services))) {
    if (impl_->lease.expired())
        throw std::invalid_argument("Enemy contact binding requires an active CombatSession");
}

RuntimeEnemyContactBindingV1::RuntimeEnemyContactBindingV1(
    CombatSession& session, RuntimeEnemyControllerV1& controller,
    RuntimeEnemyContactProjectionServicesV1 services) {
    if (!services.selected_ais_policy)
        throw std::invalid_argument("Controller-owned contact composition requires explicit selected-AIS policy");
    const auto selectedPolicy = services.selected_ais_policy;
    services.controller = [&session, &controller, selectedPolicy](
        ActorId id, RuntimeEnemyContactControllerProjectionV1& out,
        std::string& error) {
        RuntimeEnemyContactAISPolicyV1 policy = RuntimeEnemyContactAISPolicyV1::unknown;
        if (!selectedPolicy(id, policy, error)) {
            if (error.empty()) error = "Selected AIS policy provider failed";
            return false;
        }
        return controller.project_contact(session, id, policy, out, error);
    };
    impl_ = std::make_shared<Impl>(session, std::move(services), &controller);
    if (impl_->lease.expired())
        throw std::invalid_argument("Enemy contact binding requires an active CombatSession");
}

physics::RuntimeSessionContactServicesV1 RuntimeEnemyContactBindingV1::services() const {
    const auto impl = impl_;
    physics::RuntimeSessionContactServicesV1 out;
    out.snapshot = [impl](ActorId id, physics::RuntimeSessionContactSnapshotV1& snapshot,
                          std::string& error) {
        return impl->project(id, snapshot, error);
    };
    out.debug_load = [impl](std::string& error) {
        if (!impl->current(error)) return false;
        if (!impl->source.debug_load) {
            error = "Required actual POCharacter collision Debug::Load provider";
            return false;
        }
        return impl->source.debug_load(error);
    };
    out.debug_switch = [impl](const char* key, bool& value, std::string& error) {
        if (!impl->current(error)) return false;
        if (!impl->source.debug_switch) {
            error = "Required actual POCharacter isTracingPlayersCollision Debug provider";
            return false;
        }
        return impl->source.debug_switch(key, value, error);
    };
    out.cancel_sneaking = [impl](ActorId id, std::string& error) {
        if (!impl->current(error)) return false;
        if (!impl->session->actor(id)) {
            error = "CancelSneaking owner left the current CombatSession";
            return false;
        }
        if (impl->enemy_controller)
            return impl->enemy_controller->cancel_contact_sneaking(
                *impl->session, id, impl->source.sneaking, error);
        if (!impl->source.cancel_sneaking) {
            error = "Required actual source Character::CancelSneaking provider";
            return false;
        }
        return impl->source.cancel_sneaking(id, error);
    };
    out.set_target = [impl](ActorId owner, ActorId target, std::uint32_t flags,
                            std::string& error) {
        return impl->set_target(owner, target, flags, error);
    };
    return out;
}

} // namespace dh::foundation::enemy_ai
