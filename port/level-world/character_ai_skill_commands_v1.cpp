#include "character_ai_skill_commands_v1.hpp"

#include <array>
#include <cstring>
#include <limits>

namespace dh2::character_ai_skill_commands_v1 {
namespace {
struct Range { std::uintptr_t first, last; };

bool object_range(const void* pointer, std::size_t size, std::size_t alignment,
                  Range& result) {
    const auto address = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || address % alignment ||
        address > std::numeric_limits<std::uintptr_t>::max() - size)
        return false;
    result = {address, address + size};
    return true;
}
bool overlaps(Range a, Range b) { return a.first < b.last && b.first < a.last; }

bool add_disjoint(Range* ranges, unsigned& count, const void* pointer,
                  std::size_t size, std::size_t alignment) {
    Range next{};
    if (!object_range(pointer, size, alignment, next)) return false;
    for (unsigned i = 0; i < count; ++i)
        if (overlaps(next, ranges[i])) return false;
    ranges[count++] = next;
    return true;
}

std::int32_t signed_bits(std::uint32_t value) {
    std::int32_t result{};
    std::memcpy(&result, &value, sizeof(result));
    return result;
}
std::uint32_t unsigned_bits(std::int32_t value) {
    std::uint32_t result{};
    std::memcpy(&result, &value, sizeof(result));
    return result;
}

bool vector_valid(const SkillVector* vector, bool needed) {
    if (!vector) return !needed;
    Range ignored{};
    if (!object_range(vector, sizeof(*vector), alignof(SkillVector), ignored)) return false;
    const auto first = reinterpret_cast<std::uintptr_t>(vector->begin);
    const auto last = reinterpret_cast<std::uintptr_t>(vector->end);
    if (!first && !last) return true;
    if (!vector->begin || !vector->end || first % alignof(std::uintptr_t) ||
        last % alignof(std::uintptr_t) || last < first ||
        (last - first) % sizeof(std::uintptr_t) ||
        (last - first) / sizeof(std::uintptr_t) > 1048576)
        return false;
    if (last > std::numeric_limits<std::uintptr_t>::max() - sizeof(std::uintptr_t))
        return false;
    return true;
}

bool fields_valid(const Fields* fields) {
    Range items[4]{};
    unsigned count = 0;
    if (!add_disjoint(items, count, fields, sizeof(*fields), alignof(Fields))) return false;
    return add_disjoint(items, count, fields->current_slot_cc, sizeof(std::uint32_t), alignof(std::uint32_t)) &&
           add_disjoint(items, count, fields->continued_d0, sizeof(std::uint8_t), alignof(std::uint8_t)) &&
           add_disjoint(items, count, fields->last_d1, sizeof(std::uint8_t), alignof(std::uint8_t));
}

bool machine_valid(const Machine* machine) {
    Range machine_range{}, query_range{};
    if (!object_range(machine, sizeof(*machine), alignof(Machine), machine_range) ||
        !machine->identity || !machine->owner_04 ||
        !object_range(machine->state_query, sizeof(*machine->state_query),
                      alignof(character_skill_state_queries::Machine), query_range) ||
        !machine->state_query->current_state_id)
        return false;
    Range items[6]{};
    unsigned count = 0;
    if (!add_disjoint(items, count, machine, sizeof(*machine), alignof(Machine))) return false;
    return add_disjoint(items, count, machine->state_query, sizeof(*machine->state_query),
                        alignof(character_skill_state_queries::Machine)) &&
           add_disjoint(items, count, machine->state_query->current_state_id, sizeof(std::int32_t),
                        alignof(std::int32_t)) &&
           add_disjoint(items, count, machine->animation_28, sizeof(std::int32_t), alignof(std::int32_t)) &&
           add_disjoint(items, count, machine->skill_index_54, sizeof(std::uint32_t), alignof(std::uint32_t)) &&
           add_disjoint(items, count, machine->moving_58, sizeof(std::uint8_t), alignof(std::uint8_t));
}

bool character_valid(const Character* character) {
    Range character_range{}, machine_range{};
    return object_range(character, sizeof(*character), alignof(Character), character_range) &&
           character->identity && machine_valid(character->skill_machine_4fc) &&
           object_range(character->skill_machine_4fc, sizeof(Machine), alignof(Machine), machine_range) &&
           !overlaps(character_range, machine_range);
}

Character* current_character(State* state) {
    if (!state || !state->owner_04) return nullptr;
    Range ignored{};
    if (!object_range(state->owner_04, sizeof(*state->owner_04), alignof(OwnerSlot), ignored))
        return nullptr;
    auto* character = state->owner_04->current;
    return character_valid(character) ? character : nullptr;
}

bool state_valid(State* state, bool needs_vector) {
    Range ignored{};
    if (!state || !state->ai || state->reserved ||
        !object_range(state, sizeof(*state), alignof(State), ignored) ||
        !state->owner_04 || !object_range(state->owner_04, sizeof(*state->owner_04), alignof(OwnerSlot), ignored) ||
        !fields_valid(state->fields) || !vector_valid(state->skill_vector_b4, needs_vector))
        return false;
    return current_character(state) != nullptr;
}

bool source_backing_disjoint(const State* state) {
    Range ranges[20]{};
    unsigned count = 0;
    auto* character = state->owner_04->current;
    auto* machine = character->skill_machine_4fc;
    if (!add_disjoint(ranges, count, state->owner_04, sizeof(*state->owner_04), alignof(OwnerSlot)) ||
        !add_disjoint(ranges, count, state->fields, sizeof(*state->fields), alignof(Fields)) ||
        !add_disjoint(ranges, count, state->fields->current_slot_cc, sizeof(std::uint32_t), alignof(std::uint32_t)) ||
        !add_disjoint(ranges, count, state->fields->continued_d0, sizeof(std::uint8_t), alignof(std::uint8_t)) ||
        !add_disjoint(ranges, count, state->fields->last_d1, sizeof(std::uint8_t), alignof(std::uint8_t)) ||
        !add_disjoint(ranges, count, character, sizeof(*character), alignof(Character)) ||
        !add_disjoint(ranges, count, machine, sizeof(*machine), alignof(Machine)) ||
        !add_disjoint(ranges, count, machine->state_query, sizeof(*machine->state_query),
                      alignof(character_skill_state_queries::Machine)) ||
        !add_disjoint(ranges, count, machine->state_query->current_state_id, sizeof(std::int32_t), alignof(std::int32_t)) ||
        !add_disjoint(ranges, count, machine->animation_28, sizeof(std::int32_t), alignof(std::int32_t)) ||
        !add_disjoint(ranges, count, machine->skill_index_54, sizeof(std::uint32_t), alignof(std::uint32_t)) ||
        !add_disjoint(ranges, count, machine->moving_58, sizeof(std::uint8_t), alignof(std::uint8_t)))
        return false;
    if (state->skill_vector_b4) {
        if (!add_disjoint(ranges, count, state->skill_vector_b4, sizeof(*state->skill_vector_b4),
                          alignof(SkillVector))) return false;
        const auto first = reinterpret_cast<std::uintptr_t>(state->skill_vector_b4->begin);
        const auto last = reinterpret_cast<std::uintptr_t>(state->skill_vector_b4->end);
        if (last != first &&
            !add_disjoint(ranges, count, state->skill_vector_b4->begin, last - first,
                          alignof(std::uintptr_t))) return false;
    }
    return true;
}

struct Run {
    State* state;
    Services services;
    Result& result;

    Status call(Operation operation, std::uintptr_t subject, std::uint32_t index,
                std::uint32_t value, std::int32_t signed_value,
                std::uintptr_t related, std::uintptr_t row,
                const char* text, Response& response,
                Phase phase = Phase::not_started) {
        if (!services.invoke) return Status::service_unavailable;
        result.phase = phase == Phase::not_started ? phase_for(operation) : phase;
        result.last_operation = operation;
        ++result.service_calls;
        Request request{operation, subject, related, row, index, value,
                        signed_value, 0, text};
        response = {};
        try {
            if (services.invoke(services.context, state, &request, &response))
                return Status::service_failed;
        } catch (...) {
            return Status::service_failed;
        }
        return Status::complete;
    }

    static Phase phase_for(Operation operation) {
        switch (operation) {
        case Operation::get_skill_row: return Phase::not_started;
        case Operation::check_active: return Phase::active;
        case Operation::check_usable: return Phase::usable;
        case Operation::pre: return Phase::pre;
        case Operation::get_constant: return Phase::set_state;
        case Operation::get_anim_stance: return Phase::set_state;
        case Operation::raise_state_event: return Phase::set_state;
        case Operation::set_state: return Phase::set_state;
        case Operation::is_player: return Phase::player;
        case Operation::property_add_int: return Phase::property_add;
        case Operation::property_get_int: return Phase::property_get;
        case Operation::trophy_manager: return Phase::trophy_manager;
        case Operation::is_local_player: return Phase::local_player;
        case Operation::trophy_names: return Phase::trophy_names;
        case Operation::unlock_trophy: return Phase::unlock_trophy;
        case Operation::stop_skill_loop: return Phase::end_state;
        case Operation::assertion_log: return Phase::active;
        }
        return Phase::not_started;
    }

    Status row(std::uintptr_t owner, std::uint32_t index, SkillRow& row,
               Phase phase) {
        Response response{};
        const auto status = call(Operation::get_skill_row, owner, index, 0, 0,
                                 0, 0, nullptr, response, phase);
        if (status != Status::complete) return status;
        if (!response.row.identity ||
            !response.row.animation_04 ||
            !response.row.moving_08 ||
            !response.row.type_48) return Status::invalid_source_fact;
        Range ignored{};
        if (!object_range(response.row.animation_04, sizeof(std::int32_t), alignof(std::int32_t), ignored) ||
            !object_range(response.row.moving_08, sizeof(bool), alignof(bool), ignored) ||
            !object_range(response.row.type_48, sizeof(std::int32_t), alignof(std::int32_t), ignored))
            return Status::invalid_source_fact;
        row = response.row;
        if (phase == Phase::begin_row) result.begin_row = row.identity;
        else result.setter_row = row.identity;
        return Status::complete;
    }

    Status query_using(Character* character, std::uint32_t& value) {
        if (!character_valid(character)) return Status::invalid_source_fact;
        character_skill_state_queries::Result query{};
        const auto status = character_skill_state_queries::is_using_skill(
            character->skill_machine_4fc->state_query, &query);
        if (status != character_skill_state_queries::Status::complete)
            return Status::invalid_source_fact;
        value = query.value;
        return Status::complete;
    }

    Status active(std::uint32_t index, std::uint32_t& value) {
        value = 0;
        if (!vector_valid(state->skill_vector_b4, true)) return Status::invalid_source_fact;
        const auto first = reinterpret_cast<std::uintptr_t>(state->skill_vector_b4->begin);
        const auto last = reinterpret_cast<std::uintptr_t>(state->skill_vector_b4->end);
        const auto count = (last - first) / sizeof(std::uintptr_t);
        if (index >= count) {
            if (state->assertion_level == 2) return Status::unsupported_assertion;
            if (state->assertion_level == 1) {
                Response ignored{};
                auto* owner = current_character(state);
                if (!owner) return Status::invalid_source_fact;
                const auto status = call(Operation::assertion_log, state->ai, index,
                    0, 0, owner->identity, 0, "skillId < m_skillScripts.size()", ignored);
                if (status != Status::complete) return status;
            }
        }
        auto* owner = current_character(state);
        if (!owner) return Status::invalid_source_fact;
        auto status = query_using(owner, value);
        if (status != Status::complete) return status;
        if (value && *state->fields->current_slot_cc == index) {
            value = 1;
            return Status::complete;
        }
        // Source reloads the vector begin after SM_IsUsingSkill. If the index
        // is outside the vector this is an original unchecked read, so the
        // port reports the unsafe source edge instead of dereferencing it.
        if (!vector_valid(state->skill_vector_b4, true)) return Status::invalid_source_fact;
        const auto* fresh = state->skill_vector_b4->begin;
        const auto fresh_first = reinterpret_cast<std::uintptr_t>(fresh);
        const auto fresh_last = reinterpret_cast<std::uintptr_t>(state->skill_vector_b4->end);
        if (index >= (fresh_last - fresh_first) / sizeof(std::uintptr_t))
            return Status::invalid_source_fact;
        const auto instance = fresh[index];
        if (!instance) return Status::complete;
        Response response{};
        status = call(Operation::check_active, instance, index, 0, 0,
                      owner->identity, 0, nullptr, response);
        if (status == Status::complete) value = response.word;
        return status;
    }

    Status set_state(Machine* machine, std::uint32_t index, std::uint8_t moving,
                     std::uintptr_t payload, bool force) {
        if (!machine_valid(machine)) return Status::invalid_source_fact;
        const auto owner = machine->owner_04; // source captures Character+4 once
        SkillRow row_view{};
        auto status = row(owner, index, row_view, Phase::set_state);
        if (status != Status::complete) return status;
        // The original reads row+4 before the imported constant callback.
        std::uint32_t animation = unsigned_bits(*row_view.animation_04);
        Response response{};
        status = call(Operation::get_constant, owner, index, 0, 0, 0,
                      row_view.identity, "AnimStancedAnim/SL__LIST_IPHONE", response);
        if (status != Status::complete) return status;
        if (response.word & 0x200000u) {
            status = call(Operation::get_anim_stance, machine->owner_04, index, 0, 0,
                          0, row_view.identity, nullptr, response);
            if (status != Status::complete) return status;
            animation += unsigned_bits(response.signed_word);
        }
        // Source stores all three machine fields before either transition path.
        *machine->animation_28 = signed_bits(animation); ++result.field_writes;
        *machine->skill_index_54 = index; ++result.field_writes;
        *machine->moving_58 = moving; ++result.field_writes;
        if (force) {
            status = call(Operation::set_state, machine->identity, 6, 0xc355,
                          0, payload, row_view.identity, nullptr, response);
        } else {
            status = call(Operation::raise_state_event, machine->identity, 0xc355,
                          0, 0, payload, row_view.identity, nullptr, response);
        }
        return status;
    }

    Status begin(std::uint32_t index) {
        auto* initial_owner = current_character(state);
        if (!initial_owner) return Status::invalid_source_fact;
        SkillRow first_row{};
        auto status = row(initial_owner->identity, index, first_row, Phase::begin_row);
        if (status != Status::complete) return status;
        const auto type = *first_row.type_48;
        std::uint32_t answer = 0;
        if (type == 1) {
            status = active(index, answer);
            if (status != Status::complete) return status;
            if (answer) {
                if (!vector_valid(state->skill_vector_b4, true)) return Status::invalid_source_fact;
                const auto* fresh = state->skill_vector_b4->begin;
                const auto fresh_first = reinterpret_cast<std::uintptr_t>(fresh);
                const auto fresh_last = reinterpret_cast<std::uintptr_t>(state->skill_vector_b4->end);
                if (index >= (fresh_last - fresh_first) / sizeof(std::uintptr_t) || !fresh[index])
                    return Status::invalid_source_fact;
                auto* owner = current_character(state);
                if (!owner) return Status::invalid_source_fact;
                Response ignored{};
                status = call(Operation::pre, fresh[index], index, 0, 0,
                              owner->identity, 0, nullptr, ignored);
                if (status != Status::complete) return status;
                // AI_BeginSkill discards OnPre's source return and returns 1.
                result.value = 1;
                return Status::complete;
            }
        }
        auto* owner = current_character(state);
        if (!owner) return Status::invalid_source_fact;
        Response response{};
        status = call(Operation::check_usable, state->ai, index, 0, 0,
                      owner->identity, 0, nullptr, response);
        if (status != Status::complete) return status;
        result.value = response.word;
        if (!response.word) return Status::complete;

        // Source mutation order is cc, d0, d1; the GetCharSkill result in
        // first_row remains captured and its +8 byte is reread after these.
        *state->fields->current_slot_cc = index; ++result.field_writes;
        *state->fields->continued_d0 = 0; ++result.field_writes;
        *state->fields->last_d1 = 0; ++result.field_writes;
        const std::uint8_t moving = *first_row.moving_08 ? 1u : 0u;
        owner = current_character(state);
        if (!owner || !machine_valid(owner->skill_machine_4fc))
            return Status::invalid_source_fact;
        status = set_state(owner->skill_machine_4fc, index, moving, 0, false);
        if (status != Status::complete) return status;

        // Every owner load below corresponds to a fresh AI+4 source read.
        owner = current_character(state);
        if (!owner) return Status::invalid_source_fact;
        status = call(Operation::is_player, owner->identity, index, 0, 0,
                      0, 0, nullptr, response);
        if (status != Status::complete || !response.word) {
            if (status != Status::complete) return status;
            return final_using_skill();
        }

        owner = current_character(state);
        if (!owner) return Status::invalid_source_fact;
        status = call(Operation::property_add_int, owner->identity, 216, 1, 0,
                      0, 0, nullptr, response);
        if (status != Status::complete) return status;
        // 0x3d87ac captures AI+4 before 0x3d87c0 captures the trophy
        // manager. PROPS_GetInt uses that retained Character receiver.
        owner = current_character(state);
        if (!owner) return Status::invalid_source_fact;
        const auto property_owner = owner->identity;
        status = call(Operation::trophy_manager, state->ai, 0, 0, 0,
                      0, 0, nullptr, response);
        if (status != Status::complete) return status;
        result.trophy_manager = response.identity;

        status = call(Operation::property_get_int, property_owner, 216, 0, 0,
                      0, 0, nullptr, response);
        if (status != Status::complete) return status;
        if (response.signed_word > 199) {
            owner = current_character(state);
            if (!owner) return Status::invalid_source_fact;
            status = call(Operation::is_local_player, owner->identity, 0, 0, 0,
                          0, 0, nullptr, response);
            if (status != Status::complete) return status;
            if (response.word) {
                status = call(Operation::trophy_names, state->ai, 0, 0, 0,
                              result.trophy_manager, 0, nullptr, response);
                if (status != Status::complete) return status;
                const auto names = response.trophy_names;
                if (names.count > 1048576 || (names.count && !names.items))
                    return Status::invalid_source_fact;
                std::int32_t trophy = -1;
                for (std::uint32_t n = 0; n < names.count; ++n) {
                    if (!names.items[n]) return Status::invalid_source_fact;
                    ++result.trophy_name_comparisons;
                    if (std::strcmp(names.items[n], "epic_withskills") == 0) {
                        if (n > std::uint32_t(std::numeric_limits<std::int32_t>::max()))
                            return Status::invalid_source_fact;
                        trophy = static_cast<std::int32_t>(n);
                        break;
                    }
                }
                result.trophy_index = trophy;
                status = call(Operation::unlock_trophy, result.trophy_manager,
                              unsigned_bits(trophy), 0, trophy, owner->identity,
                              0, nullptr, response);
                if (status != Status::complete) return status;
            }
        }
        return final_using_skill();
    }

    Status final_using_skill() {
        auto* owner = current_character(state);
        if (!owner) return Status::invalid_source_fact;
        return query_using(owner, result.value);
    }

    Status end(std::uint32_t index) {
        auto* owner = current_character(state);
        if (!owner) return Status::invalid_source_fact;
        std::uint32_t using_skill = 0;
        auto status = query_using(owner, using_skill);
        if (status != Status::complete || !using_skill) return status;
        result.phase = Phase::end_using_skill;
        owner = current_character(state);
        if (!owner) return Status::invalid_source_fact;
        SkillRow row_view{};
        status = row(owner->identity, index, row_view, Phase::end_row);
        if (status != Status::complete) return status;
        if (*row_view.type_48 != 2) return Status::complete;
        if (!*state->fields->continued_d0) {
            *state->fields->last_d1 = 1;
            ++result.field_writes;
            return Status::complete;
        }
        owner = current_character(state);
        if (!owner || !owner->stop_skill_loop_receiver_49c)
            return Status::invalid_source_fact;
        Response ignored{};
        return call(Operation::stop_skill_loop,
                    owner->stop_skill_loop_receiver_49c, index, 1, 0,
                    owner->identity, row_view.identity, nullptr, ignored);
    }
};

Status validate(State* state, const Services* services, Result* output,
                bool needs_vector) {
    Range controls[3]{};
    if (!object_range(state, sizeof(*state), alignof(State), controls[0]) ||
        !object_range(services, sizeof(*services), alignof(Services), controls[1]) ||
        !object_range(output, sizeof(*output), alignof(Result), controls[2]))
        return Status::invalid_argument;
    for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlaps(controls[i], controls[j])) return Status::invalid_argument;
    if (!services->invoke || !state_valid(state, needs_vector)) return Status::invalid_argument;
    if (!source_backing_disjoint(state)) return Status::invalid_argument;
    // Check the complete borrowed graph against both controls and each other
    // before the first output write. The backing-array extent is checked by
    // vector_valid before it is added here.
    Range source[20]{};
    unsigned count = 0;
    auto* character = state->owner_04->current;
    auto* machine = character->skill_machine_4fc;
    const auto append = [&](const void* pointer, std::size_t size, std::size_t alignment) {
        return add_disjoint(source, count, pointer, size, alignment);
    };
    if (!append(state->owner_04, sizeof(*state->owner_04), alignof(OwnerSlot)) ||
        !append(state->fields, sizeof(*state->fields), alignof(Fields)) ||
        !append(state->fields->current_slot_cc, sizeof(std::uint32_t), alignof(std::uint32_t)) ||
        !append(state->fields->continued_d0, sizeof(std::uint8_t), alignof(std::uint8_t)) ||
        !append(state->fields->last_d1, sizeof(std::uint8_t), alignof(std::uint8_t)) ||
        !append(character, sizeof(*character), alignof(Character)) ||
        !append(machine, sizeof(*machine), alignof(Machine)) ||
        !append(machine->state_query, sizeof(*machine->state_query), alignof(character_skill_state_queries::Machine)) ||
        !append(machine->state_query->current_state_id, sizeof(std::int32_t), alignof(std::int32_t)) ||
        !append(machine->animation_28, sizeof(std::int32_t), alignof(std::int32_t)) ||
        !append(machine->skill_index_54, sizeof(std::uint32_t), alignof(std::uint32_t)) ||
        !append(machine->moving_58, sizeof(std::uint8_t), alignof(std::uint8_t)))
        return Status::invalid_argument;
    if (state->skill_vector_b4) {
        if (!append(state->skill_vector_b4, sizeof(*state->skill_vector_b4), alignof(SkillVector)))
            return Status::invalid_argument;
        const auto first = reinterpret_cast<std::uintptr_t>(state->skill_vector_b4->begin);
        const auto last = reinterpret_cast<std::uintptr_t>(state->skill_vector_b4->end);
        if (last != first && !append(state->skill_vector_b4->begin, last - first, alignof(std::uintptr_t)))
            return Status::invalid_argument;
    }
    for (const auto control : controls)
        for (unsigned i = 0; i < count; ++i)
            if (overlaps(control, source[i])) return Status::invalid_argument;
    return Status::complete;
}

Status initialize_result(State* state, const Services* services, Result* output,
                         bool needs_vector) {
    const auto status = validate(state, services, output, needs_vector);
    if (status != Status::complete) return status;
    *output = {};
    output->trophy_index = -1;
    return Status::complete;
}
} // namespace

Status set_skill_state(State* state, Machine* machine, std::uint32_t index,
                       std::uint8_t moving, std::uintptr_t payload,
                       bool force, const Services* services, Result* output) {
    const auto status = validate(state, services, output, false);
    if (status != Status::complete) return status;
    if (!machine_valid(machine)) return Status::invalid_argument;
    Range machine_range{}, existing_machine_range{};
    if (!object_range(machine, sizeof(*machine), alignof(Machine), machine_range) ||
        !object_range(state->owner_04->current->skill_machine_4fc,
                      sizeof(Machine), alignof(Machine), existing_machine_range) ||
        machine != state->owner_04->current->skill_machine_4fc ||
        !overlaps(machine_range, existing_machine_range))
        return Status::invalid_argument;
    *output = {};
    output->trophy_index = -1;
    Run run{state, *services, *output};
    const auto result = run.set_state(machine, index, moving, payload, force);
    if (result == Status::complete) output->phase = Phase::complete;
    return result;
}

Status is_skill_active(State* state, std::uint32_t index,
                       const Services* services, Result* output) {
    const auto status = initialize_result(state, services, output, true);
    if (status != Status::complete) return status;
    Run run{state, *services, *output};
    const auto result = run.active(index, output->value);
    if (result == Status::complete) output->phase = Phase::complete;
    return result;
}

Status execute(State* state, Command command, std::uint32_t index,
               const Services* services, Result* output) {
    if (command > Command::use) return Status::invalid_argument;
    const auto status = initialize_result(state, services, output,
                                          command != Command::end);
    if (status != Status::complete) return status;
    Run run{state, *services, *output};
    Status result = Status::complete;
    if (command == Command::begin) result = run.begin(index);
    else if (command == Command::end) result = run.end(index);
    else {
        result = run.begin(index);
        if (result == Status::complete && output->value) {
            result = run.end(index);
            if (result == Status::complete) output->value = 1;
        }
    }
    if (result == Status::complete) output->phase = Phase::complete;
    return result;
}

} // namespace dh2::character_ai_skill_commands_v1
