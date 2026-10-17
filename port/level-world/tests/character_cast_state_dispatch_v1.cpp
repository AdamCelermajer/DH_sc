#include "../character_cast_state_dispatch_v1.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace cast = dh2::character_cast_state_dispatch_v1;

namespace {
int failures = 0;
int checks = 0;

void expect(bool condition, const char* name) {
    ++checks;
    if (!condition) {
        ++failures;
        std::fprintf(stderr, "FAIL %s\n", name);
    }
}

void expect_edge(std::int32_t current, std::uint32_t event_id,
                 std::int32_t next, const char* name) {
    dh2::character::State state{};
    state.current = current;
    state.flags = 0x12345678u;
    state.cached_speed = 1.25f;
    const auto before = state;
    cast::Result result{99, 99};
    expect(cast::event(&state, event_id, &result) == cast::Status::complete,
           name);
    expect(result.registered == 1 && result.next == next, name);
    expect(std::memcmp(&state, &before, sizeof(state)) == 0,
           "dispatcher leaves coordinator state untouched");
}

void expect_noop(std::int32_t current, std::uint32_t event_id,
                 const char* name) {
    dh2::character::State state{};
    state.current = current;
    cast::Result result{99, 99};
    expect(cast::event(&state, event_id, &result) == cast::Status::complete,
           name);
    expect(result.registered == 0 && result.next == -1, name);
}
} // namespace

int main() {
    expect_edge(3, 50006u, 7, "idle C356 enters cast");
    expect_edge(4, 50006u, 7, "move C356 enters cast");
    expect_edge(5, 50006u, 7, "attack C356 enters cast");
    expect_edge(7, 34u, 3, "cast event34 returns idle");
    expect_edge(7, 50008u, 12, "cast C358 enters state12");

    expect_noop(3, 34u, "idle unregistered event is a no-op");
    expect_noop(4, 50008u, "move unregistered event is a no-op");
    expect_noop(5, 34u, "attack unregistered event is a no-op");
    expect_noop(6, 50006u, "unmapped state C356 is a no-op");
    expect_noop(7, 50006u, "cast C356 is a no-op");
    expect_noop(7, 12345u, "cast unregistered event is a no-op");

    dh2::character::State state{};
    state.current = 7;
    cast::Result sentinel{88, 77};
    const auto sentinel_before = sentinel;
    expect(cast::event(nullptr, 34u, &sentinel) == cast::Status::invalid_argument,
           "null state rejected");
    expect(std::memcmp(&sentinel, &sentinel_before, sizeof(sentinel)) == 0,
           "invalid state preserves output");
    expect(cast::event(&state, 34u, nullptr) == cast::Status::invalid_argument,
           "null output rejected");
    expect(cast::event(&state, 34u,
                       reinterpret_cast<cast::Result*>(&state)) ==
               cast::Status::invalid_argument,
           "aliased state and output rejected");

    std::printf("{\"suite\":\"character_cast_state_dispatch_v1\","
                "\"checks\":%d,\"failures\":%d}\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
