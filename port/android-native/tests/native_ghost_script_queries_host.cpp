#include "../app/src/main/cpp/native_ghost_script_queries.hpp"

#include <cstdio>
#include <cstdlib>

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}

}  // namespace

int main() {
    using namespace dh2::native::ghost_script_queries;
    constexpr std::uintptr_t owner = 0x100000002ull;
    std::int32_t character_state = 1;  // the existing Limbus Coordinator state
    std::uint32_t path_count = 0;
    const Binding binding{owner, &character_state, &path_count};

    std::int32_t state_output = -9;
    std::uint32_t path_output = 9;
    require(get_state(&binding, owner, &state_output) == Status::complete &&
                state_output == 1,
            "GetState must read the bound Character Coordinator");
    require(has_path(&binding, owner, &path_output) == Status::complete &&
                path_output == 0,
            "HasPath must report the bound empty native path");

    character_state = 3;
    path_count = 2;
    require(get_state(&binding, owner, &state_output) == Status::complete &&
                state_output == 3,
            "GetState must observe the live Coordinator state");
    require(has_path(&binding, owner, &path_output) == Status::complete &&
                path_output == 1,
            "HasPath must observe the live path count");
    require(character_state == 3 && path_count == 2,
            "the query adapter must not mutate source state or path");

    state_output = -9;
    path_output = 9;
    require(get_state(&binding, owner + 1, &state_output) == Status::stale_owner &&
                state_output == -9,
            "GetState must reject stale actor identity without writing");
    require(has_path(&binding, owner + 1, &path_output) == Status::stale_owner &&
                path_output == 9,
            "HasPath must reject stale actor identity without writing");
    require(get_state(nullptr, owner, &state_output) == Status::invalid_argument &&
                has_path(&binding, owner, nullptr) == Status::invalid_argument,
            "invalid callback inputs must fail without touching source owners");
    const Binding retired{0, &character_state, &path_count};
    const Binding missing_fields{owner, nullptr, nullptr};
    require(get_state(&retired, 0, &state_output) == Status::invalid_argument &&
                has_path(&retired, 0, &path_output) == Status::invalid_argument &&
                get_state(&missing_fields, owner, &state_output) == Status::invalid_argument &&
                has_path(&missing_fields, owner, &path_output) == Status::invalid_argument &&
                state_output == -9 && path_output == 9,
            "retired or absent native bindings must leave query outputs unchanged");
    path_count = 0;
    require(has_path(&binding, owner, &path_output) == Status::complete &&
                path_output == 0,
            "HasPath must observe a native route becoming empty");

    std::puts("Native Ghost source state/path query adapter checks passed");
    return 0;
}
