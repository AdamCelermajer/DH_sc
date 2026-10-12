#include "aborted_scene_repair.hpp"
#include <ostream>

namespace dh::foundation::campaign_host {

std::string repair_stamp(const std::string& repair_id, const std::string& trigger_key) { return repair_id + "|" + trigger_key; }

const char* verdict_name(SceneRepairVerdict verdict) noexcept {
    switch (verdict) {
    case SceneRepairVerdict::rearm: return "rearm";
    case SceneRepairVerdict::not_a_blocked_scene: return "not-a-blocked-scene";
    case SceneRepairVerdict::not_consumed: return "not-consumed";
    case SceneRepairVerdict::already_applied: return "already-applied";
    case SceneRepairVerdict::actor_unresolved: return "actor-unresolved";
    case SceneRepairVerdict::spawn_target_not_pristine: return "spawn-target-not-pristine";
    case SceneRepairVerdict::kill_target_gone: return "kill-target-gone";
    case SceneRepairVerdict::provider_error: return "provider-error";
    }
    return "unknown";
}

SceneRepairDecision decide_aborted_scene_rearm(const OriginalCampaignRuntime& runtime, const std::string& trigger_key,
                                               const std::set<std::string>& applied_stamps, const SceneActorFactProvider& facts) {
    SceneRepairDecision decision;
    const auto finish = [&](SceneRepairVerdict verdict, std::string detail = {}) {
        decision.verdict = verdict;
        decision.detail = std::move(detail);
        return decision;
    };
    OriginalCampaignRuntime::TriggerSceneFacts scene;
    if (!runtime.trigger_scene_facts(trigger_key, scene) || scene.blocked_spawn_targets.empty())
        return finish(SceneRepairVerdict::not_a_blocked_scene);
    if (!scene.consumed) return finish(SceneRepairVerdict::not_consumed);
    if (applied_stamps.count(repair_stamp(aborted_scene_rearm_repair_id, trigger_key)))
        return finish(SceneRepairVerdict::already_applied);
    if (!facts) return finish(SceneRepairVerdict::provider_error, "no actor fact provider");
    // The spawn targets must all be exactly as the level start leaves them: a scene that ran (or half ran) leaves at least
    // one of them spawned, dead, moved, or Limbus, and a Chief that was killed or despawned later is never "pristine".
    for (const auto& name : scene.blocked_spawn_targets) {
        SceneActorFact fact;
        std::string error;
        if (!facts(name, fact, error)) return finish(SceneRepairVerdict::provider_error, name + ": " + error);
        if (!fact.found) return finish(SceneRepairVerdict::actor_unresolved, name);
        if (!fact.alive || !fact.never_spawned || !fact.at_anchor)
            return finish(SceneRepairVerdict::spawn_target_not_pristine, name);
    }
    // A target the scene itself kills (the captive) must still be alive: an already-dead one means the scene's effects,
    // or some other death, already happened and replaying the scene is not provably harmless.
    for (const auto& name : scene.kill_targets) {
        SceneActorFact fact;
        std::string error;
        if (!facts(name, fact, error)) return finish(SceneRepairVerdict::provider_error, name + ": " + error);
        if (!fact.found) return finish(SceneRepairVerdict::actor_unresolved, name);
        if (!fact.alive) return finish(SceneRepairVerdict::kill_target_gone, name);
    }
    return finish(SceneRepairVerdict::rearm);
}

std::vector<std::string> rearm_aborted_scene_triggers(OriginalCampaignRuntime& runtime, std::set<std::string>& applied_stamps,
                                                      const SceneActorFactProvider& facts, std::ostream& log) {
    std::vector<std::string> rearmed;
    for (const auto& activation : runtime.trigger_activations()) {
        if (activation.second < 1) continue;
        const auto decision = decide_aborted_scene_rearm(runtime, activation.first, applied_stamps, facts);
        if (decision.verdict == SceneRepairVerdict::not_a_blocked_scene) continue;
        if (decision.verdict != SceneRepairVerdict::rearm) {
            log << "[repair] " << aborted_scene_rearm_repair_id << " trigger=" << activation.first << " left untouched: "
                << verdict_name(decision.verdict) << (decision.detail.empty() ? "" : " (" + decision.detail + ")") << '\n';
            continue;
        }
        if (!runtime.rearm_trigger(activation.first)) continue;
        applied_stamps.insert(repair_stamp(aborted_scene_rearm_repair_id, activation.first));
        rearmed.push_back(activation.first);
        log << "[repair] " << aborted_scene_rearm_repair_id << " PORT REPAIR (not original behaviour): trigger=" << activation.first
            << " was consumed with its blocked scene never run; armed once more\n";
    }
    return rearmed;
}

} // namespace dh::foundation::campaign_host
