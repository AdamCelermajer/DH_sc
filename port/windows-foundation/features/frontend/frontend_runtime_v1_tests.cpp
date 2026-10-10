#include "frontend_runtime_v1.hpp"

#include <iostream>
#include <memory>
#include <string>

using namespace dh::foundation::frontend;

int main() {
    unsigned checks = 0;
    auto check = [&](bool ok, const char* label) {
        ++checks;
        if (!ok) {
            std::cerr << "FAIL: " << label << '\n';
            return false;
        }
        return true;
    };

    const flow::SlotFact rogue_slot{1, true, "isolated/profile-1.save"};
    for (const char* class_id : {"KnightPlayerBase", "RoguePlayerBase", "MagePlayerBase"}) {
        auto state = std::make_shared<dh::foundation::CharacterState>();
        state->id = std::string("profile-") + class_id;
        state->class_id = class_id;
        FrontendSelectedProfileSnapshotV1 snapshot{rogue_slot, state};
        if (!check(snapshot.valid_for(rogue_slot), "same-save selected profile snapshot admits each authored class")) return 1;
    }
    auto selected_state = std::make_shared<dh::foundation::CharacterState>();
    selected_state->id = "rogue-profile";
    selected_state->class_id = "RoguePlayerBase";
    FrontendSelectedProfileSnapshotV1 selected_snapshot{rogue_slot, selected_state};
    if (!check(!selected_snapshot.valid_for(flow::SlotFact{2, true, rogue_slot.save_path}),
               "selected-profile snapshot rejects a neighboring slot identity")) return 1;
    if (!check(!selected_snapshot.valid_for(flow::SlotFact{1, true, "isolated/other.save"}),
               "selected-profile snapshot rejects a mismatched save path")) return 1;
    if (!check(!selected_snapshot.valid_for(flow::SlotFact{1, false, rogue_slot.save_path}),
               "selected-profile snapshot rejects an empty selected slot")) return 1;
    auto unidentified = std::make_shared<dh::foundation::CharacterState>();
    unidentified->class_id = "RoguePlayerBase";
    if (!check(!FrontendSelectedProfileSnapshotV1{rogue_slot, unidentified}.valid_for(rogue_slot),
               "selected-profile snapshot rejects missing same-save Character identity")) return 1;

    FrontendRunConfigV1 production_config;
    FrontendElapsedClockV1 elapsed_clock;
    elapsed_clock.reset(10.0);
    if (!check(!production_config.fixed_step_enabled &&
               frontend_frame_elapsed_milliseconds_v1(production_config, elapsed_clock, 10.0004) == 0,
               "production clock is real-time by default and carries sub-millisecond delta")) return 1;
    if (!check(frontend_frame_elapsed_milliseconds_v1(production_config, elapsed_clock, 9.0) == 0,
               "a regressed wall clock sample never advances source animation")) return 1;
    if (!check(frontend_frame_elapsed_milliseconds_v1(production_config, elapsed_clock, 10.0009) == 0,
               "fractional source milliseconds remain carried")) return 1;
    if (!check(frontend_frame_elapsed_milliseconds_v1(production_config, elapsed_clock, 10.0012) == 1,
               "carried source fraction becomes one elapsed millisecond")) return 1;
    if (!check(frontend_frame_elapsed_milliseconds_v1(production_config, elapsed_clock, 10.0171) == 16,
               "source update receives measured frame milliseconds instead of default fixed-step playback")) return 1;
    if (!check(frontend_frame_elapsed_milliseconds_v1(production_config, elapsed_clock, 10.0272) == 10,
               "source clock tracks a short measured frame with carry")) return 1;
    if (!check(frontend_frame_elapsed_milliseconds_v1(production_config, elapsed_clock, 10.5272) == 500,
               "source animation advances by actual half-second elapsed time")) return 1;

    elapsed_clock.reset(30.0);
    if (!check(frontend_frame_elapsed_milliseconds_v1(production_config, elapsed_clock, 30.0165) == 16,
               "real-time frame before class asset load uses measured elapsed time")) return 1;
    frontend_rebase_after_blocking_load_v1(production_config, elapsed_clock, 35.0165);
    if (!check(frontend_frame_elapsed_milliseconds_v1(production_config, elapsed_clock, 35.033) == 16,
               "blocking class asset load is excluded from the next real-time animation sample")) return 1;

    FrontendRunConfigV1 diagnostic_config;
    diagnostic_config.fixed_step_enabled = true;
    diagnostic_config.fixed_step_seconds = 0.016;
    elapsed_clock.reset(20.0);
    if (!check(frontend_frame_elapsed_milliseconds_v1(diagnostic_config, elapsed_clock, 20.500) == 16 &&
               frontend_frame_elapsed_milliseconds_v1(diagnostic_config, elapsed_clock, 21.000) == 16,
               "explicit diagnostics fixed-step override ignores wall-clock playback speed")) return 1;
    frontend_rebase_after_blocking_load_v1(diagnostic_config, elapsed_clock, 26.000);
    if (!check(frontend_frame_elapsed_milliseconds_v1(diagnostic_config, elapsed_clock, 26.500) == 16,
               "blocking class asset load does not alter explicit fixed-step diagnostics")) return 1;

    FrontendInputFocusGateV1 focusGate;
    if (!check(!focusGate.should_bind_host(false) && !focusGate.input_enabled(false) &&
               !focusGate.observe(false),
               "unfocused existing window is render-attachable without binding physical input")) return 1;
    if (!check(focusGate.should_bind_host(true),
               "native event host binds only after actual window focus")) return 1;
    focusGate.host_bound();
    focusGate.observe(true);
    if (!check(focusGate.input_enabled(true) && !focusGate.should_bind_host(true),
               "focused host admits input and binds once")) return 1;
    if (!check(focusGate.observe(false) && !focusGate.input_enabled(false) &&
               focusGate.take_focus_lost() && !focusGate.take_focus_lost(),
               "focus loss disables input and publishes one cancellation edge")) return 1;
    if (!check(!focusGate.observe(false) && !focusGate.observe(true) &&
               focusGate.input_enabled(true) && !focusGate.take_focus_lost(),
               "focus regain resumes input without rebinding or repeating cancellation")) return 1;
    auto queued = frontend_filter_host_events_v1(
        {HostEvent{HostEvent::Kind::key, 13, true}}, true, false, false);
    if (!check(queued.empty(), "queued physical input is discarded while the window is unfocused")) return 1;
    queued = frontend_filter_host_events_v1(
        {HostEvent{HostEvent::Kind::key, 13, true}}, true, true, true);
    if (!check(queued.size() == 1 && queued.front().kind == HostEvent::Kind::focus_lost,
               "focus edge cancels captures and discards the entire stale event batch")) return 1;
    queued = frontend_filter_host_events_v1(
        {HostEvent{HostEvent::Kind::key, 13, true}, HostEvent{HostEvent::Kind::focus_lost}},
        true, true, false);
    if (!check(queued.size() == 1 && queued.front().kind == HostEvent::Kind::focus_lost,
               "native focus-lost event suppresses coalesced physical input")) return 1;

    auto* address = reinterpret_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(
        static_cast<std::uintptr_t>(0x1000));
    auto record = std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(
        address, [](dh2::world::CanonicalCharacterCandidateRecordV60*) {});
    FrontendProfileLoanV1 a{record}, b{record};
    if (!check(a.valid() && a.same_as(b), "same canonical record and control block")) return 1;

    FrontendProfileLoanV1 same_address_different_control{
        std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(
            record.get(), [](dh2::world::CanonicalCharacterCandidateRecordV60*) {})};
    if (!check(!a.same_as(same_address_different_control),
               "same address with foreign ownership rejected")) return 1;

    FrontendRuntimeResultV1 result;
    result.outcome = FrontendRuntimeOutcomeV1::gameplay_handoff_ready;
    result.selected_slot = 2;
    result.selected_profile = a;
    if (!check(!result.gameplay_ready(), "profile loan alone is not gameplay readiness")) return 1;

    FrontendSourceStartReceiptV1 receipt;
    receipt.selected_profile = b;
    receipt.selected_slot = 2;
    receipt.source_assign_receipt = std::make_shared<int>(1);
    receipt.source_start_receipt = std::make_shared<int>(2);
    result.source_receipt = receipt;
    result.source_receipt->selected_slot = 1;
    if (!check(!result.gameplay_ready(), "mismatched source receipt slot rejected before record readiness")) return 1;

    result.outcome = FrontendRuntimeOutcomeV1::source_operation_failed;
    result.source_receipt.reset();
    if (!check(!result.gameplay_ready(), "source failure never becomes gameplay handoff")) return 1;

    auto generic_state = std::make_shared<dh::foundation::CharacterState>();
    FrontendRuntimeResultV1 generic;
    generic.outcome = FrontendRuntimeOutcomeV1::generic_gameplay_started;
    generic.selected_slot = 4;
    generic.generic_start_receipt = creation::FrontendGenericStartReceiptV1{
        generic_state, "new-slot-4.savestate", 4, true, true};
    if (!check(generic.generic_gameplay_ready() && !generic.gameplay_ready(),
               "generic same-state start is distinct from strict native profile readiness")) return 1;
    generic.generic_start_receipt->selected_slot = 3;
    if (!check(!generic.generic_gameplay_ready(), "generic save slot mismatch rejected")) return 1;
    generic.generic_start_receipt->selected_slot = 4;
    generic.generic_start_receipt->start_delivered = false;
    if (!check(!generic.generic_gameplay_ready(), "generic start receipt required after Confirm")) return 1;
    generic.generic_start_receipt->start_delivered = true;
    generic.outcome = FrontendRuntimeOutcomeV1::quit;
    if (!check(!generic.generic_gameplay_ready(), "quit never publishes a generic gameplay start")) return 1;

    std::cout << "PASS frontend runtime V1 contract checks=" << checks << '\n';
    return 0;
}
