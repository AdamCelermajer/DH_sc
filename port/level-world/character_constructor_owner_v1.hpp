#pragma once

#include <cstdint>
#include <string>

namespace dh2::character_constructor_owner_v1 {

using Identity = std::uintptr_t;

enum class Component : std::uint8_t {
    game_object, controllable, inventory, timers, ai, animator,
    state_machine, properties, net_state_primary, net_state_secondary,
    controller,
};
enum class Association : std::uint8_t {
    target_list, controller, timers, ai, animator, state_machine, properties,
};
enum class Action : std::uint8_t { component, association, state };

struct SourceStep {
    Action action;
    std::uint32_t value;
    std::uint32_t callsite;
    // Constructor argument forwarded by the source call when this component
    // is constructed. Character::Character passes GO_IDS=0 into GameObject /
    // ObjectBase at the first source step.
    std::uint32_t source_argument{};
};

// Ordered calls from Character::Character at 0x3a9340. Components and links
// are borrowed semantic owner services, never embedded-object memory overlays.
inline constexpr SourceStep source_steps[] = {
    {Action::component, std::uint32_t(Component::game_object), 0x3a9354, 0},
    {Action::component, std::uint32_t(Component::controllable), 0x3a9368},
    {Action::component, std::uint32_t(Component::inventory), 0x3a9378},
    {Action::component, std::uint32_t(Component::timers), 0x3a9394},
    {Action::component, std::uint32_t(Component::ai), 0x3a939c},
    {Action::component, std::uint32_t(Component::animator), 0x3a93a4},
    {Action::component, std::uint32_t(Component::state_machine), 0x3a93b8},
    {Action::component, std::uint32_t(Component::properties), 0x3a93c0},
    {Action::component, std::uint32_t(Component::net_state_primary), 0x3a975c},
    {Action::component, std::uint32_t(Component::net_state_secondary), 0x3a9764},
    {Action::association, std::uint32_t(Association::target_list), 0x3a9774},
    {Action::component, std::uint32_t(Component::controller), 0x3a9788},
    {Action::association, std::uint32_t(Association::controller), 0x3a97c4},
    {Action::association, std::uint32_t(Association::timers), 0x3a97d8},
    {Action::association, std::uint32_t(Association::ai), 0x3a97e4},
    {Action::association, std::uint32_t(Association::animator), 0x3a97f0},
    {Action::association, std::uint32_t(Association::state_machine), 0x3a97fc},
    {Action::association, std::uint32_t(Association::properties), 0x3a9808},
};

constexpr std::uint32_t registered_state_count = 20;
constexpr std::uint32_t all_registered_states = (1u << registered_state_count) - 1u;

struct Services {
    void* context = nullptr;
    // Ensure/adopt the canonical semantic owner for this Character.
    int (*component)(void*, Component, Identity, std::string&) = nullptr;
    // Bind an existing owner to the same Character identity.
    int (*associate)(void*, Association, Identity, std::string&) = nullptr;
    // Character::RegisterState is called for each ID 0 through 19.
    int (*register_state)(void*, Identity, std::uint32_t, std::string&) = nullptr;
    // Undo a successful provider operation, in reverse source order.
    void (*rollback)(void*, Action, std::uint32_t, Identity) noexcept = nullptr;
    // Optional typed implementation of the ObjectBase construction nested in
    // the GameObject component. When bound, this receives the exact GO_IDS
    // constructor argument before the generic component adoption callback.
    int (*construct_object_base)(void*, Identity, std::uint32_t go_ids,
                                 std::string&) = nullptr;
};

struct Owner {
    Identity identity = 0;
    std::uint32_t registered_states = 0;
    bool target_list_bound = false;
    bool controller_bound = false;
    bool constructor_complete = false;
    std::uint32_t object_base_go_ids_f4{};
    bool object_base_go_ids_f4_ready{};
};

enum class Status : std::uint8_t {
    complete, invalid_argument, service_unavailable, service_failed,
};
struct Result {
    std::uint32_t completed_steps = 0;
    std::uint32_t rolled_back_steps = 0;
    std::uint32_t registered_state_count = 0;
    std::uint32_t registered_state_mask = 0;
    std::uint32_t failed_callsite = 0;
    std::uint32_t object_base_go_ids_f4{};
    bool object_base_go_ids_f4_ready{};
};

Status construct(Owner*, Identity, const Services*, Result*, std::string&);

} // namespace dh2::character_constructor_owner_v1
