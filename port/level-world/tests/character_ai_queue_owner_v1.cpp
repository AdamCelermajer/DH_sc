#include "../character_ai_queue_owner_v1.hpp"
#include <cstdlib>
#include <iostream>

namespace {
using namespace dh2::character_ai_queue_owner_v1;
void check(bool value, const char* message) {
    if (!value) { std::cerr << message << '\n'; std::exit(1); }
}
struct Probe { std::uint32_t queries = 0; };
std::int32_t query(void* raw, dh2::character_ai_queue::State*, Entry*,
                   const Request* request, std::uint32_t* output) {
    if (!raw || !request || !output) return 1;
    ++static_cast<Probe*>(raw)->queries;
    *output = 0; // Faerie/Follower; Zonable is false so the source selects it.
    return 0;
}
OwnerProjection owner(std::uintptr_t identity) {
    return {identity, 0, 0, 1, 0, 0};
}
}

int main() {
    Runtime runtime;
    check(runtime.register_ai(11, owner(101)) == Status::complete, "register 11");
    check(runtime.register_ai(22, owner(202)) == Status::complete, "register 22");
    check(runtime.register_ai(33, owner(303)) == Status::complete, "register 33");
    check(runtime.register_ai(22, owner(202)) == Status::duplicate_ai, "duplicate registration");

    Probe probe;
    const Services services{&probe, query};
    Result result{};
    check(runtime.advance(16, false, &services, &result) == Status::complete,
          "initial due queue advance");
    check(result.decision == dh2::character_ai_queue::Decision::selected &&
          runtime.turn_globals().queue_front == 22 && runtime.timer() == 180,
          "first advance rotates once and selects exactly the new front");

    check(runtime.advance(100, false, &services, &result) == Status::complete &&
          runtime.timer() == 80 && runtime.turn_globals().queue_front == 22,
          "positive timer only consumes shared frame delta");
    check(runtime.advance(80, false, &services, &result) == Status::complete &&
          runtime.timer() == 0 && runtime.turn_globals().queue_front == 22,
          "timer reaches due without an early rotation");
    check(runtime.advance(16, false, &services, &result) == Status::complete &&
          runtime.turn_globals().queue_front == 33 && runtime.timer() == 180,
          "next due update rotates to the next eligible owner");

    check(runtime.remove_ai(33) == Status::complete && runtime.count() == 2 &&
          runtime.turn_globals().queue_front == 11 && runtime.timer() == 180,
          "destruction removes the retained entry and preserves global timer");
    check(runtime.refresh_owner(11, owner(101)) == Status::complete && probe.queries != 0,
          "live owner refresh and source predicate callbacks remain connected");
    std::cout << "character_ai_queue_owner_v1 PASS registrations=3 rotations=2 timer=180 removal=1\n";
}
