#include "../character_existing_identity_v1.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

namespace ctor = dh2::character_constructor_owner_v1;
namespace existing = dh2::character_existing_identity_v1;

struct Fixture {
    std::uint32_t components{};
    std::uint32_t associations{};
    std::uint32_t states{};
    std::uint32_t rollbacks{};
    std::uint32_t fail_component{UINT32_MAX};
};

int component(void* raw, ctor::Component id, ctor::Identity, std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    if (std::uint32_t(id) == f.fail_component) return 1;
    ++f.components;
    return 0;
}
int associate(void* raw, ctor::Association, ctor::Identity, std::string&) {
    ++static_cast<Fixture*>(raw)->associations;
    return 0;
}
int register_state(void* raw, ctor::Identity, std::uint32_t, std::string&) {
    ++static_cast<Fixture*>(raw)->states;
    return 0;
}
void rollback(void* raw, ctor::Action, std::uint32_t, ctor::Identity) noexcept {
    ++static_cast<Fixture*>(raw)->rollbacks;
}

int main() {
    std::string error;
    Fixture f{};
    const ctor::Services services{&f, component, associate, register_state,
                                  rollback};
    existing::Owner owner{};
    existing::Result result{};
    constexpr auto identity = std::uintptr_t{0x100000123};
    assert(existing::construct(&owner, identity, &services, &result, error) ==
           existing::Status::complete);
    assert(owner.identity == identity && owner.constructor.identity == identity);
    assert(owner.constructor.constructor_complete);
    assert(result.constructor_save_vectors_ready &&
           owner.constructor_save_vectors_ready &&
           result.source_component_adoption_complete &&
           owner.source_component_adoption_complete);
    assert((owner.quick_save_position_1468 == std::array<std::uint32_t, 3>{}));
    assert((owner.revive_position_1474 == std::array<std::uint32_t, 3>{}));
    assert(f.components == 11 && f.associations == 7 && f.states == 20);
    const auto successful_steps = result.constructor.completed_steps;
    assert(existing::construct(&owner, identity, &services, &result, error) ==
           existing::Status::invalid_argument);

    Fixture failed{};
    failed.fail_component = std::uint32_t(ctor::Component::controller);
    const ctor::Services failing_services{&failed, component, associate,
        register_state, rollback};
    existing::Owner rejected{};
    assert(existing::construct(&rejected, identity + 1, &failing_services,
        &result, error) == existing::Status::constructor_failed);
    assert(!rejected.identity && !rejected.constructor.constructor_complete &&
           !rejected.constructor_save_vectors_ready &&
           !rejected.source_component_adoption_complete &&
           !result.constructor_save_vectors_ready &&
           !result.source_component_adoption_complete);
    assert(failed.rollbacks == failed.components + failed.associations);

    std::cout << "character_existing_identity_v1 PASS; complete steps="
              << successful_steps << '\n';
}
