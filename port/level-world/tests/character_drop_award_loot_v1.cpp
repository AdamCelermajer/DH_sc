#include "../character_drop_award_loot_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::character_drop_award_loot_v1;

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    std::vector<std::string> trace;
    std::size_t fail_spawn_at = static_cast<std::size_t>(-1);
    bool reenter = false;
    Runtime* runtime = nullptr;
    Status nested = Status::complete;
};

bool scatter(void* raw, std::uintptr_t item,
             std::array<float, 3>& position,
             std::array<float, 3>& destination, std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    f.trace.push_back("scatter:" + std::to_string(item));
    position = {float(item), 1.f, 2.f};
    destination = {float(item + 10), 1.f, 2.f};
    if (f.reenter) {
        Result nested{};
        std::string error;
        f.nested = f.runtime->run(&nested, error);
    }
    return true;
}

bool spawn(void* raw, std::uintptr_t item,
           const std::array<float, 3>& position,
           const std::array<float, 3>& destination,
           std::uintptr_t& world, std::string& error) {
    auto& f = *static_cast<Fixture*>(raw);
    f.trace.push_back("spawn:" + std::to_string(item));
    if (position[0] != float(item) || destination[0] != float(item + 10))
        throw std::runtime_error("position projections changed");
    if (item == f.fail_spawn_at) { error = "spawn prefix"; return false; }
    world = item + 1000;
    return true;
}

bool pickup(void* raw, std::uintptr_t world, std::uintptr_t killer,
            std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    check(killer == 99, "wrong killer receiver");
    f.trace.push_back("pickup:" + std::to_string(world));
    return true;
}
}

int main() {
    const StagedItem items[]{{1, 1}, {2, 7}, {3, 1}};
    Fixture f;
    Services services{&f, scatter, spawn, pickup};
    Request request{items, 3, 99, 1};
    Runtime runtime(request, services);
    f.runtime = &runtime;
    f.reenter = true;
    Result result{};
    std::string error;
    check(runtime.run(&result, error) == Status::complete,
          "ordered source DropAndAwardLoot failed");
    check(error.empty() && result.items_completed == 3 &&
          result.spawn_completed == 3 && result.auto_pickup_completed == 2,
          "source DropAndAwardLoot result counters differ");
    const std::vector<std::string> expected{
        "scatter:1", "spawn:1", "pickup:1001",
        "scatter:2", "spawn:2",
        "scatter:3", "spawn:3", "pickup:1003"};
    check(f.trace == expected, "spawn and automatic Interact order differs from source");
    check(f.nested == Status::busy, "reentrant source loot callback was not rejected");
    check(runtime.run(&result, error) == Status::consumed,
          "completed source loot episode replayed");

    Fixture failure;
    failure.fail_spawn_at = 2;
    Runtime partial(request, Services{&failure, scatter, spawn, pickup});
    check(partial.run(&result, error) == Status::provider_failed &&
          error == "spawn prefix" && result.items_completed == 1 &&
          result.spawn_completed == 1 && result.current_index == 1,
          "source failure did not preserve reached per-item prefix");
    check(failure.trace == std::vector<std::string>({
              "scatter:1", "spawn:1", "pickup:1001", "scatter:2", "spawn:2"}),
          "later drop or auto pickup ran after a source failure");
    check(partial.run(&result, error) == Status::consumed,
          "failed source loot prefix was replayed");
    std::cout << "PASS: source DropAndAwardLoot per-item order, auto pickup, prefix and reentry\n";
}
