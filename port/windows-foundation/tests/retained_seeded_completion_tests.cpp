#include "../asset_catalog.hpp"
#include "../original_combat_visual_plan.hpp"
#include "../original_melee_bindings.hpp"
#include "../retained_sequence_playback.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

struct CaseResult {
    unsigned delivered{};
    unsigned completedFrames{};
    unsigned seededCloses{};
    unsigned fullCloses{};
    bool closeAfterFinalFrame{};
    std::uint32_t finalFramePending{};
    std::int32_t finalFrameExtra{};
};

void run_seed_case(const AssetCatalog& assets, const OriginalCombatVisualPlan& plan,
                   const OriginalCombatPhase& phase, std::int32_t repeats,
                   CaseResult& result, std::string& error) {
    auto config = plan.config;
    config.motion_node_id = "auto";
    config.consume_root_motion = true;
    config.clips.clear();
    config.clips.emplace_back(phase.clipName, phase.resolvedPath);
    CharacterVisual visual;
    check(visual.load(assets, config, error), error);
    RetainedSequencePlayback playback(visual, complete_pose_root_sampler(visual));
    RetainedAnimationFrame initial;
    bool finalFrameDelivered = false;
    RetainedSequenceServices services;
    services.frame = [&](std::size_t sourcePhase, const RetainedAnimationFrame& frame,
                         std::string&) {
        check(sourcePhase == SIZE_MAX, "Seeded callback fabricated a finite-action phase index");
        ++result.delivered;
        if (frame.completion.pending) {
            ++result.completedFrames;
            finalFrameDelivered = true;
            result.finalFramePending = frame.completion.pending;
            result.finalFrameExtra = frame.completion.extra_ms;
        }
        return true;
    };
    services.closed = [&](const dh2::timeline::Completion&, std::string&) {
        ++result.fullCloses;
        return true;
    };
    services.seeded_closed = [&](const dh2::timeline::Completion& completion, std::string&) {
        ++result.seededCloses;
        result.closeAfterFinalFrame = finalFrameDelivered && completion.pending == result.finalFramePending &&
            completion.extra_ms == result.finalFrameExtra;
        return true;
    };
    check(playback.prepare_seeded(assets, plan, phase, std::move(services), error), error);
    check(playback.seed_sequence(assets, phase.clipName, phase.resolvedPath,
          static_cast<float>(phase.speed), static_cast<std::int32_t>(phase.blendOut),
          phase.moveGO != 0, repeats, initial, error), error);
    check(!playback.finished(), "Seeded state changed the full-action finished flag");

    for (unsigned frame = 0; frame < 2000 && result.seededCloses == 0; ++frame)
        check(playback.advance_seeded(0.05, error), error);
    check(playback.seeded() && !playback.finished(),
          "Seeded completion changed finite-action lifecycle flags");
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Expected original asset root");
        const AssetCatalog assets(argv[1]);
        const AssetCatalog metadata(".local-inputs/windows-melee-bindings");
        OriginalMeleeBindings bindings;
        std::string error;
        check(bindings.load(metadata, "original-melee-bindings.xml", error), error);
        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan plan;
        check(build_original_combat_visual_plan(assets, bindings, "Swamp_LizadMan_Type1",
              customization, "seeded-completion-test", plan, error), error);
        const auto* injury = plan.phase("Injured", 0, {0});
        const auto* died = plan.phase("Died", 0, {0});
        const auto* walk = plan.phase("Walk", 0, {0});
        check(injury && injury->has_visual() && died && died->has_visual() && walk && walk->has_visual(),
              "Actual Lizard Injury, Died or Walk source phase is unavailable");

        // The new callback is separate from the existing full finite-action
        // close policy and must not alter its finished lifecycle.
        OriginalSequencePolicies policies;
        check(original_sequence_policies(bindings, policies, error), error);
        const auto* spawn = plan.sequence("Spawn", 0);
        check(spawn && !spawn->phases.empty(), "Actual Lizard Spawn finite sequence is unavailable");
        auto fullConfig = plan.config;
        fullConfig.motion_node_id = "auto";
        fullConfig.consume_root_motion = true;
        fullConfig.clips.clear();
        for (const auto& source : spawn->phases)
            if (source.has_visual()) fullConfig.clips.emplace_back(source.clipName, source.resolvedPath);
        CharacterVisual fullVisual;
        check(fullVisual.load(assets, fullConfig, error), error);
        RetainedSequencePlayback full(fullVisual, complete_pose_root_sampler(fullVisual));
        OriginalAttackSelection fullSelection;
        fullSelection.state = "Spawn";
        unsigned fullCloseCalls = 0, seededCloseCalls = 0;
        RetainedSequenceServices fullServices;
        fullServices.frame = [](std::size_t sourcePhase, const RetainedAnimationFrame&, std::string&) {
            return sourcePhase != SIZE_MAX;
        };
        fullServices.closed = [&](const dh2::timeline::Completion&, std::string&) {
            ++fullCloseCalls;
            return true;
        };
        fullServices.seeded_closed = [&](const dh2::timeline::Completion&, std::string&) {
            ++seededCloseCalls;
            return true;
        };
        check(full.prepare(assets, plan, policies, fullSelection, std::move(fullServices),
              "full-action-control", error), error);
        check(full.begin(error), error);
        for (unsigned frame = 0; frame < 2000 && full.active(); ++frame)
            check(full.advance(0.05, error), error);
        check(full.finished() && full.completed_phases() == spawn->phases.size() &&
              fullCloseCalls == 1 && seededCloseCalls == 0,
              "Full finite-action closure was captured or altered by seeded completion routing");

        // One authored finite source selection closes once after the final
        // delivered frame, with the actual completion object.
        CaseResult one;
        run_seed_case(assets, plan, *injury, 0, one, error);
        check(one.completedFrames == 1 && one.seededCloses == 1 && one.fullCloses == 0 &&
              one.closeAfterFinalFrame, "Finite Injury seed did not close once after final frame delivery");

        // Positive source Loop values count additional selections, so N=2
        // means three genuine clip completions but one final close.
        CaseResult repeated;
        run_seed_case(assets, plan, *died, 2, repeated, error);
        check(repeated.completedFrames == 3 && repeated.seededCloses == 1 &&
              repeated.fullCloses == 0 && repeated.closeAfterFinalFrame,
              "Finite source repeats did not close once after the last selection");

        // Infinite source repeats and raw timeline loops remain open-ended.
        CaseResult infinite;
        run_seed_case(assets, plan, *died, -1, infinite, error);
        check(infinite.completedFrames >= 2 && infinite.seededCloses == 0 && infinite.fullCloses == 0,
              "Infinite source repeat incorrectly emitted finite seeded completion");

        // The older raw seed API remains caller-owned even for a finite clip;
        // the new callback belongs only to the source sequence Loop policy.
        auto singleSeedConfig = plan.config;
        singleSeedConfig.motion_node_id = "auto";
        singleSeedConfig.consume_root_motion = true;
        singleSeedConfig.clips.clear();
        singleSeedConfig.clips.emplace_back(injury->clipName, injury->resolvedPath);
        CharacterVisual singleSeedVisual;
        check(singleSeedVisual.load(assets, singleSeedConfig, error), error);
        RetainedSequencePlayback singleSeed(singleSeedVisual, complete_pose_root_sampler(singleSeedVisual));
        RetainedAnimationFrame singleSeedFrame;
        unsigned rawFiniteCloses = 0;
        RetainedSequenceServices singleSeedServices;
        singleSeedServices.frame = [](std::size_t, const RetainedAnimationFrame&, std::string&) { return true; };
        singleSeedServices.seeded_closed = [&](const dh2::timeline::Completion&, std::string&) {
            ++rawFiniteCloses;
            return true;
        };
        check(singleSeed.prepare_seeded(assets, plan, *injury, std::move(singleSeedServices), error), error);
        check(singleSeed.seed(assets, injury->clipName, injury->resolvedPath, false,
              static_cast<float>(injury->speed), static_cast<std::int32_t>(injury->blendOut),
              injury->moveGO != 0, singleSeedFrame, error), error);
        for (unsigned frame = 0; frame < 2000 && !singleSeed.animation()->pose().current_ended(); ++frame)
            check(singleSeed.advance_seeded(0.05, error), error);
        check(singleSeed.animation()->pose().current_ended() && rawFiniteCloses == 0,
              "Legacy finite seed API was captured by the source-only completion callback");

        // The final-completion latch is retired before user code runs. A
        // synchronous zero-dt reentry may deliver a fresh frame, but cannot
        // recursively replay the old completion.
        auto reentryConfig = plan.config;
        reentryConfig.motion_node_id = "auto";
        reentryConfig.consume_root_motion = true;
        reentryConfig.clips.clear();
        reentryConfig.clips.emplace_back(injury->clipName, injury->resolvedPath);
        CharacterVisual reentryVisual;
        check(reentryVisual.load(assets, reentryConfig, error), error);
        RetainedSequencePlayback reentry(reentryVisual, complete_pose_root_sampler(reentryVisual));
        RetainedAnimationFrame reentrySeed;
        unsigned reentryCloses = 0, nestedFrames = 0;
        bool nestedResult = false;
        std::string nestedError;
        RetainedSequenceServices reentryServices;
        reentryServices.frame = [&](std::size_t, const RetainedAnimationFrame&, std::string&) {
            ++nestedFrames;
            return true;
        };
        reentryServices.seeded_closed = [&](const dh2::timeline::Completion&, std::string&) {
            ++reentryCloses;
            nestedResult = reentry.advance_seeded(0, nestedError);
            return true;
        };
        check(reentry.prepare_seeded(assets, plan, *injury, std::move(reentryServices), error), error);
        check(reentry.seed_sequence(assets, injury->clipName, injury->resolvedPath,
              static_cast<float>(injury->speed), static_cast<std::int32_t>(injury->blendOut),
              injury->moveGO != 0, 0, reentrySeed, error), error);
        for (unsigned frame = 0; frame < 2000 && reentryCloses == 0; ++frame)
            check(reentry.advance_seeded(0.05, error), error);
        check(reentryCloses == 1 && nestedResult && nestedFrames >= 2,
              "Seeded completion reentry replayed or rejected the already-retired end");
        check(reentry.advance_seeded(0, error) && reentryCloses == 1,
              "Seeded completion reappeared after callback reentry");

        // Callback failure is reported, but the completed seed is consumed;
        // the next call must not retry the same End.
        auto failureConfig = plan.config;
        failureConfig.motion_node_id = "auto";
        failureConfig.consume_root_motion = true;
        failureConfig.clips.clear();
        failureConfig.clips.emplace_back(died->clipName, died->resolvedPath);
        CharacterVisual failureVisual;
        check(failureVisual.load(assets, failureConfig, error), error);
        RetainedSequencePlayback failing(failureVisual, complete_pose_root_sampler(failureVisual));
        RetainedAnimationFrame failureSeed;
        unsigned failedCalls = 0;
        RetainedSequenceServices failureServices;
        failureServices.frame = [](std::size_t, const RetainedAnimationFrame&, std::string&) { return true; };
        failureServices.seeded_closed = [&](const dh2::timeline::Completion&, std::string& problem) {
            ++failedCalls;
            problem = "intentional seeded completion failure";
            return false;
        };
        check(failing.prepare_seeded(assets, plan, *died, std::move(failureServices), error), error);
        check(failing.seed_sequence(assets, died->clipName, died->resolvedPath,
              static_cast<float>(died->speed), static_cast<std::int32_t>(died->blendOut),
              died->moveGO != 0, 0, failureSeed, error), error);
        bool failed = false;
        for (unsigned frame = 0; frame < 2000 && !failed; ++frame)
            failed = !failing.advance_seeded(0.05, error);
        check(failed && failedCalls == 1 && error == "intentional seeded completion failure",
              "Seeded completion callback failure was not reported exactly once");
        check(failing.advance_seeded(0, error) && failedCalls == 1,
              "Failed seeded completion callback replayed on the next frame");

        auto rawConfig = plan.config;
        rawConfig.motion_node_id = "auto";
        rawConfig.consume_root_motion = true;
        rawConfig.clips.clear();
        rawConfig.clips.emplace_back(walk->clipName, walk->resolvedPath);
        CharacterVisual rawVisual;
        check(rawVisual.load(assets, rawConfig, error), error);
        RetainedSequencePlayback raw(rawVisual, complete_pose_root_sampler(rawVisual));
        RetainedAnimationFrame rawInitial;
        unsigned rawClosed = 0;
        RetainedSequenceServices rawServices;
        rawServices.frame = [](std::size_t, const RetainedAnimationFrame&, std::string&) { return true; };
        rawServices.seeded_closed = [&](const dh2::timeline::Completion&, std::string&) {
            ++rawClosed;
            return true;
        };
        check(raw.prepare_seeded(assets, plan, *walk, std::move(rawServices), error), error);
        check(raw.seed(assets, walk->clipName, walk->resolvedPath, true,
              static_cast<float>(walk->speed), static_cast<std::int32_t>(walk->blendOut),
              walk->moveGO != 0, rawInitial, error), error);
        for (unsigned frame = 0; frame < 100; ++frame)
            check(raw.advance_seeded(0.1, error), error);
        check(rawClosed == 0 && !raw.animation()->pose().current_ended(),
              "Raw timeline loop incorrectly emitted a finite seeded completion");

        std::cout << "PASS finite seed completion, finite repeats, infinite repeat and raw-loop suppression\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "Retained seeded completion FAIL: " << failure.what() << '\n';
        return 1;
    }
}
