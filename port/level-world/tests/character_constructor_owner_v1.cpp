#include "../character_constructor_owner_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace ctor = dh2::character_constructor_owner_v1;
namespace {
void require(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}
struct Fixture {
    std::vector<std::uint32_t> operations;
    std::vector<std::uint32_t> rollbacks;
    std::uint32_t fail_state = UINT32_MAX;
    std::uint32_t go_ids = UINT32_MAX;
    std::uint32_t object_base_calls = 0;
};
std::uint32_t encode(ctor::Action action, std::uint32_t value) {
    return (std::uint32_t(action) << 24) | value;
}
int component(void* raw, ctor::Component value, ctor::Identity,
              std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    f.operations.push_back(encode(ctor::Action::component,
                                  std::uint32_t(value)));
    return 0;
}
int associate(void* raw, ctor::Association value, ctor::Identity,
              std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    f.operations.push_back(encode(ctor::Action::association,
                                  std::uint32_t(value)));
    return 0;
}
int register_state(void* raw, ctor::Identity, std::uint32_t state,
                   std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    f.operations.push_back(encode(ctor::Action::state, state));
    return state == f.fail_state ? 1 : 0;
}
int construct_object_base(void* raw, ctor::Identity identity,
                          std::uint32_t go_ids, std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    if (!identity) return 1;
    f.go_ids = go_ids;
    ++f.object_base_calls;
    return 0;
}
void rollback(void* raw, ctor::Action action, std::uint32_t value,
              ctor::Identity) noexcept {
    static_cast<Fixture*>(raw)->rollbacks.push_back(encode(action, value));
}
}

int main() {
    try {
        constexpr std::uint32_t expected[] = {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0x01000000u, 10,
            0x01000001u, 0x01000002u, 0x01000003u, 0x01000004u,
            0x01000005u, 0x01000006u,
        };
        require(sizeof(ctor::source_steps) / sizeof(ctor::source_steps[0]) ==
                    sizeof(expected) / sizeof(expected[0]),
                "constructor step count changed");
        for (std::size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); ++i)
            require(encode(ctor::source_steps[i].action,
                           ctor::source_steps[i].value) == expected[i],
                    "embedded constructor or association order changed");

        Fixture f;
        ctor::Services services{&f, component, associate, register_state, rollback,
                                construct_object_base};
        ctor::Owner owner{};
        ctor::Result result{};
        std::string error;
        require(ctor::construct(&owner, 0x1234, &services, &result, error) ==
                    ctor::Status::complete,
                "source constructor owner failed");
        require(owner.identity == 0x1234 && owner.constructor_complete &&
                    owner.target_list_bound && owner.controller_bound &&
                    owner.registered_states == ctor::all_registered_states &&
                    result.registered_state_count == 20 &&
                    result.registered_state_mask == ctor::all_registered_states &&
                    owner.object_base_go_ids_f4_ready &&
                    owner.object_base_go_ids_f4 == 0 &&
                    result.object_base_go_ids_f4_ready &&
                    result.object_base_go_ids_f4 == 0 &&
                    f.object_base_calls == 1 && f.go_ids == 0,
                "constructor defaults or all 20 source states are incomplete");
        require(f.operations.size() == 38 && f.operations[18] == 0x02000000u &&
                    f.operations[37] == 0x02000013u,
                "RegisterState IDs were not called in source order 0..19");

        Fixture failed;
        failed.fail_state = 7;
        services.context = &failed;
        owner = {};
        require(ctor::construct(&owner, 0x5678, &services, &result, error) ==
                    ctor::Status::service_failed &&
                    result.failed_callsite == 0x3a9820 &&
                    result.rolled_back_steps == 25 && owner.identity == 0 &&
                    !owner.constructor_complete && failed.rollbacks.size() == 25 &&
                    failed.rollbacks.front() == 0x02000006u &&
                    failed.rollbacks.back() == 0,
                "failed state registration did not roll back the exact prefix");

        std::cout << "{\"constructor_order\":true,\"registered_states\":20,"
                     "\"failure_rollback\":true}\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
