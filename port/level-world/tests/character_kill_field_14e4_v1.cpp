#include "../character_kill_field_14e4_v1.hpp"

#include <cstdlib>
#include <iostream>

namespace field = dh2::character_kill_field_14e4_v1;

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << '\n';
        std::exit(1);
    }
}
}

int main() {
    field::State state{};
    std::uint8_t value = 0xff;
    require(field::read(&state, 0x100, &value) == field::Status::not_initialized,
            "unseen Character fabricated a +0x14e4 value");
    require(field::character_constructor_completed(&state, 0x100) ==
                field::Status::complete,
            "source Character constructor write was not retained");
    require(state.value == 0 && state.constructor_write_seen == 1 &&
                state.network_spawn_write_seen == 0,
            "Character constructor did not project its explicit zero byte");
    require(field::read(&state, 0x100, &value) == field::Status::complete && value == 0,
            "same-Character constructor byte did not read as zero");
    require(field::network_spawn_marker_written(&state, 0x100) ==
                field::Status::complete,
            "source network-spawn marker write was not retained");
    require(field::read(&state, 0x100, &value) == field::Status::complete && value == 1,
            "network-spawn marker did not override the constructor byte");
    require(field::network_spawn_marker_written(&state, 0x100) ==
                field::Status::complete && state.value == 1,
            "repeated source marker write was not idempotent");
    require(field::read(&state, 0x200, &value) == field::Status::wrong_character,
            "field read crossed canonical Character identities");
    require(field::retire(&state, 0x100) == field::Status::complete &&
                field::read(&state, 0x100, &value) == field::Status::retired,
            "retired Character field remained readable");

    field::State marker_before_ctor{};
    require(field::network_spawn_marker_written(&marker_before_ctor, 0x300) ==
                field::Status::not_initialized,
            "network marker created a projection without its constructor owner");
    std::cout << "{\"validation\":\"PASS\",\"constructor_zero\":true,"
                 "\"network_spawn_one\":true,\"identity_guard\":true,"
                 "\"retirement_guard\":true}\n";
    return 0;
}
