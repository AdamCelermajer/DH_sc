#include "runtime_boss_encounter_session_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;
using namespace dh2::bosses::runtime_v1;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

void run(const std::string& assetRoot) {
    AssetCatalog campaignAssets(assetRoot);
    AssetCatalog assets(assetRoot + "/original-cache");
    std::string error;
    OriginalMeleeBindings bindings;
    check(bindings.load(campaignAssets, "original-melee-bindings.xml", error), error);
    ActorCustomization customization;
    customization.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan visual;
    check(build_original_combat_visual_plan(assets, bindings, "SwampKing", customization,
        "boss-session-plan-v1", visual, error), error);
    const auto* idle = visual.sequence("Idle", 0);
    check(idle && idle->name == "SwampKing_Idle" && idle->loop == -1 && idle->phases.size() == 1,
          "Actual SwampKing Idle sequence row is missing");
    const auto* idleLeaf = visual.phase("Idle", 0, {0});
    check(idleLeaf && idleLeaf->animationId == 1328 &&
          idleLeaf->sourceUri == "data/3D/characters/swampking/animations/swampking_idle.bdae",
          "Original SwampKing Idle root/animation row changed");
    check(!assets.read(idleLeaf->sourceUri).empty(), "Original SwampKing Idle root asset is absent");
    std::vector<std::string> unsupportedVisualStates;
    for (const auto& clip : visual.config.clips) {
        auto probeConfig = visual.config;
        probeConfig.clips = {clip};
        CharacterVisual probe;
        std::string probeError;
        if (!probe.load(assets, probeConfig, probeError)) unsupportedVisualStates.push_back(clip.first);
    }
    check(unsupportedVisualStates.size() == 1 &&
          unsupportedVisualStates.front().find("/Injured/") != std::string::npos,
          "Only the unrelated source Injury clip should be excluded from this timer/phase fixture");

    // The retained profile exposes one unrelated Injured BDAE with no
    // bindable tracks. Keep the test Session's source actor/model/Idle row
    // byte-for-byte from the original binding while omitting only that
    // unsupported state; the consumer under test never selects Injury.
    const auto sourceBindingBytes = campaignAssets.read("original-melee-bindings.xml");
    std::string sourceBindingXml(sourceBindingBytes.begin(), sourceBindingBytes.end());
    const auto actorStart = sourceBindingXml.find("<actor id=\"SwampKing\"");
    const auto actorEnd = actorStart == std::string::npos ? std::string::npos : sourceBindingXml.find("</actor>", actorStart);
    check(actorStart != std::string::npos && actorEnd != std::string::npos,
          "Original SwampKing actor binding row is absent");
    const auto injuryStart = sourceBindingXml.find("<state name=\"Injured\">", actorStart);
    const auto injuryEnd = injuryStart == std::string::npos ? std::string::npos : sourceBindingXml.find("</state>", injuryStart);
    check(injuryStart != std::string::npos && injuryStart < actorEnd && injuryEnd != std::string::npos && injuryEnd < actorEnd,
          "Original SwampKing Injury source row location changed");
    sourceBindingXml.erase(injuryStart, injuryEnd + std::string("</state>").size() - injuryStart);
    std::vector<std::uint8_t> sessionBindingBytes(sourceBindingXml.begin(), sourceBindingXml.end());
    OriginalMeleeBindings sessionBindings;
    check(sessionBindings.decode(sessionBindingBytes, error), error);
    const auto* sourceKing = bindings.find_actor("SwampKing");
    const auto* sessionKing = sessionBindings.find_actor("SwampKing");
    check(sourceKing && sessionKing && sourceKing->model == sessionKing->model &&
          sourceKing->propertyRow == sessionKing->propertyRow &&
          sourceKing->states.at("Idle").at(0).id == sessionKing->states.at("Idle").at(0).id &&
          sessionKing->states.count("Injured") == 0,
          "Test binding must preserve the actual SwampKing actor/model/Idle source rows and omit only unsupported Injury");
    OriginalCombatVisualPlan sessionVisual;
    check(build_original_combat_visual_plan(assets, sessionBindings, "SwampKing", customization,
        "boss-session-retained-v1", sessionVisual, error), error);

    OriginalPropertyDatabase database;
    check(load_original_property_tables(assets, "data/pydata", database, error), error);
    CombatSessionConfig config;
    config.diagnosticRngSeed = 211;
    config.playerId = 1;
    config.playerProfileId = "SwampKing";
    config.tableRoot = "data/pydata";
    config.playerVisualConfig = sessionVisual.config;
    config.playerVisualConfig.motion_node_id = "auto";
    config.playerVisualConfig.consume_root_motion = true;
    CombatSessionProfile profile;
    profile.initialIdle = {"Idle", 0, {0}};
    profile.customization = customization;
    profile.animationOnly = true;
    profile.motionRoot = "auto";
    config.profiles.emplace("SwampKing", profile);
    ActorPopulation population;
    CharacterVisual playerVisual;
    CombatSession session;
    check(session.initialize(assets, database, sessionBindings, config, playerVisual, population,
                             {0, 0, 0}, customization, error), "same CombatSession init: " + error);

    const auto boss = session.player_id();
    auto* actor = session.actor(boss);
    check(actor && actor->definition_id == "SwampKing", "Same-Session SwampKing actor is missing");
    check(session.retained_actor_pose(boss), "Same-Session retained SwampKing pose is missing");
    // Actor 358's full native HP initialization remains outside this helper;
    // seed a live Session actor fact so the frame consumer exercises AIStates.Idle.
    actor->health = 100.0f;
    actor->max_health = 100.0f;
    actor->action = CharacterAction::idle;
    check(session.set_actor_original_state(boss, 3, error), error);
    SwampKingSessionConsumerV1 consumer;
    check(consumer.bind(session, boss, error), error);

    SessionFrameFactsV1 facts{boss, true, 100.0f, true, false, true, false};
    session.set_frame_begin_provider([&](CombatSession& current, double dt, std::string& e) {
        return consumer.on_frame_begin(current, dt, facts, e);
    });
    InputActions input;
    check(session.update(0.0, input, {0, 0, 0}, 0.0f, error), error);
    check(consumer.status().timers_started && consumer.status().phase == Phase::phase1,
          "Source admitted+Idle actor did not start timer owner: started=" +
          std::to_string(consumer.status().timers_started) + " phase=" +
          std::to_string(static_cast<int>(consumer.status().phase)) + " native=" +
          std::to_string(session.original_actor_state(boss)) + " frame=" +
          std::to_string(session.update_serial()));
    check(session.update(0.5, input, {0, 0, 0}, 0.0f, error), error);
    check(!consumer.status().attack_ready && consumer.status().timer_callbacks == 0,
          "1500 ms source attack timer fired early");
    check(session.update(0.5, input, {0, 0, 0}, 0.0f, error), error);
    check(!consumer.status().attack_ready && consumer.status().timer_callbacks == 0,
          "1500 ms source attack timer fired at 1000 ms");

    // The third half-second reaches the exact source interval. HasTarget is
    // false on this actor, but source isAttackReady still sets attack_flag.
    check(actor->target_id == invalid_actor_id, "Fixture unexpectedly has a target");
    check(session.update(0.5, input, {0, 0, 0}, 0.0f, error), error);
    check(consumer.status().attack_ready && consumer.status().timer_callbacks == 1 &&
          consumer.status().attack_timer_elapsed_ms == 0.0,
          "1500 ms source timer did not set readiness for same-Session actor");
    facts.source_native_ai_skill = true;
    check(session.update(1.5, input, {0, 0, 0}, 0.0f, error), error);
    check(!consumer.status().attack_ready && consumer.status().timer_callbacks == 2,
          "Source AIStates.Skill did not block attack readiness");

    facts.source_native_ai_skill = false;
    facts.source_script_idle = false;
    check(session.update(1.5, input, {0, 0, 0}, 0.0f, error), error);
    check(!consumer.status().attack_ready && consumer.status().timer_callbacks == 3,
          "Source script state gate did not block readiness");
    facts.source_script_idle = true;
    facts.can_dive_now = true;
    check(session.update(1.5, input, {0, 0, 0}, 0.0f, error), error);
    check(!consumer.status().attack_ready && consumer.status().timer_callbacks == 4,
          "Source canDiveNow gate did not block readiness");

    facts.can_dive_now = false;
    facts.source_hp_percent = 75.0f;
    check(session.update(0.0, input, {0, 0, 0}, 0.0f, error), error);
    check(consumer.status().phase == Phase::phase2 && !consumer.status().attack_ready,
          "Exact HPPercentCurr <= 75 transition did not latch phase 2 and clear readiness");
    check(session.retained_actor_pose(boss) && session.original_actor_state(boss) == 3,
          "Phase switch replaced the source Idle root or changed native state without a source transition");

    const auto before = consumer.status().timer_callbacks;
    auto foreignFacts = facts;
    foreignFacts.source_actor = boss + 1;
    check(!consumer.on_frame_begin(session, 0.0, foreignFacts, error) && !error.empty(),
          "Consumer accepted source facts from a different same-Session actor");
    check(consumer.status().timer_callbacks == before,
          "Foreign source facts advanced the timer");
    check(!consumer.on_frame_begin(session, 0.5, facts, error) && !error.empty(),
          "Consumer accepted a duplicate callback for the same CombatSession frame");
    check(consumer.status().timer_callbacks == before,
          "Duplicate Session frame advanced the source timer twice");

    std::cout << "PASS SwampKing source Idle asset/row 1328, same CombatSession actor, phase <=75 one-way transition, "
              << "1500 ms repeating readiness callback, source AI/script/dive gates, target-independent readiness, "
              << "same-actor fact binding, and duplicate-frame rejection\n";
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "usage: runtime_boss_encounter_session_v1_tests <asset-root>");
        run(argv[1]);
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL: " << ex.what() << '\n';
        return 1;
    }
}
