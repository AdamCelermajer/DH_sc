#include "../player_enemy_kill_credit_v1.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace ch = dh2::character;
namespace credit = dh2::player_enemy_kill_credit_v1;
namespace data = dh2::data;

namespace {
constexpr std::uintptr_t PLAYER = 0x100001ull;
constexpr std::uintptr_t CHAR_AI = 0x200002ull;
constexpr std::uintptr_t AIS = 0x300003ull;
constexpr std::uintptr_t CONTROLLER = 0x400004ull;
constexpr std::uintptr_t MACHINE = 0x500005ull;
constexpr std::uintptr_t VICTIM = 0x600006ull;

void check(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}

struct Fixture {
    data::PropertyRules rules{};
    data::PropertyState properties{};
    data::PropertyView view{};
    data::AggroEntry entry{PLAYER, 0x3f800000u, 0};
    std::array<data::AggroEntry, 2> extra{{entry, {0x700007ull, 0x3f800000u, 0}}};
    data::AggroTable aggro{&entry, 1, 1};
    std::uintptr_t current_target = VICTIM;
    std::vector<unsigned> trace;
    credit::Runtime* runtime = nullptr;
    bool service_fails = false;
    bool clear_fails = false;
    bool reenter = false;
    unsigned service_calls = 0;

    Fixture() {
        rules.defaults.fill(0);
        rules.types.fill(-1);
        rules.types[23] = 32;
        rules.types[24] = 32;
        data::reset_properties(rules, properties);
        properties.saved[23] = 2 * 256;
        properties.saved[24] = 7 * 256;
        std::string error;
        check(data::recalc_properties(rules, properties, error), "property setup failed");
        view = data::property_view(rules, properties);
    }

    static int backend(void* raw, ch::AIEventState64* state,
                       const ch::AIEventRequest40* request, std::uint32_t*) {
        auto& self = *static_cast<Fixture*>(raw);
        ++self.service_calls;
        check(state && state->ai == CHAR_AI && state->active == AIS,
              "event service did not retain the live CharAI/active AIS");
        check(request && request->event == 4 && request->payload == VICTIM,
              "event4 payload changed before the selected source service");
        check(self.properties.saved[23] == 2 * 256 && self.properties.saved[24] == 7 * 256,
              "Player property credit ran before the event4 source service returned");
        if (request->service == ch::ai_event_ais_virtual) {
            check(request->operation == 0xb0 && request->subject == AIS &&
                  request->callee == credit::ais_player_on_kill_identity,
                  "CharAI::OnKill did not relay to AISPlayer::OnKill");
            self.trace.push_back(1);
        } else {
            check(request->service == ch::ai_event_state_event && !request->operation &&
                  request->subject == MACHINE && !request->callee,
                  "blocked event4 did not reach the current Coordinator");
            self.trace.push_back(3);
        }
        if (self.reenter) {
            self.reenter = false;
            credit::Result nested{};
            nested.event4_completed = 88;
            std::string nested_error = "sentinel";
            check(self.runtime->after_loot_attempt(VICTIM, PLAYER, 0, &nested, nested_error) == credit::Status::busy,
                  "reentrant kill-credit delivery was not blocked");
            check(nested.event4_completed == 88 && nested_error == "sentinel",
                  "busy reentry changed its outputs");
        }
        return self.service_fails ? 1 : 0;
    }

    static int clear_target(void* raw, std::uintptr_t victim) {
        auto& self = *static_cast<Fixture*>(raw);
        check(victim == VICTIM && self.current_target == 0,
              "renderer projection ran before the direct source target clear");
        check(self.properties.saved[23] == 2 * 256 && self.properties.saved[24] == 7 * 256,
              "property23/24 ran before matching target clear");
        self.trace.push_back(2);
        return self.clear_fails ? 1 : 0;
    }

    credit::Bindings bindings() {
        return {PLAYER, CHAR_AI, AIS, CONTROLLER, MACHINE,
                reinterpret_cast<std::uintptr_t>(&properties), 0, 0, 0, 0,
                &aggro, &view, &current_target, this, clear_target,
                {this, backend, (1u << ch::ai_event_state_event) |
                                (1u << ch::ai_event_ais_virtual), 0}};
    }
};

void normal_kill_path() {
    Fixture f;
    credit::Runtime runtime(f.bindings());
    f.runtime = &runtime;
    f.reenter = true;
    credit::Result result{};
    std::string error;
    check(runtime.after_loot_attempt(VICTIM, PLAYER, 0, &result, error) == credit::Status::complete,
          "single Player kill episode failed");
    check(result.event4_reached == 1 && result.event4_status == 0 &&
          result.event4_completed == 1 && result.target_cleared == 1 &&
          result.target_projection_attempted == 1 && result.target_projection_status == 0 &&
          result.property23_added == 1 && result.property24_added == 1,
          "kill episode result missed a completed source step");
    check(result.dispatch.last_service == ch::ai_event_virtual && result.dispatch.service_calls == 1,
          "Character::RaiseEvent(4) did not use the existing dispatcher virtual slot");
    check(f.trace == std::vector<unsigned>{1, 2}, "event4/target-clear order changed");
    check(f.current_target == 0 && f.properties.saved[23] == 3 * 256 &&
          f.properties.saved[24] == 8 * 256 && f.properties.resolved[23] == 3 * 256 &&
          f.properties.resolved[24] == 8 * 256,
          "AddInt(23,1)/AddInt(24,1) did not update the canonical property sheets");
    result.event4_completed = 77;
    error = "sentinel";
    check(runtime.after_loot_attempt(VICTIM, PLAYER, 0, &result, error) == credit::Status::consumed,
          "same kill episode was delivered twice");
    check(result.event4_completed == 77 && error == "sentinel",
          "consumed retry changed outputs");
}

void aggro_scope_and_state_machine_fallback() {
    Fixture f;
    f.aggro = {f.extra.data(), 2, 2};
    credit::Runtime excluded(f.bindings());
    credit::Result untouched{};
    untouched.event4_completed = 91;
    std::string error = "unchanged";
    check(excluded.after_loot_attempt(VICTIM, PLAYER, 0, &untouched, error) == credit::Status::ineligible_aggro,
          "multi-recipient outgoing aggro was treated as supported Player-only credit");
    check(untouched.event4_completed == 91 && error == "unchanged" && f.service_calls == 0,
          "unsupported aggro episode produced effects");

    f.aggro = {&f.entry, 1, 1};
    f.trace.clear();
    f.current_target = VICTIM;
    auto bindings = f.bindings();
    bindings.locked = 1; // Existing RaiseAIEvent gate routes event4 to the Coordinator.
    credit::Runtime fallback(bindings);
    credit::Result result{};
    error.clear();
    check(fallback.after_loot_attempt(VICTIM, PLAYER, 0, &result, error) == credit::Status::complete,
          "source blocked-event state-machine fallback failed");
    check(f.service_calls == 1 && f.trace == std::vector<unsigned>{3, 2},
          "blocked event4 was sent through the AIS callback instead of Coordinator");
    check(result.dispatch.last_service == ch::ai_event_state_event &&
          result.dispatch.service_calls == 1 && result.target_cleared == 1 &&
          f.properties.saved[23] == 3 * 256 && f.properties.saved[24] == 8 * 256,
          "state-machine event4 did not preserve later Kill property order");
}

void failures_preserve_reached_prefix() {
    Fixture event_failure;
    event_failure.service_fails = true;
    credit::Runtime failed_event(event_failure.bindings());
    credit::Result result{};
    std::string error;
    check(failed_event.after_loot_attempt(VICTIM, PLAYER, 0, &result, error) == credit::Status::complete,
          "Kill incorrectly aborted after ignored RaiseEvent failure");
    check(result.event4_reached == 1 && result.event4_status != 0 &&
          result.event4_completed == 0 && result.target_cleared == 1 &&
          result.property23_added == 1 && result.property24_added == 1 &&
          event_failure.properties.saved[23] == 3 * 256 && event_failure.properties.saved[24] == 8 * 256 &&
          event_failure.current_target == 0,
          "RaiseEvent error did not preserve source Kill continuation and order");

    Fixture clear_failure;
    clear_failure.clear_fails = true;
    credit::Runtime failed_clear(clear_failure.bindings());
    result = {};
    error.clear();
    check(failed_clear.after_loot_attempt(VICTIM, PLAYER, 0, &result, error) == credit::Status::complete,
          "renderer projection error incorrectly aborted Kill");
    check(result.event4_completed == 1 && result.target_cleared == 1 &&
          result.target_projection_status != 0 &&
          result.property23_added == 1 && result.property24_added == 1 &&
          clear_failure.current_target == 0 &&
          clear_failure.properties.saved[23] == 3 * 256 && clear_failure.properties.saved[24] == 8 * 256,
          "renderer projection failure blocked direct target clear or Kill property credit");
}

void source_kill_guard_and_null_active_ais() {
    Fixture null_killer;
    credit::Runtime no_killer(null_killer.bindings());
    credit::Result untouched{};
    untouched.event4_reached = 61;
    std::string error = "unchanged";
    check(no_killer.after_loot_attempt(VICTIM, 0, 0, &untouched, error) == credit::Status::ineligible_kill,
          "null killer did not suppress Kill aggro/event4 continuation");
    check(untouched.event4_reached == 61 && error == "unchanged" &&
          null_killer.service_calls == 0 && null_killer.current_target == VICTIM &&
          null_killer.properties.saved[23] == 2 * 256 && null_killer.properties.saved[24] == 7 * 256,
          "null-killer guard produced source effects");

    Fixture forced;
    credit::Runtime forced_kill(forced.bindings());
    check(forced_kill.after_loot_attempt(VICTIM, PLAYER, 1, &untouched, error) == credit::Status::ineligible_kill,
          "nonzero Kill force did not suppress aggro/event4 continuation");
    check(forced.service_calls == 0 && forced.current_target == VICTIM &&
          forced.properties.saved[23] == 2 * 256 && forced.properties.saved[24] == 7 * 256,
          "Kill force was confused with recipient controller forced or produced effects");

    Fixture no_active;
    auto b = no_active.bindings();
    b.active_ais = 0;
    b.backend.available = 0; // CharAI::OnKill returns before the AIS virtual.
    credit::Runtime empty_ais(b);
    credit::Result result{};
    check(empty_ais.after_loot_attempt(VICTIM, PLAYER, 0, &result, error) == credit::Status::complete,
          "null active AIS should preserve CharAI::OnKill's source no-op");
    check(result.event4_completed == 1 && result.dispatch.service_calls == 1 &&
          no_active.service_calls == 0 && result.property23_added && result.property24_added,
          "null active AIS prevented later Kill credit");
}
}

int main() {
    normal_kill_path();
    aggro_scope_and_state_machine_fallback();
    failures_preserve_reached_prefix();
    source_kill_guard_and_null_active_ais();
    std::puts("PASS: source Kill gate -> event4 (ignored return) -> direct target clear -> canonical AddInt(23/24); sole Player recipient, FSM fallback, null AIS, and one-shot retry guard");
}
