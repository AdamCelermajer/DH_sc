#include "../loot_pickup_quest_tail_v10.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::character;

namespace {

struct Fixture {
    std::vector<int> calls;
    bool player{true};
    bool registered{true};
    std::uintptr_t event_manager{789};
    int fail_at{};
    LootPickupQuestEventV10 copied{};

    bool step(int id, std::string& error) {
        calls.push_back(id);
        if (fail_at != id) return true;
        error = "declared callback failure " + std::to_string(id);
        return false;
    }

    static bool is_player(void* raw, std::uintptr_t character, bool& out,
                          std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        if (character != 123) throw std::runtime_error("IsPlayer character");
        out = self.player;
        return self.step(1, error);
    }

    static bool gathering(void* raw, std::uintptr_t character, std::int32_t item,
                          bool& out, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        if (character != 123 || item != 456)
            throw std::runtime_error("registered gathering IDs");
        out = self.registered;
        return self.step(2, error);
    }

    static bool current(void* raw, std::uintptr_t& out, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        out = self.event_manager;
        return self.step(3, error);
    }

    static bool constant(void* raw, const char* group, const char* key,
                         std::int32_t& out, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        if (std::string(group) != "v2QuestObjectiveType" ||
            std::string(key) != "GatherLoot")
            throw std::runtime_error("source literal order/value");
        out = 37;
        return self.step(4, error);
    }

    static bool async(void* raw, std::uintptr_t event_manager,
                      const LootPickupQuestEventV10& event,
                      std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        if (event_manager != 789) throw std::runtime_error("EventManager identity");
        self.copied = event;
        return self.step(5, error);
    }
};

void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

const LootPickupQuestServicesV10 services_for(Fixture& fixture) {
    return {&fixture, Fixture::is_player, Fixture::gathering, Fixture::current,
            Fixture::constant, Fixture::async};
}

}  // namespace

int main() {
    try {
        unsigned checks{};
        std::string error;

        Fixture fixture;
        auto services = services_for(fixture);
        check(loot_pickup_quest_tail_v10(0, 456, {}, error), "null character no-op");
        check(fixture.calls.empty(), "null character queried a service");
        ++checks;

        fixture.player = false;
        check(loot_pickup_quest_tail_v10(123, 456, services, error), "non-player no-op");
        check(fixture.calls == std::vector<int>{1}, "non-player gate order");
        ++checks;

        fixture = {};
        fixture.registered = false;
        services = services_for(fixture);
        check(loot_pickup_quest_tail_v10(123, 456, services, error), "unregistered item no-op");
        check(fixture.calls == std::vector<int>({1, 2}), "registered-list gate order");
        ++checks;

        fixture = {};
        fixture.event_manager = 0;
        services = services_for(fixture);
        check(loot_pickup_quest_tail_v10(123, 456, services, error), "missing current-level no-op");
        check(fixture.calls == std::vector<int>({1, 2, 3}), "current-level gate order");
        ++checks;

        fixture = {};
        services = services_for(fixture);
        check(loot_pickup_quest_tail_v10(123, 456, services, error), "successful quest tail");
        check(fixture.calls == std::vector<int>({1, 2, 3, 4, 5}), "success callback order");
        check(fixture.copied.objective_type == 37 && fixture.copied.character == 123 &&
              fixture.copied.item_id == 456 && fixture.copied.network_id == -1 &&
              fixture.copied.subject_id == -1 && fixture.copied.flag0 == 0 &&
              fixture.copied.flag1 == 0, "copied source event fields");
        ++checks;

        for (int failure = 1; failure <= 5; ++failure) {
            fixture = {};
            fixture.fail_at = failure;
            services = services_for(fixture);
            error.clear();
            check(!loot_pickup_quest_tail_v10(123, 456, services, error),
                  "required callback failure must stop tail");
            check(error == "declared callback failure " + std::to_string(failure),
                  "callback error must be preserved");
            check(fixture.calls.size() == static_cast<std::size_t>(failure),
                  "failure must stop at reached prefix");
            ++checks;
        }

        std::cout << "PASS loot pickup quest tail: gates, callback order, copied event, "
                     "and five required failure prefixes; checks=" << checks << '\n';
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
