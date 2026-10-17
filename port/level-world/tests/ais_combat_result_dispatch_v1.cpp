#include "../ais_combat_result_dispatch_v1.hpp"
#include <cassert>
#include <cstdint>
#include <vector>

using namespace dh2::ais_combat_result_dispatch_v1;
struct Call { std::uintptr_t ais, attacker, defender; Callback callback; };
struct Trace { std::vector<Call> calls; std::int32_t fail_at = -1; };

static std::int32_t invoke(void* raw, std::uintptr_t ais, Callback callback,
                           std::uintptr_t attacker, std::uintptr_t defender) {
    auto& trace = *static_cast<Trace*>(raw);
    trace.calls.push_back({ais, attacker, defender, callback});
    return static_cast<std::int32_t>(trace.calls.size()) == trace.fail_at ? 1 : 0;
}

int main() {
    Trace trace;
    Services services{&trace, invoke};
    std::uint32_t attacker_flags = 0x800, defender_flags = 0x1000;
    Actor attacker{0x101, 0x201, &attacker_flags};
    Actor defender{0x102, 0x202, &defender_flags};
    data::CombatResult result{};
    Report report{};
    Arguments args{&attacker, &defender, &result};

    // Hit dispatches only the source OnTargetHit member, attacker first.
    assert(dispatch(&args, &services, &report) == Status::complete);
    assert(report.callbacks == 1 && report.selected == Callback::target_hit);
    assert(trace.calls.size() == 1 && trace.calls[0].ais == attacker.ais);

    // Any miss/dodge/block outcome selects OnTargetMissed for both owners,
    // with the original attacker/defender identities and source call order.
    trace.calls.clear(); result.outcomes = 2;
    assert(dispatch(&args, &services, &report) == Status::complete);
    assert(report.callbacks == 1 && report.selected == Callback::target_missed);
    assert(trace.calls.size() == 1 && trace.calls[0].ais == defender.ais);
    assert(trace.calls[0].attacker == attacker.character &&
           trace.calls[0].defender == defender.character);

    defender_flags = 0x1800; attacker_flags = 0x1800;
    trace.calls.clear(); result.outcomes = 0;
    assert(dispatch(&args, &services, &report) == Status::complete);
    assert(trace.calls.size() == 2 && trace.calls[0].ais == attacker.ais &&
           trace.calls[1].ais == defender.ais);

    // The F_ApplyResult suppression bit bypasses both callbacks.
    trace.calls.clear(); result.mask = 0x20000000;
    assert(dispatch(&args, &services, &report) == Status::complete);
    assert(trace.calls.empty() && report.callbacks == 0);

    // A source Lua error does not alter native OnCombatResults control flow:
    // the defender callback still follows the failed attacker callback.
    result.mask = 0; trace.calls.clear(); trace.fail_at = 1;
    assert(dispatch(&args, &services, &report) == Status::service_failed);
    assert(trace.calls.size() == 2 && report.attempts == 2 &&
           report.callbacks == 1 && report.failures == 1 && report.last_status == 1);
    assert(trace.calls[0].ais == attacker.ais && trace.calls[1].ais == defender.ais);

    // A missing dispatch provider is recorded independently for both source
    // owners; it must not hide the defender's required callback attempt.
    trace.calls.clear(); trace.fail_at = -1;
    Services unavailable{&trace, nullptr};
    assert(dispatch(&args, &unavailable, &report) == Status::service_failed);
    assert(trace.calls.empty() && report.attempts == 2 && report.callbacks == 0 &&
           report.failures == 2 &&
           report.last_status == static_cast<std::int32_t>(Status::service_unavailable));
}
