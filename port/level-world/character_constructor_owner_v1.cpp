#include "character_constructor_owner_v1.hpp"

#include <array>
#include <cstddef>

namespace dh2::character_constructor_owner_v1 {
namespace {
struct Completed {
    Action action;
    std::uint32_t value;
};
constexpr std::size_t operation_count = sizeof(source_steps) / sizeof(source_steps[0]) +
                                        registered_state_count;
}

Status construct(Owner* owner, Identity identity, const Services* services,
                 Result* result, std::string& error) {
    error.clear();
    if (!owner || !identity || !services || !result || owner->constructor_complete) {
        error = "Character constructor owner arguments or state are invalid";
        return Status::invalid_argument;
    }
    *result = {};
    if (!services->component || !services->associate ||
        !services->register_state || !services->rollback) {
        error = "Character constructor owner provider set is incomplete";
        return Status::service_unavailable;
    }
    if (owner->identity && owner->identity != identity) {
        error = "Character constructor owner cannot change identity";
        return Status::invalid_argument;
    }
    *owner = {};
    owner->identity = identity;

    std::array<Completed, operation_count> completed{};
    std::size_t count = 0;
    auto fail = [&](std::uint32_t callsite) {
        result->failed_callsite = callsite;
        for (std::size_t i = count; i > 0; --i) {
            services->rollback(services->context, completed[i - 1].action,
                               completed[i - 1].value, identity);
            ++result->rolled_back_steps;
        }
        *owner = {};
        result->object_base_go_ids_f4 = 0;
        result->object_base_go_ids_f4_ready = false;
        if (error.empty()) error = "Character constructor owner provider failed";
        return Status::service_failed;
    };

    for (const auto& step : source_steps) {
        int status = 1;
        bool object_base_constructed = false;
        try {
            if (step.action == Action::component &&
                step.value == std::uint32_t(Component::game_object) &&
                services->construct_object_base) {
                status = services->construct_object_base(services->context,
                    identity, step.source_argument, error);
                if (status == 0) {
                    object_base_constructed = true;
                    owner->object_base_go_ids_f4 = step.source_argument;
                    owner->object_base_go_ids_f4_ready = true;
                }
            }
            if (status == 0 || step.action != Action::component ||
                step.value != std::uint32_t(Component::game_object) ||
                !services->construct_object_base) {
                if (step.action == Action::component)
                status = services->component(services->context,
                    static_cast<Component>(step.value), identity, error);
                else
                    status = services->associate(services->context,
                        static_cast<Association>(step.value), identity, error);
            }
        } catch (...) {
            status = 1;
        }
        if (status != 0) {
            if (object_base_constructed)
                services->rollback(services->context, step.action, step.value,
                                   identity);
            return fail(step.callsite);
        }
        completed[count++] = {step.action, step.value};
        ++result->completed_steps;
        if (step.action == Action::association &&
            step.value == std::uint32_t(Association::target_list))
            owner->target_list_bound = true;
        if (step.action == Action::association &&
            step.value == std::uint32_t(Association::controller))
            owner->controller_bound = true;
    }

    for (std::uint32_t state = 0; state < registered_state_count; ++state) {
        int status = 1;
        try {
            status = services->register_state(services->context, identity,
                                               state, error);
        } catch (...) {
            status = 1;
        }
        if (status != 0) return fail(0x3a9820);
        completed[count++] = {Action::state, state};
        owner->registered_states |= 1u << state;
        ++result->registered_state_count;
        ++result->completed_steps;
    }

    owner->constructor_complete = owner->target_list_bound && owner->controller_bound &&
        owner->registered_states == all_registered_states;
    result->registered_state_mask = owner->registered_states;
    result->object_base_go_ids_f4 = owner->object_base_go_ids_f4;
    result->object_base_go_ids_f4_ready = owner->object_base_go_ids_f4_ready;
    if (!owner->constructor_complete) {
        error = "Character constructor owner did not reach its source defaults";
        return fail(0x3a9868);
    }
    return Status::complete;
}

} // namespace dh2::character_constructor_owner_v1
