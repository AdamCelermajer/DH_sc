#include "../character_kill_npc_provider_v1.hpp"
#include "../../game-data/properties.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace tail = dh2::character_kill_death_tail_v1;
namespace ctrl = dh2::player_kill_continuation_v1;
namespace npc = dh2::character_kill_npc_provider_v1;
namespace {
constexpr std::uintptr_t character = 0x101;
constexpr std::uintptr_t killer = 0x202;
constexpr std::uintptr_t killer_character = 0x303;
enum class Step { is_dead, preflight, dead, hp_zero, is_player, drop,
                  credit, convert, xp, virtual54, field14e4, objective, event2,
                  player_branch };

void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    std::vector<Step> trace;
    bool player = false;
    bool fail_event2 = false;
    std::uint8_t field = 0;
    std::uint32_t* dead = nullptr;
    dh2::data::PropertyView* properties = nullptr;
    std::uintptr_t expected_killer = killer;
    unsigned xp_calls = 0;
    std::array<dh2::character_ai_classification::AiRow, 1> ai_rows{{{0, 6}}};
    dh2::character_ai_classification::AiTable ai_table{ai_rows.data(), 1};

    static bool preflight(void* raw, std::uintptr_t c, std::uintptr_t,
                          std::uint32_t, std::string&) {
        auto& f = *static_cast<Fixture*>(raw);
        check(c == character, "preflight Character changed");
        f.trace.push_back(Step::preflight);
        return true;
    }
    static std::int32_t classify(void* raw,
            dh2::character_ai_classification::State* state,
            const dh2::character_ai_classification::Request* request,
            dh2::character_ai_classification::Response* response) {
        auto& f = *static_cast<Fixture*>(raw);
        check(state && state->character == character && request && response,
              "source Character classification identity changed");
        using Operation = dh2::character_ai_classification::Operation;
        if (request->operation == Operation::ai_table) {
            f.trace.push_back(Step::is_player);
            if (f.player) f.ai_rows[0].type = 1;
            response->table = &f.ai_table;
            return 0;
        }
        if (request->operation == Operation::ai_count) {
            response->count = 1;
            return 0;
        }
        if (request->operation == Operation::find_player_name) {
            response->match = nullptr;
            return 0;
        }
        return 1;
    }
    static bool player_branch(void* raw, std::uintptr_t c, std::uintptr_t,
                              std::uint32_t, std::string&) {
        auto& f = *static_cast<Fixture*>(raw);
        check(c == character, "player branch Character changed");
        f.trace.push_back(Step::player_branch);
        return true;
    }
    static bool event2(void* raw, std::uintptr_t c, std::uintptr_t k,
                       std::string& error) {
        auto& f = *static_cast<Fixture*>(raw);
        check(c == character && k == f.expected_killer, "event2 identities changed");
        f.trace.push_back(Step::event2);
        if (f.fail_event2) { error = "event2 failure"; return false; }
        return true;
    }
    static bool tail_preflight(void* raw, const tail::Request&, std::string&) {
        static_cast<Fixture*>(raw)->trace.push_back(Step::preflight);
        return true;
    }
    static bool drop(void* raw, std::uintptr_t c, std::uintptr_t k,
                     std::string&) {
        check(c == character && k == static_cast<Fixture*>(raw)->expected_killer,
              "DropLoot identities changed");
        static_cast<Fixture*>(raw)->trace.push_back(Step::drop);
        return true;
    }
    static bool credit(void* raw, std::uintptr_t c, std::uintptr_t k,
                       std::string&) {
        check(c == character && k == static_cast<Fixture*>(raw)->expected_killer,
              "killer credit changed");
        static_cast<Fixture*>(raw)->trace.push_back(Step::credit);
        return true;
    }
    static bool convert(void* raw, std::uintptr_t c, std::uintptr_t k,
                        std::uintptr_t& out, bool& award, std::string&) {
        auto& f = *static_cast<Fixture*>(raw);
        check(c == character && k == f.expected_killer, "killer conversion inputs changed");
        f.trace.push_back(Step::convert);
        out = killer_character;
        award = true;
        return true;
    }
    static bool xp(void* raw, std::uintptr_t recipient, std::uintptr_t victim,
                   std::string&) {
        auto& f = *static_cast<Fixture*>(raw);
        check(recipient == killer_character && victim == character,
              "DistributeXP identities changed");
        f.trace.push_back(Step::xp);
        ++f.xp_calls;
        return true;
    }
    static bool virtual54(void* raw, std::uintptr_t c, std::int32_t& result,
                          std::string&) {
        check(c == character, "virtual+0x54 Character changed");
        static_cast<Fixture*>(raw)->trace.push_back(Step::virtual54);
        result = 0;
        return true;
    }
    static bool read_field(void* raw, std::uintptr_t c, std::uint8_t& value,
                           std::string&) {
        auto& f = *static_cast<Fixture*>(raw);
        check(c == character, "Character+0x14e4 identity changed");
        f.trace.push_back(Step::field14e4);
        value = f.field;
        return true;
    }
    static bool objective(void* raw, std::uintptr_t c, std::uintptr_t k,
                          std::int32_t result, std::uint8_t field,
                          std::string&) {
        check(c == character && k == static_cast<Fixture*>(raw)->expected_killer &&
              result == 0 && field == 0,
              "objective gates changed");
        static_cast<Fixture*>(raw)->trace.push_back(Step::objective);
        return true;
    }
    static bool uncalled_null_credit(void*, std::uintptr_t, std::uintptr_t,
                                     std::string&) { return false; }
    static bool uncalled_null_convert(void*, std::uintptr_t, std::uintptr_t,
                                      std::uintptr_t&, bool&, std::string&) {
        return false;
    }
    static bool raise_for_test(void* raw, std::uintptr_t c, std::uintptr_t k,
                               std::string& error) {
        return event2(raw, c, k, error);
    }
    static int backend(void* raw, const ctrl::Request* q, ctrl::Reply* reply,
                       std::string& error) {
        auto& f = *static_cast<Fixture*>(raw);
        if (q->operation == ctrl::Operation::is_dead) {
            check(q->subject == character, "outer IsDead receiver changed");
            f.trace.push_back(Step::is_dead);
            reply->word = static_cast<std::int32_t>(*f.dead);
            return 0;
        }
        // The provider under test is the CtrlCaller backend; its own nested
        // dispatch handles all three source operations.
        error = "unexpected fixture backend request";
        return 1;
    }
};

dh2::data::PropertyRules rules() {
    dh2::data::PropertyRules value{};
    value.types.fill(8);
    return value;
}

} // namespace

int main() {
    try {
        auto property_rules = rules();
        dh2::data::PropertyState property_state{};
        property_state.resolved.fill(0);
        property_state.resolved[36] = 99;
        auto properties = dh2::data::property_view(property_rules, property_state);
        std::uint32_t dead = 0;
        Fixture fixture{};
        fixture.dead = &dead;
        fixture.properties = &properties;
        dh2::character_ai_classification::State classification_state{
            character, 0, 0, "NPC_Ghoul", 0};
        const dh2::character_ai_classification::Services classification_services{
            &fixture, Fixture::classify};

        tail::Services tail_services{&fixture, Fixture::tail_preflight,
            Fixture::drop, Fixture::credit, Fixture::convert, Fixture::xp,
            Fixture::virtual54, Fixture::read_field, Fixture::objective};
        npc::Services services{};
        services.context = &fixture;
        services.preflight = Fixture::preflight;
        services.player_branch = Fixture::player_branch;
        services.raise_event2 = Fixture::raise_for_test;
        services.general_tail = tail_services;
        npc::Runtime runtime({character, &dead, &properties,
            &classification_state, &classification_services, services});
        auto& caller = runtime.caller();
        ctrl::Result result{};
        std::string error;
        check(caller.kill(killer, 0, &result, error) == ctrl::Status::complete &&
              result.kill_completed && result.event2_completed && dead == 1 &&
              property_state.resolved[36] == 0 && fixture.xp_calls == 1,
              "NPC full Kill did not complete canonical state/XP/event2 stages");
        const std::vector<Step> expected{
            Step::preflight, Step::is_player,
            Step::preflight, Step::drop, Step::credit, Step::convert, Step::xp,
            Step::virtual54, Step::field14e4, Step::objective, Step::event2};
        check(fixture.trace == expected && runtime.snapshot().inner_is_dead_reads == 2,
              "NPC full Kill did not preserve Ctrl_Kill/Kill source order");

        const auto prefix = fixture.trace.size();
        check(caller.kill(killer, 0, &result, error) == ctrl::Status::complete &&
              result.skipped && fixture.trace.size() == prefix &&
              fixture.xp_calls == 1,
              "repeated NPC ingress replayed full Character::Kill");

        std::uint32_t second_dead = 0;
        dh2::data::PropertyState second_state{};
        second_state.resolved.fill(0);
        auto second_properties = dh2::data::property_view(property_rules, second_state);
        Fixture null_killer{};
        null_killer.dead = &second_dead;
        null_killer.properties = &second_properties;
        null_killer.expected_killer = 0;
        dh2::character_ai_classification::State null_classification_state{
            character, 0, 0, "NPC_Ghoul", 0};
        const dh2::character_ai_classification::Services null_classification_services{
            &null_killer, Fixture::classify};
        auto null_tail_services = tail::Services{&null_killer,
            Fixture::tail_preflight, Fixture::drop, Fixture::uncalled_null_credit,
            Fixture::uncalled_null_convert, Fixture::xp, Fixture::virtual54,
            Fixture::read_field, Fixture::objective};
        auto null_services = npc::Services{};
        null_services.context = &null_killer;
        null_services.preflight = Fixture::preflight;
        null_services.player_branch = Fixture::player_branch;
        null_services.raise_event2 = Fixture::raise_for_test;
        null_services.general_tail = null_tail_services;
        npc::Runtime null_runtime({character, &second_dead, &second_properties,
            &null_classification_state, &null_classification_services,
            null_services});
        check(null_runtime.caller().kill(0, 0, &result, error) ==
                  ctrl::Status::complete && result.kill_completed &&
              null_killer.xp_calls == 0 && second_dead == 1 &&
              null_killer.trace == std::vector<Step>({Step::preflight,
                  Step::is_player,
                  Step::preflight, Step::drop, Step::virtual54,
                  Step::field14e4, Step::objective, Step::event2}),
              "null killer did not skip aggro/XP while reaching objective/event2 tails");
        std::cout << "CHARACTER KILL NPC PROVIDER PASS 3\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
