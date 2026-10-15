#include <string>
#include "character_sm_set_dead_state_v1.hpp"

#include <cstring>
#include <limits>

namespace dh2::character_sm_set_dead_state_v1 { namespace {

struct Range { std::uintptr_t begin = 0, end = 0; };

template<class T>
bool range(const T* pointer, Range& result) {
    const auto begin = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || begin % alignof(T) || begin > UINTPTR_MAX - sizeof(T))
        return false;
    result = {begin, begin + sizeof(T)};
    return true;
}

bool overlaps(Range left, Range right) {
    return left.begin < right.end && right.begin < left.end;
}

bool aliases_outputs(const Bindings& bindings, const Request& request,
                     const Result* result, const std::string& error) {
    Range output_range{}, error_range{};
    if (!range(result, output_range) || !range(&error, error_range) ||
        overlaps(output_range, error_range))
        return true;
    const auto disjoint = [&](const auto* pointer) {
        Range candidate{};
        return range(pointer, candidate) && !overlaps(output_range, candidate) &&
               !overlaps(error_range, candidate);
    };
    if (!disjoint(&bindings) || !disjoint(&request) ||
        !disjoint(bindings.coordinator) ||
        !disjoint(&bindings.coordinator->state) ||
        !disjoint(bindings.character.properties) || !disjoint(bindings.tables) ||
        !disjoint(bindings.constants) || !disjoint(&bindings.anim_stance))
        return true;
    const auto& timers = bindings.coordinator->timers();
    for (std::uint32_t i = 0; i < timers.count; ++i)
        if (!disjoint(timers.slots + i)) return true;
    return false;
}

bool coherent(const Bindings& bindings, std::uintptr_t identity) {
    return bindings.coordinator && bindings.coordinator->bound() &&
           bindings.coordinator->owner() == identity &&
           bindings.character.identity == identity;
}

Status invalid(Status status, const char* message, std::string& error) {
    if (error.empty()) error = message;
    return status;
}

} // namespace

Status execute(const Bindings& bindings, const Request& request, Result* output,
               std::string& error) {
    if (!output || !bindings.coordinator || !bindings.character.identity ||
        (request.dead != 0 && request.dead != 1) ||
        (request.force_transition != 0 && request.force_transition != 1) ||
        !coherent(bindings, bindings.character.identity) ||
        aliases_outputs(bindings, request, output, error))
        return invalid(Status::invalid_argument,
                       "Invalid same-Coordinator SM_SetDeadState request", error);

    *output = {};
    error.clear();

    character_anim_table_resolver_v1::Result table{};
    const auto resolve_status = character_anim_table_resolver_v1::resolve(
        &bindings.character, bindings.tables, &table);
    if (resolve_status == character_anim_table_resolver_v1::Status::invalid_argument)
        return invalid(Status::invalid_argument,
                       "Invalid Character animation-table view", error);
    if (resolve_status != character_anim_table_resolver_v1::Status::complete)
        return invalid(Status::invalid_source_fact,
                       "Character animation table is unavailable", error);

    output->table_id = table.table_id;
    output->table_count = table.table_count;
    // GetCharAnimTableId falls back to row 17; SM_SetDeadState then performs
    // its own bounds check and returns before mutating any machine field.
    if (!table.row) return Status::complete;
    output->table_valid = 1;

    auto& coordinator = *bindings.coordinator;
    auto& machine = coordinator.state;
    if (machine.dead_alternate > UINT8_MAX)
        return invalid(Status::invalid_source_fact,
                       "CharStateMachine+0x3f projection is not a byte", error);

    const bool great_for_death = machine.dead_alternate != 0;
    character_dead_animation_selection_v1::OneAnimationRequest selected_request{};
    selected_request.character = bindings.character.identity;
    selected_request.row = table.row;
    selected_request.constants = bindings.constants;
    selected_request.field = great_for_death ? 3u : 6u;
    selected_request.stance_mask = great_for_death ? 0x20000u : 0x8000u;
    selected_request.anim_stance_context = bindings.anim_stance.context;
    selected_request.get_anim_stance = bindings.anim_stance.get_anim_stance;

    character_dead_animation_selection_v1::OneAnimationResult selected{};
    auto selection_status = character_dead_animation_selection_v1::select_one(
        &selected_request, &selected);
    if (selection_status != character_dead_animation_selection_v1::Status::complete)
        return invalid(Status::invalid_source_fact,
                       ("Death animation selection failed at check " +
                        std::to_string(selected.failure_point)).c_str(), error);
    output->death_animation = selected.animation;
    output->death_mask_queries = selected.mask_queries;
    output->death_stance_queries = selected.stance_queries;
    if (!coherent(bindings, bindings.character.identity))
        return invalid(Status::invalid_source_fact,
                       "Character owner changed during death selection", error);

    // Source reloads machine+0x3f after the death GetAnimStance branch, before
    // it stores +0x28. Capture that fresh value before the same-owner write.
    if (machine.dead_alternate > UINT8_MAX)
        return invalid(Status::invalid_source_fact,
                       "CharStateMachine+0x3f changed outside byte range", error);
    const bool great_for_despawn = machine.dead_alternate != 0;
    machine.animation_override = selected.animation;

    selected_request.field = great_for_despawn ? 5u : 4u;
    selected_request.stance_mask = great_for_despawn ? 0x40000u : 0x10000u;
    selected = {};
    selection_status = character_dead_animation_selection_v1::select_one(
        &selected_request, &selected);
    if (selection_status != character_dead_animation_selection_v1::Status::complete)
        return invalid(Status::invalid_source_fact,
                       "Despawn animation selection failed", error);
    output->despawn_animation = selected.animation;
    output->despawn_mask_queries = selected.mask_queries;
    output->despawn_stance_queries = selected.stance_queries;
    if (!coherent(bindings, bindings.character.identity))
        return invalid(Status::invalid_source_fact,
                       "Character owner changed during despawn selection", error);

    // Source writes machine+0x38, then byte+0x3e, then clears byte+0x3f.
    coordinator.set_source_despawn_animation_38(selected.animation);
    coordinator.set_source_dead_byte_3e(static_cast<std::uint8_t>(request.dead));
    machine.dead_alternate = 0;

    // SM_IsAwaitingToRevive is true only for state IDs 0, 16, or 17. A null
    // current-state pointer then reads as -1 in the existing Coordinator.
    if (machine.current == 0 || machine.current == 16 || machine.current == 17)
        machine.current = -1;

    if (request.force_transition) {
        const auto status = coordinator.transition(12, 50008, request.killer);
        if (status != 1)
            return invalid(Status::service_failed,
                           "Forced CharStateMachine state 12 transition failed", error);
        output->state_transitioned = 1;
    } else {
        // RaiseStateEvent returns its non-null machine receiver even when the
        // current state's event table ignores 50008; zero is a valid no-op.
        const auto status = coordinator.raise_dead_state_event_50008(request.killer);
        if (status < 0)
            return invalid(Status::service_failed,
                           "CharStateMachine event 50008 failed", error);
        output->state_transitioned = status == 1;
    }
    return Status::complete;
}

} // namespace dh2::character_sm_set_dead_state_v1
