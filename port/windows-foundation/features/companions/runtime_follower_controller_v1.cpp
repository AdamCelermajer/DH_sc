#include "runtime_follower_controller_v1.hpp"

#include <cmath>
#include <iostream>

namespace dh::foundation::companions {

namespace {
const char* action_name(FollowerActionV1 a) {
    switch (a) {
    case FollowerActionV1::approach: return "approach";
    case FollowerActionV1::stop: return "stop";
    case FollowerActionV1::warp_behind: return "warp_behind";
    case FollowerActionV1::warp_to_master: return "warp_to_master";
    default: return "none";
    }
}
} // namespace

RuntimeFollowerControllerV1::RuntimeFollowerControllerV1(RuntimeFollowerServicesV1 services) : services_(std::move(services)) {}

const FollowerMemoryV1* RuntimeFollowerControllerV1::memory(ActorId id) const noexcept {
    const auto found = actors_.find(id);
    return found == actors_.end() ? nullptr : &found->second.memory;
}

bool RuntimeFollowerControllerV1::update(CombatSession& session, double dt_seconds, std::string& error) {
    error.clear();
    if (!services_.approach || !services_.stop || !services_.warp) {
        error = "Follower controller needs the route, stop and warp services";
        return false;
    }
    if (services_.active && !services_.active()) return true;
    auto* world = session.world();
    const auto* tables = session.original_ai_tables();
    if (!world || !tables) return true;
    ActorState* master = world->find_actor(session.player_id());
    if (!master) return true;
    const bool master_alive = master->alive() && master->action != CharacterAction::dead;
    for (const auto& entry : world->actors()) {
        const ActorId id = entry.first;
        if (id == session.player_id()) continue;
        const auto* props = world->combat_properties(id);
        const auto* traits = world->traits(id);
        if (!props || !traits || traits->is_player) continue;
        const auto* ai = dh2::data::ai_props(*tables, props->sheets.resolved[1]);
        if (!ai) continue;
        const auto script = follower_script_v1(ai->script);
        if (script == FollowerScriptV1::none) continue;
        ActorState* owner = world->find_actor(id);
        if (!owner || !owner->alive() || owner->action == CharacterAction::dead) continue;
        auto& runtime = actors_[id];
        if (runtime.failed) continue;
        const char* skip = nullptr;
        if (services_.enabled && !services_.enabled(id)) skip = "not enabled in the level population";
        // A script, cutscene or hit reaction owns the pose: the AI does not run on this actor right now.
        else if (session.owns_pose(id)) skip = "pose owned by a script/state sequence";
        if (skip != runtime.skip) {
            std::cout << "[follower] actor=" << id << " script=" << ai->script << (skip ? std::string(" waiting: ") + skip : std::string(" running")) << '\n';
            runtime.skip = skip;
        }
        if (skip) continue;
        ++report_.ticks;

        FollowerInputV1 input;
        const auto& a = owner->transform.position;
        const auto& b = master->transform.position;
        input.distance = std::sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) + (a[2] - b[2]) * (a[2] - b[2]));
        input.master_alive = master_alive;
        input.dt_ms = dt_seconds * 1000.0;
        input.radii.view_radius = ai->view_radius;
        input.radii.interact_radius = ai->interact_radius;
        input.radii.own_reach = ai->melee_radius;
        input.radii.master_radius = services_.body_radius ? services_.body_radius(*master) : 0.0f;

        const auto decision = follower_step_v1(script, runtime.memory, input);
        auto action = decision.action;
        std::string failure;
        bool ok = true;
        if (decision.release_haste) ++report_.haste_off;
        if (decision.apply_haste) ++report_.haste_on;
        switch (action) {
        case FollowerActionV1::approach: {
            bool found = true;
            ok = services_.approach(session, *owner, *master, *ai, runtime.memory.catching_up, found, failure);
            ++report_.approaches;
            // rene CatchFallbackCheck: no path to the master -> WarpTo(g_master).
            if (ok && !found && script == FollowerScriptV1::rene) {
                action = FollowerActionV1::warp_to_master;
                ok = services_.warp(session, *owner, b, failure);
                ++report_.warps_to_master;
            }
            break;
        }
        case FollowerActionV1::stop:
            ok = services_.stop(session, *owner, failure);
            ++report_.stops;
            break;
        case FollowerActionV1::warp_behind: {
            std::array<float, 3> target{};
            follower_warp_behind_position_v1(b.data(), master->transform.rotation[2], target.data());
            ok = services_.warp(session, *owner, target, failure) && services_.stop(session, *owner, failure);
            ++report_.warps_behind;
            break;
        }
        default: break;
        }
        if (action != runtime.last) {
            std::cout << "[follower] actor=" << id << " script=" << ai->script << " action=" << action_name(action)
                      << " distance=" << input.distance << " view=" << input.radii.view_radius
                      << " interact=" << input.radii.interact_radius << (decision.apply_haste ? " haste=on" : "")
                      << (decision.release_haste ? " haste=off" : "") << '\n';
            runtime.last = action;
        }
        if (!ok) {
            runtime.failed = true;
            ++report_.failures;
            std::cout << "[follower] actor=" << id << " disabled: " << failure << '\n';
        }
    }
    return true;
}

} // namespace dh::foundation::companions
