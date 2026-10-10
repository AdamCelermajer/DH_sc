#include "../player_character_kill_provider_v1.hpp"
#include "../../player-info-level/player_locality_v1.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace source = dh2::player_character_kill_provider_v1;
namespace ctrl = dh2::player_kill_continuation_v1;
namespace locality = dh2::player_locality_v1;
namespace {
constexpr std::uintptr_t CHARACTER = 0x100000001ull;
constexpr std::uintptr_t KILLER = 0x200000002ull;
enum class Step { is_player, add_death_count,
                  application, locality, trophy_manager, get_death_count,
                  trophy_id, trophy_unlock, online, on_died, event2 };

void check(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}

struct Fixture {
    std::array<std::int32_t, 224> defaults{}, types{}, base{}, saved{},
        gear{}, resolved{};
    dh2::data::PropertyView properties{};
    std::uint32_t dead = 0;
    std::vector<Step> trace;
    unsigned get_count = 0;
    unsigned event2_calls = 0;
    bool fail_event2 = false;
    locality::PlayerInfo player{0x610000006ull, 7, nullptr};
    locality::PlayerInfo fallback{0x620000006ull, -1, nullptr};
    locality::PlayerInfo* entries[1]{&player};
    locality::Registry registry{0x630000006ull, entries, 1, &fallback};
    locality::MatchingLocalFields matching_fields{};
    locality::Matching matching{0x640000006ull, &matching_fields.active_c};
    locality::Services locality_services{};
    locality::Result locality_result{};

    Fixture() {
        locality::construct_matching_local_fields(matching_fields);
        locality_services.context = this;
        locality_services.online = locality_online;
        locality_services.acquire_matching = acquire_matching;
        locality_services.character_660 = character_660;
        locality_services.player_virtual_is_local = player_virtual_is_local;
        locality_services.member_1a0 = member_1a0;
        locality_services.matching_member_id = matching_member_id;
        types[25] = 8;
        types[36] = 8;
        resolved[25] = 9 * 256;
        resolved[36] = 75 * 256;
        properties = {defaults.data(), types.data(), base.data(), saved.data(),
                      gear.data(), resolved.data(), nullptr, 0};
    }

    static int locality_online(void*, std::uint8_t* online) {
        *online = 0;
        return 0;
    }
    static int acquire_matching(void* raw, locality::Matching** out) {
        *out = &static_cast<Fixture*>(raw)->matching;
        return 0;
    }
    static int character_660(void*, locality::PlayerInfo* player,
                             std::uintptr_t* character) {
        check(player == nullptr || player->identity == 0x610000006ull,
              "locality resolved a different PlayerInfo");
        *character = CHARACTER;
        return 0;
    }
    static int player_virtual_is_local(void* raw, locality::PlayerInfo* player,
                                       std::int32_t* value) {
        auto& self = *static_cast<Fixture*>(raw);
        check(player == &self.player, "locality virtual receiver changed");
        const auto status = locality::cnet_player_is_local(
            player, &self.locality_services, &self.locality_result);
        if (status != locality::Status::complete) return 1;
        *value = self.locality_result.value;
        return 0;
    }
    static int member_1a0(void* raw, locality::PlayerInfo* player,
                          std::int32_t* member) {
        check(player == &static_cast<Fixture*>(raw)->player,
              "CNetPlayerInfo member receiver changed");
        *member = -1;
        return 0;
    }
    static int matching_member_id(void* raw, locality::Matching* matching,
                                  std::int32_t* member) {
        auto& self = *static_cast<Fixture*>(raw);
        check(matching == &self.matching, "local Matching receiver changed");
        *member = self.matching_fields.member_3638;
        return 0;
    }

    static int continuation(void* raw, const ctrl::Request* q,
                            ctrl::Reply* r, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        switch (q->operation) {
        case ctrl::Operation::is_player:
            check(q->subject == CHARACTER, "IsPlayer receiver changed");
            self.trace.push_back(Step::is_player); r->word = 1; break;
        case ctrl::Operation::property_add_int:
            check(q->index == 25 && q->argument == 1 &&
                  q->properties == &self.properties,
                  "PROPS_AddInt(25,1) request changed");
            self.trace.push_back(Step::add_death_count);
            check(!dh2_property_add(q->properties, 25, 256),
                  "death counter property update failed");
            break;
        case ctrl::Operation::application_player_manager:
            self.trace.push_back(Step::application);
            r->identity = 0x300000003ull; r->player_manager = 0x400000004ull;
            break;
        case ctrl::Operation::is_local_player:
            check(q->subject == 0x400000004ull, "locality manager changed");
            self.trace.push_back(Step::locality);
            check(locality::is_local_player(&self.registry, &self.locality_services,
                      CHARACTER, &self.locality_result) == locality::Status::complete &&
                  self.locality_result.route == locality::Route::registered_player &&
                  self.locality_result.registered,
                  "source PlayerManager local-player route did not resolve Prince");
            r->word = static_cast<std::uint32_t>(self.locality_result.value);
            break;
        case ctrl::Operation::trophy_manager:
            self.trace.push_back(Step::trophy_manager); r->identity = 0x500000005ull;
            break;
        case ctrl::Operation::property_get_int:
            check(q->index == 25 && q->argument == 0,
                  "PROPS_GetInt(25,false) request changed");
            self.trace.push_back(Step::get_death_count);
            r->word = self.resolved[25] / 256; ++self.get_count; break;
        case ctrl::Operation::trophy_id:
            check(q->name && std::string(q->name) == "quest_died_10_times",
                  "wrong death trophy name");
            self.trace.push_back(Step::trophy_id); r->word = 17; break;
        case ctrl::Operation::trophy_unlock:
            check(q->subject == 0x500000005ull && q->argument == 17,
                  "trophy manager/index owner changed");
            self.trace.push_back(Step::trophy_unlock); break;
        case ctrl::Operation::online:
            self.trace.push_back(Step::online); r->word = 0; break;
        default:
            error = "unexpected Kill continuation operation"; return 1;
        }
        return 0;
    }
    static int on_died(void* raw, std::uintptr_t character,
                       std::uintptr_t killer, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(character == CHARACTER && killer == KILLER && self.dead == 1 &&
              self.resolved[36] == 0 && self.resolved[25] == 10 * 256,
              "OnDied ran before Kill continuation completed");
        self.trace.push_back(Step::on_died); return 0;
    }
    static int state_event(void* raw, std::uintptr_t character,
                           std::uint32_t event, std::uintptr_t payload,
                           std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        check(character == CHARACTER && event == 2 && payload == KILLER,
              "RaiseEvent(2,killer) arguments changed");
        self.trace.push_back(Step::event2); ++self.event2_calls;
        if (self.fail_event2) { error = "injected event 2 failure"; return 1; }
        return 0;
    }
};

void exact_order_and_no_replay() {
    Fixture f;
    source::Runtime runtime({CHARACTER, &f.dead, &f.properties,
        {&f, Fixture::continuation}, {&f, Fixture::on_died, Fixture::state_event}});
    ctrl::Result result{}; std::string error;
    check(runtime.caller().kill(KILLER, 0, &result, error) == ctrl::Status::complete,
          error.c_str());
    const std::vector<Step> expected = {
        Step::is_player, Step::add_death_count, Step::application, Step::locality,
        Step::trophy_manager, Step::get_death_count, Step::trophy_id,
        Step::trophy_unlock, Step::online, Step::on_died, Step::event2};
    check(f.trace == expected, "source Kill / Ctrl_Kill order changed");
    check(f.dead == 1 && f.resolved[36] == 0 && f.resolved[25] == 10 * 256 &&
          result.kill_completed && result.event2_completed && f.get_count == 1 &&
          f.locality_result.value == 1,
          "player death owners did not complete the exact branch");

    const auto count = f.trace.size();
    check(runtime.caller().kill(KILLER, 0, &result, error) == ctrl::Status::complete &&
          result.skipped && f.trace.size() == count && f.event2_calls == 1 &&
          runtime.snapshot().outer_is_dead_reads == 2,
          "completed Ctrl_Kill episode replayed its inner Kill or event 2");
    const auto& snapshot = runtime.snapshot();
    check(snapshot.outer_is_dead_reads == 2 && snapshot.inner_is_dead_reads == 1 &&
          snapshot.kill_prefix_completed && snapshot.on_died_completed &&
          snapshot.state_event_completed,
          "provider completion snapshot omitted a reached source stage");
}

void event_failure_is_not_replayed() {
    Fixture f; f.fail_event2 = true;
    source::Runtime runtime({CHARACTER, &f.dead, &f.properties,
        {&f, Fixture::continuation}, {&f, Fixture::on_died, Fixture::state_event}});
    ctrl::Result result{}; std::string error;
    check(runtime.caller().kill(KILLER, 0, &result, error) == ctrl::Status::failed &&
          result.kill_completed && !result.event2_completed &&
          runtime.snapshot().on_died_completed && f.event2_calls == 1,
          "event2 failure lost the retained Kill/OnDied prefix");
    const auto count = f.trace.size(); f.fail_event2 = false;
    check(runtime.caller().kill(KILLER, 0, &result, error) == ctrl::Status::consumed &&
          f.trace.size() == count,
          "failed event2 replayed source death effects");
}
}

int main() {
    try {
        exact_order_and_no_replay();
        event_failure_is_not_replayed();
        std::cout << "PLAYER CHARACTER KILL PROVIDER PASS 2\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
