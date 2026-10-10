#include "effects_executor.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation::effects {
namespace {
bool valid_actor(const ActorAttachment& actor, std::string& error) {
    if (!actor.actor) { error = "Required source FX actor identity"; return false; }
    for (float value : actor.target_position)
        if (!std::isfinite(value)) { error = "Invalid source FX target position"; return false; }
    for (float value : actor.source_rotation)
        if (!std::isfinite(value)) { error = "Invalid source FX rotation"; return false; }
    return true;
}
struct StepContext {
    EffectsExecutor& executor;
    const ActorAttachment& actor;
    StepServices services;
    std::string error;
};
}
DispatchResult EffectsExecutor::marker(const ActorAttachment& actor, const std::string& clip,
                                      const MarkerOccurrence& occurrence, std::string& error) {
    error.clear();
    if (occurrence.marker.name.compare(0, 3, "fx_") != 0) return DispatchResult::ignored;
    if (!valid_actor(actor, error) || clip.empty()) {
        if (error.empty()) error = "Required retained source FX clip identity";
        return DispatchResult::required_failure;
    }
    MarkerKey key{actor.actor, clip, occurrence.generation, occurrence.cycle, occurrence.marker.index};
    // Commit before synchronous callbacks. A failed backend may have delivered
    // an original prefix; repeating that prefix is not a safe retry policy.
    if (!markers_.insert(std::move(key)).second) return DispatchResult::duplicate;
    return manager_.animation_event(occurrence.marker.name.c_str(), actor.target_position, error)
        ? DispatchResult::delivered : DispatchResult::required_failure;
}
DispatchResult EffectsExecutor::phase(const ActorAttachment& actor, const PhaseOccurrence& occurrence,
                                     const dh2::data::AnimationStep& step, StepServices services,
                                     std::string& error) {
    error.clear();
    if (!valid_actor(actor, error) || occurrence.actor != actor.actor) {
        if (error.empty()) error = "Required same source phase actor";
        return DispatchResult::required_failure;
    }
    if (!phases_.insert({actor.actor, occurrence.generation, occurrence.occurrence,
                         occurrence.source_path}).second) return DispatchResult::duplicate;
    StepContext context{*this, actor, services, {}};
    dh2::character::AnimationStepFxServicesV2 original;
    original.context = &context;
    original.owner = [](void* p, std::uintptr_t& result) {
        result = static_cast<StepContext*>(p)->actor.actor; return 0;
    };
    original.swoosh_fx_gate = [](void* p, const dh2::data::AnimationStep& row, bool& gate) {
        auto& c = *static_cast<StepContext*>(p);
        return c.services.swoosh_fx_gate ? c.services.swoosh_fx_gate(c.services.context, row, gate) : -1;
    };
    original.target_position = [](void* p, std::uintptr_t, float* result) {
        std::copy_n(static_cast<StepContext*>(p)->actor.target_position, 3, result); return 0;
    };
    original.rotation = [](void* p, std::uintptr_t, float* result) {
        std::copy_n(static_cast<StepContext*>(p)->actor.source_rotation, 3, result); return 0;
    };
    original.play = [](void* p, std::int32_t set, const float* position,
                       const float* rotation, std::uintptr_t anchor) {
        auto& c = *static_cast<StepContext*>(p);
        return c.executor.manager_.play_set(set, position, rotation, anchor, nullptr, c.error) ? 0 : -1;
    };
    if (dh2::character::character_animation_step_fx_v2(step, original, error)) {
        if (!context.error.empty()) error += ": " + context.error;
        return DispatchResult::required_failure;
    }
    return DispatchResult::delivered;
}
bool EffectsExecutor::play_set(std::int32_t set, const ActorAttachment& actor, bool anchored,
                               bool socket, std::uintptr_t* identity, std::string& error) {
    error.clear();
    if (identity) *identity = 0;
    if (set == -1) return true;
    if (!valid_actor(actor, error)) return false;
    if (socket && (!anchored || !actor.socket)) {
        error = "Required original FX socket anchor"; return false;
    }
    const float zero[3]{};
    return manager_.play_set(set, anchored ? zero : actor.target_position,
                             anchored ? nullptr : actor.source_rotation,
                             anchored ? (socket ? actor.socket : actor.actor) : 0, identity, error);
}
bool EffectsExecutor::stop(std::uintptr_t& identity, std::string& error) {
    error.clear();
    return identity == 0 || manager_.drop(identity, error);
}
void EffectsExecutor::release_actor(std::uintptr_t actor, std::uintptr_t socket) {
    if (actor) manager_.detach_anchor_v117(actor);
    if (socket && socket != actor) manager_.detach_anchor_v117(socket);
    for (auto it = markers_.begin(); it != markers_.end();)
        if (std::get<0>(*it) == actor) it = markers_.erase(it); else ++it;
    for (auto it = phases_.begin(); it != phases_.end();)
        if (std::get<0>(*it) == actor) it = phases_.erase(it); else ++it;
}
bool EffectsExecutor::scene_phase(std::int32_t absolute, std::int32_t dt, std::string& error) {
    if (dt < 0) { error = "Invalid source FX App dt"; return false; }
    return manager_.scene_frame(absolute, dt, error);
}
bool EffectsExecutor::manager_phase(std::int32_t dt, std::string& error) {
    if (dt < 0) { error = "Invalid source FX App dt"; return false; }
    return manager_.manager_frame(dt, error);
}
bool EffectsExecutor::draw_sources(std::vector<dh2::fx::CharacterFxMeshDrawSourceV4>& meshes,
                                   std::vector<dh2::fx::CharacterParticleDrawSourceV3>& particles,
                                   std::string& error) const {
    std::vector<dh2::fx::CharacterFxMeshDrawSourceV4> source_meshes;
    std::vector<dh2::fx::CharacterParticleDrawSourceV3> source_particles;
    if (!manager_.mesh_draw_sources_v4(source_meshes, error) ||
        !manager_.particle_draw_sources_v3(source_particles, error)) return false;
    meshes = std::move(source_meshes); particles = std::move(source_particles); return true;
}
} // namespace dh::foundation::effects
