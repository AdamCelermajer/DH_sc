#include "source_script_fx_adapter_v1.hpp"

#include <array>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace dh::foundation::effects;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
struct Call {
    std::int32_t set;
    std::array<float, 3> position;
    std::uintptr_t anchor;
};
}

int main() {
    try {
        std::string error = "stale";
        const std::array<float, 3> before{0.f, 1.f, 0.f};
        const std::array<float, 3> after{25.f, 1.f, -12.f};
        std::vector<Call> calls;
        SourceScriptFxPlaySetV1 play = [&](std::int32_t set, const float p[3],
                std::uintptr_t anchor, std::string&) {
            calls.push_back({set, {p[0], p[1], p[2]}, anchor});
            return true;
        };

        // Source skip branch: no call and stale error is cleared.
        check(dispatch_source_play_effect_v1(true, 132, before.data(), 77, play, error) ==
                  SourceScriptFxResultV1::skipped, "skip result");
        check(calls.empty() && error.empty(), "skip must make no call");

        // Authored two-cue transition: preserve order and do not deduplicate
        // the set when the caller has moved the actor between source commands.
        check(dispatch_source_play_effect_v1(false, 132, before.data(), 77, play, error) ==
                  SourceScriptFxResultV1::played, "pre-transition result");
        check(dispatch_source_play_effect_v1(false, 132, after.data(), 77, play, error) ==
                  SourceScriptFxResultV1::played, "post-transition result");
        check(calls.size() == 2, "both authored calls must be dispatched");
        check(calls[0].set == 132 && calls[1].set == 132, "source set is forwarded exactly");
        check(calls[0].position == before && calls[1].position == after,
              "source positions retain their order across relocation");
        check(calls[0].anchor == 77 && calls[1].anchor == 77,
              "resolved object anchor is forwarded");

        check(dispatch_source_play_effect_v1(false, 132, nullptr, 77, play, error) ==
                  SourceScriptFxResultV1::failed && !error.empty(), "missing position fails closed");
        check(dispatch_source_play_effect_v1(false, 132, before.data(), 77, {}, error) ==
                  SourceScriptFxResultV1::failed && !error.empty(), "missing play seam fails closed");

        SourceScriptFxPlaySetV1 failing = [](std::int32_t, const float*, std::uintptr_t,
                std::string& e) { e = "original asset missing"; return false; };
        check(dispatch_source_play_effect_v1(false, 132, before.data(), 77, failing, error) ==
                  SourceScriptFxResultV1::failed && error == "original asset missing",
              "play failure is returned without replacement art");

        SourceScriptFxPlaySetV1 failing_without_reason = [](std::int32_t, const float*,
                std::uintptr_t, std::string&) { return false; };
        check(dispatch_source_play_effect_v1(false, 132, before.data(), 77,
                  failing_without_reason, error) == SourceScriptFxResultV1::failed &&
                  !error.empty(), "failed play gets explicit diagnostic");

        SourceScriptFxPlaySetV1 reject_invalid_set = [](std::int32_t set, const float*,
                std::uintptr_t, std::string& e) {
            if (set < 0 || set >= 276) { e = "set outside active EffectsTables"; return false; }
            return true;
        };
        check(dispatch_source_play_effect_v1(false, 276, before.data(), 77,
                  reject_invalid_set, error) == SourceScriptFxResultV1::failed &&
                  error == "set outside active EffectsTables",
              "an out-of-range rejection returned by the play callback is surfaced");
        std::cout << "PASS | source Script_PlayEffect adapter branches and duplicate cues\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL | " << e.what() << '\n';
        return 1;
    }
}
