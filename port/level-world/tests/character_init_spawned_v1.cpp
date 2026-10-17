#include "../character_init_spawned_v1.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

namespace init = dh2::character_init_spawned_v1;

struct Fixture {
    std::vector<init::Request> calls;
    int reject_at = -1;
};

std::int32_t record(void* context, const init::Request* request) {
    auto& fixture = *static_cast<Fixture*>(context);
    const auto index = static_cast<int>(fixture.calls.size());
    fixture.calls.push_back(*request);
    return index == fixture.reject_at ? 1 : 0;
}

bool require(bool condition) { return condition; }

int main() {
    const float position[3]{12.5f, -3.0f, 8.25f};
    Fixture fixture{};
    const init::Services services{&fixture, &record};
    bool ok = true;
    ok &= require(init::activate(70000, position, &services) == init::Status::complete);
    const std::array<init::Operation, 9> expected{
        init::Operation::write_character_id,
        init::Operation::write_init_spawned_suppression,
        init::Operation::write_in_zone,
        init::Operation::set_initial_position,
        init::Operation::set_game_object_position,
        init::Operation::invoke_virtual,
        init::Operation::invoke_virtual,
        init::Operation::invoke_virtual,
        init::Operation::request_spawn_state,
    };
    ok &= require(fixture.calls.size() == expected.size());
    if (fixture.calls.size() == expected.size()) {
        for (std::size_t i = 0; i < expected.size(); ++i)
            ok &= require(fixture.calls[i].operation == expected[i]);
        ok &= require(fixture.calls[0].integer == 4464);
        ok &= require(fixture.calls[1].integer == 1);
        ok &= require(fixture.calls[2].integer == 1);
        ok &= require(fixture.calls[3].position[0] == position[0] &&
                      fixture.calls[3].position[1] == position[1] &&
                      fixture.calls[3].position[2] == position[2]);
        ok &= require(fixture.calls[5].virtual_slot == 0x1c);
        ok &= require(fixture.calls[6].virtual_slot == 0x58);
        ok &= require(fixture.calls[7].virtual_slot == 0x40 &&
                      fixture.calls[7].integer == 1);
        ok &= require(fixture.calls[8].integer == 0 &&
                      fixture.calls[8].secondary == 0);
    }

    Fixture rejected{};
    rejected.reject_at = 4;
    const init::Services rejected_services{&rejected, &record};
    ok &= require(init::activate(17, position, &rejected_services) ==
                  init::Status::service_rejected);
    ok &= require(rejected.calls.size() == 5);
    ok &= require(init::activate(17, nullptr, &services) ==
                  init::Status::invalid_argument);

    if (!ok) return 1;
    std::cout << "{\"character_init_spawned_order\":true,"
                 "\"partial_failure_short_circuits\":true,"
                 "\"mismatches\":0}\n";
    return 0;
}
