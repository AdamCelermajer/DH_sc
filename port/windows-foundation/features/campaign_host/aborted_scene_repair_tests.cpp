// B145 tests: the generic aborted-scene re-arm rule over the real compiled Swamp script bank (argument: staged assets root).
#include "aborted_scene_repair.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::campaign_host;

namespace {
void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }

const std::string kKey = "corner_ruin_ws_00.mgp::_prim_TriggerZone_BigLizIntro";
const std::string kChief = "_prim_Monster_05_325", kCaptive = "_prim_Monster_08_329";

struct Fixture {
    std::map<std::string, SceneActorFact> actors;
    bool providerFails = false;
    SceneActorFactProvider provider() {
        return [this](const std::string& name, SceneActorFact& fact, std::string& error) {
            if (providerFails) { error = "injected provider failure"; return false; }
            const auto found = actors.find(name);
            fact = found == actors.end() ? SceneActorFact{} : found->second;
            return true;
        };
    }
    void damaged() {
        actors[kChief] = {true, true, true, true};   // alive, never spawned, on its anchor
        actors[kCaptive] = {true, true, false, false}; // alive
    }
};

void register_zone(OriginalCampaignRuntime& runtime, const std::string& key, const std::string& script, const std::string& count = "") {
    std::map<std::string, std::string> attributes{{"name", key.substr(key.find("::") + 2)}, {"gametype", "TriggerZone"}, {"script", script}};
    if (!count.empty()) attributes["triggercount"] = count;
    std::string error;
    check(runtime.register_trigger(key, attributes, error), "register " + key + ": " + error);
}

void scene_facts_from_the_real_bank(OriginalCampaignRuntime& runtime) {
    OriginalCampaignRuntime::TriggerSceneFacts facts;
    check(runtime.trigger_scene_facts(kKey, facts), "BigLizIntro enter script is analyzable");
    check(facts.blocked_spawn_targets == std::vector<std::string>{kChief}, "exactly the captain is spawned inside the save-blocked window");
    check(facts.kill_targets == std::vector<std::string>{kCaptive}, "exactly the captive is killed inside the save-blocked window");
    check(!facts.consumed, "no restored activation count yet");
    runtime.restore_trigger_activations({{kKey, 1}});
    check(runtime.trigger_scene_facts(kKey, facts) && facts.consumed, "a restored count of 1 consumes the triggercount-1 zone");
    check(!runtime.trigger_scene_facts("missing.mgp::none", facts), "unknown trigger has no scene facts");
}

void survey_of_the_swamp_bank(OriginalCampaignRuntime& runtime) {
    // Every level script, as if a zone ran it: which would the rule treat as a save-blocked spawn scene.
    std::vector<std::string> scenes;
    int index = 0;
    for (const auto& script : runtime.scripts()) {
        if (script.scope == "common") continue;
        const std::string key = "survey.mgp::z" + std::to_string(index++);
        std::string error;
        runtime.register_trigger(key, {{"name", key}, {"gametype", "TriggerZone"}, {"script", script.name}}, error);
        OriginalCampaignRuntime::TriggerSceneFacts facts;
        if (runtime.trigger_scene_facts(key, facts) && !facts.blocked_spawn_targets.empty()) {
            std::string line = script.name + " ->";
            for (const auto& name : facts.blocked_spawn_targets) line += " " + name;
            scenes.push_back(line);
        }
    }
    std::cout << "survey: level scripts that spawn inside a Script_BlockSaveGame window:\n";
    for (const auto& line : scenes) std::cout << "  " << line << '\n';
    check(!scenes.empty(), "the survey finds the captain scene");
}

void decision_matrix(OriginalCampaignRuntime& runtime) {
    Fixture f;
    std::set<std::string> applied;
    runtime.restore_trigger_activations({{kKey, 1}});
    f.damaged();
    check(decide_aborted_scene_rearm(runtime, kKey, applied, f.provider()).verdict == SceneRepairVerdict::rearm, "aborted-scene signature is admitted");

    auto expect = [&](const char* label, SceneRepairVerdict verdict, const std::function<void(Fixture&)>& mutate) {
        Fixture g;
        g.damaged();
        mutate(g);
        const auto got = decide_aborted_scene_rearm(runtime, kKey, applied, g.provider());
        check(got.verdict == verdict, std::string(label) + ": got " + verdict_name(got.verdict) + " expected " + verdict_name(verdict));
    };
    expect("captain already spawned/fighting", SceneRepairVerdict::spawn_target_not_pristine, [](Fixture& g) { g.actors[kChief].never_spawned = false; });
    expect("captain killed", SceneRepairVerdict::spawn_target_not_pristine, [](Fixture& g) { g.actors[kChief].alive = false; });
    expect("captain not on its anchor", SceneRepairVerdict::spawn_target_not_pristine, [](Fixture& g) { g.actors[kChief].at_anchor = false; });
    expect("captain unresolved", SceneRepairVerdict::actor_unresolved, [](Fixture& g) { g.actors.erase(kChief); });
    expect("captive already dead (scene effects present)", SceneRepairVerdict::kill_target_gone, [](Fixture& g) { g.actors[kCaptive].alive = false; });
    expect("captive unresolved", SceneRepairVerdict::actor_unresolved, [](Fixture& g) { g.actors.erase(kCaptive); });
    expect("provider failure", SceneRepairVerdict::provider_error, [](Fixture& g) { g.providerFails = true; });

    std::set<std::string> stamped{repair_stamp(aborted_scene_rearm_repair_id, kKey)};
    check(decide_aborted_scene_rearm(runtime, kKey, stamped, f.provider()).verdict == SceneRepairVerdict::already_applied, "a stamped save is never repaired again");
    runtime.restore_trigger_activations({});
    check(decide_aborted_scene_rearm(runtime, kKey, applied, f.provider()).verdict == SceneRepairVerdict::not_consumed, "a never-activated trigger is left alone");
    runtime.restore_trigger_activations({{kKey, 1}});
}

void rearm_pass(OriginalCampaignRuntime& runtime) {
    const std::string other = "other.mgp::_prim_Zone_Other";
    register_zone(runtime, other, "enterLocation_South_Boardwalk");
    const std::string multi = "multi.mgp::_prim_Zone_Multi";
    register_zone(runtime, multi, "LizCaptain_Intro", "3");
    runtime.restore_trigger_activations({{kKey, 1}, {other, 1}, {multi, 1}});
    runtime.restore_trigger_inside({kKey});

    Fixture f;
    f.damaged();
    std::set<std::string> applied;
    std::ostringstream log;
    auto rearmed = rearm_aborted_scene_triggers(runtime, applied, f.provider(), log);
    check(rearmed == std::vector<std::string>{kKey}, "only the consumed, pristine scene trigger is re-armed");
    const auto counts = runtime.trigger_activations();
    check(!counts.count(kKey), "re-armed trigger count is back to zero");
    check(counts.at(other) == 1 && counts.at(multi) == 1, "unrelated and not-yet-consumed triggers keep their counts");
    check(runtime.trigger_inside().empty(), "the saved inside edge is dropped so the zone fires on the first contact");
    check(applied.count(repair_stamp(aborted_scene_rearm_repair_id, kKey)) == 1, "the stamp is recorded");
    check(log.str().find("PORT REPAIR (not original behaviour)") != std::string::npos, "the log labels the repair");

    // Idempotence: the same pass again changes nothing; a later identical signature is refused by the stamp.
    std::ostringstream again;
    check(rearm_aborted_scene_triggers(runtime, applied, f.provider(), again).empty() && !runtime.trigger_activations().count(kKey), "second pass is a no-op");
    runtime.restore_trigger_activations({{kKey, 1}});
    std::ostringstream stamped;
    check(rearm_aborted_scene_triggers(runtime, applied, f.provider(), stamped).empty() && runtime.trigger_activations().at(kKey) == 1, "stamped trigger is not re-armed twice");
    check(stamped.str().find("already-applied") != std::string::npos, "the refusal is logged");

    // A legitimately finished scene: captain fighting (spawned) and captive dead. Nothing changes.
    std::set<std::string> none;
    Fixture done;
    done.actors[kChief] = {true, true, false, false};
    done.actors[kCaptive] = {true, false, false, false};
    runtime.restore_trigger_activations({{kKey, 1}});
    std::ostringstream quiet;
    check(rearm_aborted_scene_triggers(runtime, none, done.provider(), quiet).empty() && none.empty() && runtime.trigger_activations().at(kKey) == 1,
          "a finished scene is untouched and unstamped");
    // Chief killed after the scene.
    Fixture killed;
    killed.actors[kChief] = {true, false, false, false};
    killed.actors[kCaptive] = {true, false, false, false};
    check(rearm_aborted_scene_triggers(runtime, none, killed.provider(), quiet).empty() && none.empty(), "a killed captain is untouched");
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply the staged assets root");
        const AssetCatalog assets(argv[1]);
        OriginalCampaignRuntime runtime;
        std::string error;
        check(runtime.load_compiled_level(assets, "data/scene/001_swamp.mlx", error), "compiled Swamp bank: " + error);
        register_zone(runtime, kKey, "LizCaptain_Intro");
        scene_facts_from_the_real_bank(runtime);
        survey_of_the_swamp_bank(runtime);
        decision_matrix(runtime);
        rearm_pass(runtime);
        std::cout << "aborted_scene_repair tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "aborted_scene_repair test failed: " << e.what() << '\n';
        return 1;
    }
}
