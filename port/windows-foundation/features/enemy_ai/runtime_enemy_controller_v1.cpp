#include "runtime_enemy_controller_v1.hpp"

#include "../../../level-world/character_cancel_sneaking.hpp"
#include "../../../level-world/character_sneaking_tables.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace dh::foundation::enemy_ai {
namespace {
bool fail(std::string& error, const char* message) {
    if (error.empty()) error = message;
    return false;
}

bool same_lease(const std::weak_ptr<const void>& a,
                const std::weak_ptr<const void>& b) {
    return !a.owner_before(b) && !b.owner_before(a);
}

bool inside_view(const ActorState& owner, const ActorState& candidate,
                 const dh2::data::AiProps& owner_ai,
                 const dh2::data::AiProps& candidate_ai, float radius) {
    dh2::data::AiRangeRequest request{};
    for (unsigned i = 0; i != 3; ++i) {
        request.owner[i] = owner.transform.position[i];
        request.target[i] = candidate.transform.position[i];
    }
    request.owner_melee = owner_ai.melee_radius;
    request.target_melee = candidate_ai.melee_radius;
    request.view = radius;
    dh2::data::AiRangeResult result{};
    return dh2_ai_range(&result, &request) == 0 && result.sight != 0;
}
}

RuntimeEnemyControllerV1::RuntimeEnemyControllerV1(RuntimeEnemyControllerServicesV1 services)
    : services_(std::move(services)) {}

bool RuntimeEnemyControllerV1::ensure_session(CombatSession& session,
                                               std::string& error) {
    const auto lease = session.actor_binding_lease();
    if (lease.expired()) return fail(error, "Enemy controller requires an active Session actor binding lease");
    if (!lease_initialized_ || !same_lease(binding_lease_, lease)) {
        actors_.clear();
        binding_lease_ = lease;
        lease_initialized_ = true;
        contact_clock_valid_ = false;
    }
    error.clear();
    return true;
}

bool RuntimeEnemyControllerV1::begin_contact_frame(CombatSession& session,
                                                    std::uint32_t dt_ms,
                                                    std::string& error) {
    if (!ensure_session(session, error)) return false;
    const auto serial = session.update_serial();
    if (serial > std::numeric_limits<std::uint32_t>::max())
        return fail(error, "Enemy contact Session serial exceeds the source 32-bit frame field");
    auto* world = session.world();
    if (!world) return fail(error, "Enemy contact frame requires the same live Session world");
    if (contact_clock_valid_ && contact_frame_ == serial) {
        if (contact_dt_ms_ != dt_ms)
            return fail(error, "Enemy contact frame received conflicting dt for one Session serial");
        error.clear();
        return true;
    }
    contact_frame_ = serial;
    contact_dt_ms_ = dt_ms;
    contact_clock_valid_ = true;
    for (const auto& pair : world->actors()) {
        auto inserted = actors_.try_emplace(pair.first);
        auto& state = inserted.first->second;
        if (inserted.second) {
            // A modern actor enters this shared owner with only its canonical
            // current target. A null target is the source constructor's null
            // preferred target; later accepted setters update both fields.
            state.preferred_target = pair.second.target_id;
            state.preferred_target_known = pair.second.target_id == invalid_actor_id;
        }
        if (state.collision_paused && state.pause_timer_remaining_ms &&
            state.pause_timer_last_frame != serial) {
            if (dt_ms >= state.pause_timer_remaining_ms) {
                // Source Timer event31 only clears the paused byte and returns.
                state.pause_timer_remaining_ms = 0;
                state.collision_paused = 0;
            } else {
                state.pause_timer_remaining_ms -= dt_ms;
            }
            state.pause_timer_last_frame = serial;
        }
    }
    error.clear();
    return true;
}

bool RuntimeEnemyControllerV1::project_contact(
    CombatSession& session, ActorId id, RuntimeEnemyContactAISPolicyV1 ais_policy,
    RuntimeEnemyContactControllerProjectionV1& out, std::string& error) {
    if (!ensure_session(session, error)) return false;
    if (!contact_clock_valid_ || contact_frame_ != session.update_serial())
        return fail(error, "Enemy contact projection has no current Session frame/dt projection");
    auto* actor = session.actor(id);
    if (!actor || !session.world() || !session.world()->traits(id) ||
        !session.world()->combat_properties(id))
        return fail(error, "Enemy contact projection actor left the current Session world");
    auto inserted = actors_.try_emplace(id);
    auto& state = inserted.first->second;
    if (inserted.second) {
        state.preferred_target = actor->target_id;
        state.preferred_target_known = actor->target_id == invalid_actor_id;
    }
    if (!state.preferred_target_known)
        return fail(error, "Enemy controller has no source-backed preferred-target projection for this actor");
    out = {};
    out.controller_projection = &state;
    out.preferred_target_known = true;
    out.preferred_target = state.preferred_target;
    out.application_frame = static_cast<std::uint32_t>(contact_frame_);
    out.dt_ms = contact_dt_ms_;
    out.paused = state.collision_paused;
    out.interactive415 = state.interactive415;
    out.ais_policy = ais_policy;
    error.clear();
    return true;
}

bool RuntimeEnemyControllerV1::set_contact_target(
    CombatSession& session, ActorId owner_id, ActorId target_id,
    std::uint32_t flags, std::string& error) {
    if (!ensure_session(session, error)) return false;
    if (flags != 0) return fail(error, "Source AI_SetTarget contact flags must be zero");
    ActorState* owner = session.actor(owner_id);
    if (!owner || !session.world() || !session.world()->traits(owner_id) ||
        !session.world()->combat_properties(owner_id))
        return fail(error, "Source AI_SetTarget owner left the current Session");
    if (target_id != invalid_actor_id &&
        (!session.actor(target_id) || !session.world()->traits(target_id) ||
         !session.world()->combat_properties(target_id)))
        return fail(error, "Source AI_SetTarget target is not a current same-Session actor");
    auto& state = actors_[owner_id];
    // The recovered flags0 setter stores its preferred target before current
    // target and performs additional native side effects outside this modern
    // controller's projection.
    state.preferred_target = target_id;
    state.preferred_target_known = true;
    owner->target_id = target_id;
    state.aggroed = target_id != invalid_actor_id;
    state.observed_target = target_id;
    if (!state.aggroed) {
        state.route = Route::searching;
        state.state_elapsed = 0.0;
    }
    error.clear();
    return true;
}

bool RuntimeEnemyControllerV1::process_contact_pause(
    CombatSession& session, physics::RuntimeSessionContactOwnerV1& contact,
    std::string& error) {
    if (!ensure_session(session, error)) return false;
    if (!contact_clock_valid_ || contact_frame_ != session.update_serial())
        return fail(error, "AIS collision pause processing requires the current Session frame/dt");
    for (const auto& pair : session.world()->actors()) {
        const ActorId id = pair.first;
        if (!contact.ais_fields(id)) continue; // No physical AIS body is registered.
        auto inserted = actors_.try_emplace(id);
        auto& state = inserted.first->second;
        if (inserted.second) {
            state.preferred_target = pair.second.target_id;
            state.preferred_target_known = pair.second.target_id == invalid_actor_id;
        }
        bool triggered = false;
        const bool ok = contact.process_collision_pause_threshold(id,
            [&](ActorId callback_id, std::string& callback_error) {
                if (callback_id != id) {
                    callback_error = "AIS collision pause callback changed actor identity";
                    return false;
                }
                // AI_PauseUpdate sets the byte and starts timer event31 before
                // source controller Stop. These are retained source-prefix
                // effects if a later stop provider rejects.
                state.collision_paused = 1;
                state.pause_timer_remaining_ms = 1000;
                state.pause_timer_last_frame = contact_frame_;
                ActorState* actor = session.actor(id);
                if (!actor) {
                    callback_error = "AIS collision pause actor left the current Session";
                    return false;
                }
                if (!services_.stop) {
                    callback_error = "AIS collision pause reached missing source controller Stop owner";
                    return false;
                }
                if (!services_.stop(session, *actor, callback_error)) {
                    if (callback_error.empty()) callback_error = "AIS collision pause controller Stop failed";
                    return false;
                }
                return true;
            }, triggered, error);
        if (!ok) return false;
        (void)triggered;
    }
    error.clear();
    return true;
}

bool RuntimeEnemyControllerV1::track_contact_buff(
    CombatSession& session, ActorId id, std::uint32_t buff_key,
    std::uintptr_t record, std::string& error) {
    if (!ensure_session(session, error)) return false;
    if (!record || !session.actor(id) || !session.world() ||
        !session.world()->traits(id) || !session.world()->combat_properties(id))
        return fail(error, "Source buff record requires a live same-Session actor and nonnull record identity");
    actors_[id].source_buffs[buff_key].push_back(record);
    error.clear();
    return true;
}

bool RuntimeEnemyControllerV1::cancel_contact_sneaking(
    CombatSession& session, ActorId id,
    const RuntimeEnemyContactSneakingServicesV1& source, std::string& error) {
    if (!ensure_session(session, error)) return false;
    const auto* traits = session.world() ? session.world()->traits(id) : nullptr;
    const auto* properties = session.world() ? session.world()->combat_properties(id) : nullptr;
    if (!traits || !properties || !session.actor(id))
        return fail(error, "CancelSneaking requires a current same-Session actor/property projection");
    auto& runtime = actors_[id];
    if (properties->sheets.resolved.size() != 224)
        return fail(error, "CancelSneaking requires the full 224-word current resolved property sheet");

    std::unique_ptr<dh2::character::sneaking::SneakingTables> source_tables;
    dh2::character::sneaking::Tables32 unavailable_tables{};
    const dh2::character::sneaking::Tables32* tables = nullptr;
    if (source.skill_tables) {
        try {
            source_tables = std::make_unique<dh2::character::sneaking::SneakingTables>(source.skill_tables);
            tables = &source_tables->view();
        } catch (const std::exception& failure) {
            return fail(error, failure.what());
        }
    } else if (properties->sheets.resolved[198] > 0) {
        // The positive branch reaches the ordered SkillList traversal. Keep a
        // valid, empty table object so the recovered kernel rejects the first
        // unavailable row instead of treating it as a successful no-skill case.
        unavailable_tables = {nullptr, 0, 0, nullptr, 0, 0};
        tables = &unavailable_tables;
    }

    std::vector<std::uintptr_t> scripts;
    if (source.skill_scripts && properties->sheets.resolved[198] > 0) {
        if (!source.skill_scripts(id, scripts, error)) {
            if (error.empty()) error = "Actual current CharAI skill-script vector provider failed";
            return false;
        }
    }
    dh2::character::sneaking::Character48 character{};
    dh2::character::sneaking::AI24 ai{};
    dh2::character::PropertySheet16 resolved{
        properties->sheets.resolved.data(), 224, 0};
    character.identity = id;
    character.resolved = resolved;
    character.tables = tables;
    character.ai = &ai;
    character.changed415 = runtime.interactive415;
    ai.owner = &character;
    ai.scripts = scripts.empty() ? nullptr : scripts.data();
    ai.count = static_cast<std::uint32_t>(scripts.size());

    struct Context {
        RuntimeEnemyControllerV1* owner{};
        CombatSession* session{};
        ActorId actor{};
        const RuntimeEnemyContactSneakingServicesV1* source{};
        dh2::character::sneaking::Character48* character{};
        dh2::character::sneaking::AI24* ai{};
        std::vector<std::uintptr_t>* scripts{};
    } context{this, &session, id, &source, &character, &ai, &scripts};
    const dh2::character::sneaking::Services16 services{
        &context,
        [](void* opaque, const dh2::character::sneaking::Request24* request,
           std::uint32_t* result) -> int {
            auto& call = *static_cast<Context*>(opaque);
            if (!request || !result) return 1;
            std::string callback_error;
            if (!call.owner->ensure_session(*call.session, callback_error)) return 1;
            auto actor_it = call.owner->actors_.find(call.actor);
            const auto* actor = call.session->actor(call.actor);
            const auto* actor_traits = call.session->world() ?
                call.session->world()->traits(call.actor) : nullptr;
            if (actor_it == call.owner->actors_.end() || !actor || !actor_traits) return 1;
            switch (request->operation) {
            case dh2::character::sneaking::is_player:
                if (request->receiver != call.actor) return 1;
                *result = actor_traits->is_player ? 1u : 0u;
                return 0;
            case dh2::character::sneaking::delete_buff: {
                if (request->receiver != call.actor || request->argument != 146u) return 1;
                auto records_it = actor_it->second.source_buffs.find(146u);
                while (records_it != actor_it->second.source_buffs.end() &&
                       !records_it->second.empty()) {
                    if (!call.source->delete_buff_record) return 1;
                    const auto token = records_it->second.front();
                    if (!call.source->delete_buff_record(call.actor, 146u, token,
                                                         callback_error)) return 1;
                    if (!call.owner->ensure_session(*call.session, callback_error)) return 1;
                    actor_it = call.owner->actors_.find(call.actor);
                    if (actor_it == call.owner->actors_.end()) return 1;
                    records_it = actor_it->second.source_buffs.find(146u);
                    if (records_it == actor_it->second.source_buffs.end()) break;
                    const auto record_it = std::find(records_it->second.begin(),
                                                     records_it->second.end(), token);
                    if (record_it != records_it->second.end())
                        records_it->second.erase(record_it);
                    if (records_it->second.empty()) {
                        actor_it->second.source_buffs.erase(records_it);
                        break;
                    }
                }
                *result = 0;
                return 0;
            }
            case dh2::character::sneaking::skill_check_active: {
                if (request->index >= call.ai->count || !call.ai->scripts ||
                    request->receiver != call.ai->scripts[request->index]) return 1;
                if (!call.source->skill_active) return 1;
                bool active = false;
                if (!call.source->skill_active(call.actor, request->receiver,
                        request->index, active, callback_error)) return 1;
                if (active) {
                    // The source reloads the current script vector after an
                    // Active callback may synchronously replace its entry.
                    if (!call.source->skill_scripts) return 1;
                    std::vector<std::uintptr_t> refreshed;
                    if (!call.source->skill_scripts(call.actor, refreshed,
                                                    callback_error)) return 1;
                    *call.scripts = std::move(refreshed);
                    call.ai->scripts = call.scripts->empty() ? nullptr : call.scripts->data();
                    call.ai->count = static_cast<std::uint32_t>(call.scripts->size());
                }
                *result = active ? 1u : 0u;
                return 0;
            }
            case dh2::character::sneaking::skill_pre:
                if (request->index >= call.ai->count || !call.ai->scripts ||
                    request->receiver != call.ai->scripts[request->index]) return 1;
                if (!call.source->skill_pre ||
                    !call.source->skill_pre(call.actor, request->receiver,
                        request->index, callback_error)) return 1;
                *result = 0;
                return 0;
            }
            return 1;
        }};

    const unsigned status = dh2_character_cancel_sneaking(&character, &services);
    auto updated = actors_.find(id);
    if (updated != actors_.end()) updated->second.interactive415 = character.changed415;
    if (status != 0) {
        if (error.empty()) {
            error = status == 2 ? "CancelSneaking source buff/skill provider failed" :
                "CancelSneaking positive Special_Sneak branch lacks valid original skill/script owners";
        }
        return false;
    }
    error.clear();
    return true;
}

bool RuntimeEnemyControllerV1::update(CombatSession& session, double dt, std::string& error) {
    error.clear();
    if (!std::isfinite(dt) || dt < 0.0)
        return fail(error, "Enemy decision dt must be finite and nonnegative");
    auto* world = session.world();
    const auto* tables = session.original_ai_tables();
    if (!world || !tables)
        return fail(error, "Enemy decision requires the session's live world and original AI tables");
    if (!ensure_session(session, error)) return false;

    // ActorId order is deterministic and these are the canonical records. It
    // is not claimed to reproduce the source scene/list60 ordering.
    std::vector<ActorId> ids;
    ids.reserve(world->actors().size());
    for (const auto& entry : world->actors()) ids.push_back(entry.first);
    for (auto it = actors_.begin(); it != actors_.end();) {
        if (!std::binary_search(ids.begin(), ids.end(), it->first)) it = actors_.erase(it);
        else ++it;
    }

    for (const ActorId id : ids) {
        ActorState* owner = world->find_actor(id);
        if (!owner || !owner->alive() || owner->action == CharacterAction::dead) continue;
        const auto* traits = world->traits(id);
        const auto* owner_props = world->combat_properties(id);
        if (!traits || traits->is_player || !owner_props) continue;
        const auto* ai = dh2::data::ai_props(*tables, owner_props->sheets.resolved[1]);
        if (!ai || ai->script != "monster") continue;

        ++report_.supported_actors;
        ++report_.per_actor_ticks;
        auto inserted = actors_.try_emplace(id);
        auto& state = inserted.first->second;
        if (inserted.second) {
            state.preferred_target = owner->target_id;
            state.preferred_target_known = owner->target_id == invalid_actor_id;
        }
        const ActorId tracked_id = owner->target_id;
        const bool had_target = tracked_id != invalid_actor_id;
        state.aggroed = had_target;

        if (had_target) {
            ActorState* target = world->find_actor(tracked_id);
            const bool dead = !target || !target->alive() || target->action == CharacterAction::dead;
            bool valid = target && !dead && world->eligible_target(*owner, *target);
            bool out_of_view_radius = false;
            if (valid) {
                const auto* target_props = world->combat_properties(tracked_id);
                const auto* target_ai = target_props ? dh2::data::ai_props(*tables, target_props->sheets.resolved[1]) : nullptr;
                if (!target_props || !target_ai) valid = false;
                else if (!inside_view(*owner, *target, *ai, *target_ai, ai->view_radius)) {
                    valid = false;
                    out_of_view_radius = true;
                }
            }
            // Source _UpdateTarget retains an existing target using
            // AI_IsInSight's authored target-position/view-radius predicate.
            // Navigation ray visibility belongs to acquisition/approach and
            // is not an additional retention gate here.
            if (!valid) {
                if (services_.stop && !services_.stop(session, *owner, error))
                    return fail(error, "Shared animation/navigation stop owner failed on target loss");
                owner = world->find_actor(id);
                if (!owner) continue;
                // The target ID is the sole authority. No parallel aggro list
                // or copied target state is left behind by this controller.
                owner->target_id = invalid_actor_id;
                state.preferred_target = invalid_actor_id;
                state.preferred_target_known = true;
                state.aggroed = false;
                state.observed_target = invalid_actor_id;
                state.route = Route::searching;
                state.state_elapsed = 0.0;
                if (dead) ++report_.target_deaths;
                else if (out_of_view_radius) ++report_.target_range_losses;
                else ++report_.target_sight_losses;
                ++report_.lost_targets;
                continue;
            }

            owner = world->find_actor(id);
            target = world->find_actor(tracked_id);
            if (!owner || !target) continue;
            const bool in_melee = world->original_melee_in_range(id, tracked_id);
            const Route next_route = in_melee ? Route::attack : Route::approach;
            // Hand the shared locomotion pose back before a newly in-range
            // attack request. Stop only on the approach->attack transition;
            // repeating it during cooldown would fight an active attack pose.
            if (next_route == Route::attack && state.route == Route::approach &&
                !session.owns_pose(id) && services_.stop &&
                !services_.stop(session, *owner, error))
                return fail(error, "Shared navigation stop owner failed on melee approach completion");
            if (state.observed_target != tracked_id || state.route != next_route)
                state.state_elapsed = 0.0;
            else state.state_elapsed += dt;
            state.observed_target = tracked_id;
            state.route = next_route;

            if (!in_melee) {
                if (!services_.approach)
                    return fail(error, "Shared navigation/locomotion owner is required for an out-of-range monster target");
                if (!services_.approach(session, *owner, *target, *ai, error))
                    return fail(error, "Shared navigation/locomotion owner failed for an out-of-range monster target");
                ++report_.approaches;
                continue;
            }

            bool special_handled = false;
            if (services_.special_action) {
                if (!services_.special_action(*owner, *target, *owner_props,
                                              *ai, special_handled, error))
                    return fail(error, "Source skill/buff special-action owner failed");
            } else {
                ++report_.unsupported_special_actions;
            }
            if (special_handled) continue;
            if (!session.request_actor_attack(id, tracked_id, dt, error))
                return fail(error, "CombatSession rejected the source enemy attack request");
            ++report_.attack_requests;
            continue;
        }

        // No current source target: use the authored no-aggro radius and the
        // original AI range kernel. Search order is stable ActorId order until
        // the native level's list60 producer is connected.
        const float radius = state.aggroed ? ai->view_radius : ai->view_radius_no_aggro;
        for (const ActorId candidate_id : ids) {
            if (candidate_id == id) continue;
            owner = world->find_actor(id);
            ActorState* candidate = world->find_actor(candidate_id);
            if (!owner || !candidate || !world->eligible_target(*owner, *candidate)) {
                ++report_.candidates_rejected_by_faction_or_sneak;
                continue;
            }
            const auto* candidate_props = world->combat_properties(candidate_id);
            const auto* candidate_ai = candidate_props ?
                dh2::data::ai_props(*tables, candidate_props->sheets.resolved[1]) : nullptr;
            if (!candidate_props || !candidate_ai ||
                !search_sneak_gate(*owner_props, *candidate_props)) {
                ++report_.candidates_rejected_by_faction_or_sneak;
                continue;
            }
            if (!inside_view(*owner, *candidate, *ai, *candidate_ai, radius)) continue;
            if (!services_.can_see)
                return fail(error, "Current-level LOS/interact owner is required for an in-radius candidate");
            bool visible = false;
            if (!services_.can_see(session, *owner, *candidate, *owner_props,
                                   *candidate_props, *ai, visible, error))
                return fail(error, "Current-level LOS/interact owner failed for a search candidate");
            if (!visible) continue;

            owner = world->find_actor(id);
            candidate = world->find_actor(candidate_id);
            if (!owner || !candidate || !world->eligible_target(*owner, *candidate)) continue;
            owner->target_id = candidate_id;
            state.preferred_target = candidate_id;
            state.preferred_target_known = true;
            state.aggroed = true;
            state.observed_target = candidate_id;
            const bool in_melee = world->original_melee_in_range(id, candidate_id);
            state.route = in_melee ? Route::attack : Route::approach;
            state.state_elapsed = 0.0;
            ++report_.acquired_targets;
            if (in_melee) {
                // The source OnTargetInMeleeRange callback always issues Stop
                // before Attack/DoSkill. Acquisition can itself find a target
                // already inside melee reach, so do not leave a previously
                // selected Walk route/pose active for the first attack.
                if (!services_.stop)
                    return fail(error, "Shared stop owner is required before an in-melee monster attack");
                if (!services_.stop(session, *owner, error))
                    return fail(error, "Shared stop owner failed before an in-melee monster attack");
            } else {
                if (!services_.approach)
                    return fail(error, "Shared navigation/locomotion owner is required after monster target acquisition");
                if (!services_.approach(session, *owner, *candidate, *ai, error))
                    return fail(error, "Shared navigation/locomotion owner failed after monster target acquisition");
                ++report_.approaches;
            }
            break;
        }
    }
    return true;
}

CombatSession::ActorDecisionProvider make_runtime_enemy_decision_provider_v1(
    RuntimeEnemyControllerServicesV1 services) {
    auto controller = std::make_shared<RuntimeEnemyControllerV1>(std::move(services));
    return [controller = std::move(controller)](CombatSession& session, double dt,
                                                 std::string& error) {
        return controller->update(session, dt, error);
    };
}

} // namespace dh::foundation::enemy_ai
