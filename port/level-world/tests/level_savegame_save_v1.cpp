#include "../level_savegame_save_v1.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace save = dh2::level_savegame_save_v1;

namespace {
void check(bool ok, const char* text) {
    if (!ok) { std::fprintf(stderr, "FAIL: %s\n", text); std::exit(1); }
}
struct Fixture {
    std::uintptr_t received{};
    std::uint32_t calls{};
    save::Services services{this, dispatch};
    static std::int32_t dispatch(void* raw, std::uintptr_t identity, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        self.received = identity;
        ++self.calls;
        return 0;
    }
};
}

int main() {
    save::State state{0xabc, 0, 0, 0, 0};
    save::Result result{};
    std::string error;
    Fixture fixture;
    check(save::run(&state, &fixture.services, &result, error) == save::Status::saved &&
          fixture.calls == 1 && fixture.received == 0xabc,
          "offline LevelSavegame did not dispatch canonical saveAll");

    state.gate_39 = 0x6c;
    check(save::run(&state, &fixture.services, &result, error) == save::Status::skipped &&
          fixture.calls == 1, "nonzero LevelSavegame gate did not skip saveAll");
    state.gate_39 = 0;
    state.online = 1;
    state.local_player_is_host = 0;
    check(save::run(&state, &fixture.services, &result, error) == save::Status::skipped &&
          fixture.calls == 1, "online non-host dispatched saveAll");
    state.local_player_is_host = 1;
    check(save::run(&state, &fixture.services, &result, error) == save::Status::saved &&
          fixture.calls == 2, "online host could not save");
    state.player_manager_gate_719 = 1;
    check(save::run(&state, &fixture.services, &result, error) == save::Status::skipped &&
          fixture.calls == 2, "PlayerManager+0x719 source gate was ignored");
    state.player_manager_gate_719 = 0;
    state.savegame = 0;
    check(save::run(&state, &fixture.services, &result, error) == save::Status::skipped &&
          fixture.calls == 2, "null Savegame identity dispatched saveAll");
    std::puts("PASS level_savegame_save_v1 checks=5");
}
