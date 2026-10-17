#include "player_skill_progression_v1.hpp"

#include <cstring>
#include <limits>

namespace dh2::player_skill_progression_v1 {
namespace {

constexpr std::int32_t kSkillTreeProperty = 28;
constexpr std::int32_t kCharacterLevelProperty = 19;
constexpr std::int32_t kSkillPointsProperty = 157;
constexpr std::int32_t kPotionCapacityProperty = 194;

std::int32_t signed_bits(std::uint32_t bits) noexcept {
    std::int32_t value = 0;
    static_assert(sizeof(value) == sizeof(bits));
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

std::int32_t asr8(std::int32_t value) noexcept {
    const auto bits = static_cast<std::uint32_t>(value);
    return signed_bits((bits >> 8) | ((bits & 0x80000000u) ? 0xff000000u : 0u));
}

bool live_resolved(const data::PropertyView* properties) noexcept {
    return properties && properties->resolved &&
           reinterpret_cast<std::uintptr_t>(properties->resolved) %
                   alignof(std::int32_t) ==
               0;
}

Predicate resolve_skill(const data::PropertyView* properties,
                        const data::SkillTables* tables,
                        std::uintptr_t character,
                        std::uint32_t skill_index) noexcept {
    Predicate result{};
    result.status = PredicateStatus::missing_projection;
    if (!character || !live_resolved(properties) || !tables ||
        tables->skill_lists.empty())
        return result;

    const auto selector = properties->resolved[kSkillTreeProperty];
    std::size_t list_index = 0;
    if (selector < 0 || static_cast<std::size_t>(selector) >=
                            tables->skill_lists.size()) {
        list_index = 3;
    } else {
        list_index = static_cast<std::size_t>(selector);
    }
    if (list_index >= tables->skill_lists.size()) {
        result.status = PredicateStatus::source_row_unavailable;
        return result;
    }
    const auto& list = tables->skill_lists[list_index].members;
    if (skill_index >= list.size()) {
        result.status = PredicateStatus::source_row_unavailable;
        return result;
    }
    const auto skill_id = list[skill_index];
    if (skill_id < 0 || static_cast<std::size_t>(skill_id) >= tables->skills.size()) {
        result.status = PredicateStatus::source_row_unavailable;
        return result;
    }
    result.status = PredicateStatus::evaluated;
    result.character_level = asr8(properties->resolved[kCharacterLevelProperty]);
    result.required_level = tables->skills[static_cast<std::size_t>(skill_id)].level;
    return result;
}

struct Invocation {
    std::uintptr_t character{};
    Services services{};
    Result* result{};
};

Status invoke(Invocation& call, Operation operation, std::int32_t a = 0,
              std::int32_t b = 0, std::int32_t c = 0,
              std::int32_t d = 0, std::uint32_t* returned = nullptr,
              const data::PlayerSavegameV1* savegame = nullptr) noexcept {
    if (!call.services.invoke) return Status::missing_service;
    Request request{};
    request.operation = operation;
    request.character = call.character;
    request.savegame = savegame;
    request.arguments[0] = a;
    request.arguments[1] = b;
    request.arguments[2] = c;
    request.arguments[3] = d;
    Response response{};
    ++call.result->service_calls;
    call.result->last_operation = static_cast<std::uint32_t>(operation);
    int status = 0;
    try {
        status = call.services.invoke(call.services.context, &request, &response);
    } catch (...) {
        return Status::service_failed;
    }
    if (status) return Status::service_failed;
    if (returned) *returned = response.word;
    return Status::complete;
}

Status debug_pair(Invocation& call, std::int32_t load_site,
                  std::int32_t query_site) noexcept {
    auto status = invoke(call, Operation::debug_load, load_site);
    if (status != Status::complete) return status;
    return invoke(call, Operation::debug_query, query_site);
}

Status current_savegame(Invocation& call,
                        data::PlayerSavegameV1*& savegame) noexcept {
    if (!call.services.current_savegame) return Status::missing_service;
    savegame = nullptr;
    int status = 0;
    try {
        status = call.services.current_savegame(call.services.context,
                                                call.character, &savegame);
    } catch (...) {
        return Status::service_failed;
    }
    return status ? Status::service_failed : Status::complete;
}

bool valid_savegame(const data::PlayerSavegameV1* savegame,
                    std::uintptr_t character) noexcept {
    return savegame && savegame->character() == character &&
           savegame->skills_initialized();
}

}  // namespace

Predicate is_skill_available(const data::PropertyView* properties,
                             const data::SkillTables* tables,
                             std::uintptr_t character,
                             std::uint32_t skill_index) noexcept {
    auto result = resolve_skill(properties, tables, character, skill_index);
    if (result.status == PredicateStatus::evaluated)
        result.value = result.character_level >= result.required_level;
    return result;
}

Predicate can_increment_skill(const data::PropertyView* properties,
                              const data::SkillTables* tables,
                              const data::PlayerSavegameV1* savegame,
                              std::uintptr_t character,
                              std::uint32_t skill_index) noexcept {
    Predicate result{};
    result.status = PredicateStatus::missing_projection;
    if (!character) return result;
    if (!savegame) {
        result.status = PredicateStatus::evaluated;
        result.value = 0;
        return result;
    }
    if (savegame->character() != character) {
        result.status = PredicateStatus::owner_mismatch;
        return result;
    }
    if (!savegame->skills_initialized() ||
        skill_index >= savegame->skills().size()) {
        result.status = PredicateStatus::evaluated;
        result.value = 0;
        result.saved_level = -1;
        return result;
    }
    result.saved_level = savegame->skill_level(skill_index);
    if (result.saved_level < 0) {
        result.status = PredicateStatus::source_row_unavailable;
        return result;
    }
    result = resolve_skill(properties, tables, character, skill_index);
    if (result.status != PredicateStatus::evaluated) return result;
    result.saved_level = savegame->skill_level(skill_index);
    const auto difference = signed_bits(
        static_cast<std::uint32_t>(result.character_level) -
        static_cast<std::uint32_t>(result.required_level));
    // Original ARM CMP/MOVLE compares the zero-extended LDRH saved value
    // against the wrapped signed 32-bit subtraction result.
    result.value = result.saved_level <= difference;
    return result;
}

Status increment_skill(const data::PropertyView* properties,
                       const data::SkillTables* tables,
                       std::uintptr_t character, std::uint32_t skill_index,
                       bool test_only, const Services* services,
                       Result* result) noexcept {
    if (!result || !character || !properties || !tables || !services ||
        reinterpret_cast<std::uintptr_t>(result) % alignof(Result) ||
        reinterpret_cast<std::uintptr_t>(services) % alignof(Services))
        return Status::invalid_argument;
    *result = {};
    result->decision = IncrementDecision::cannot_increment;
    Invocation call{character, *services, result};
    if (!call.services.invoke || !call.services.current_savegame)
        return Status::missing_service;
    if (!live_resolved(properties)) return Status::invalid_source_fact;

    data::PlayerSavegameV1* savegame = nullptr;
    auto status = current_savegame(call, savegame);
    if (status != Status::complete) return status;
    if (!savegame) return Status::unsupported_source_boundary;
    if (savegame->character() != character) return Status::invalid_source_fact;
    if (!savegame->skills_initialized()) return Status::unsupported_source_boundary;
    if (skill_index >= savegame->skills().size())
        return Status::unsupported_source_boundary;

    result->skill_list_index = static_cast<std::int32_t>(skill_index);
    result->skill_points_before = asr8(properties->resolved[kSkillPointsProperty]);
    if (result->skill_points_before <= 0) {
        result->decision = IncrementDecision::no_skill_points;
        return debug_pair(call, 0x3bcdb8, 0x3bcde0);
    }

    const auto available = is_skill_available(properties, tables, character,
                                               skill_index);
    if (available.status != PredicateStatus::evaluated)
        return available.status == PredicateStatus::source_row_unavailable
                   ? Status::unsupported_source_boundary
                   : Status::invalid_source_fact;
    if (!available.value) {
        result->decision = IncrementDecision::skill_unavailable;
        return debug_pair(call, 0x3bcce0, 0x3bcd08);
    }

    std::uint32_t raw = 0;
    status = invoke(call, Operation::skill_limit, 0, 0, 0, 0, &raw);
    if (status != Status::complete) return status;
    auto cap = signed_bits(raw);

    status = invoke(call, Operation::unlocked_difficulty, 0, 0, 0, 0, &raw,
                    savegame);
    if (status != Status::complete) return status;
    auto difficulty = signed_bits(raw);
    if (difficulty == 1) {
        status = invoke(call, Operation::skill_limit, 1, 0, 0, 0, &raw);
        if (status != Status::complete) return status;
        cap = signed_bits(raw);
    } else {
        // The second query is source-fresh and is intentionally not replaced
        // by the first DifficultyUnlocked result.
        status = invoke(call, Operation::unlocked_difficulty, 0, 0, 0, 0,
                        &raw, savegame);
        if (status != Status::complete) return status;
        difficulty = signed_bits(raw);
        if (difficulty == 2) {
            status = invoke(call, Operation::skill_limit, 2, 0, 0, 0, &raw);
            if (status != Status::complete) return status;
            cap = signed_bits(raw);
        }
    }
    result->skill_cap = cap;

    status = current_savegame(call, savegame);
    if (status != Status::complete) return status;
    if (!valid_savegame(savegame, character) ||
        skill_index >= savegame->skills().size())
        return Status::unsupported_source_boundary;
    const auto saved = savegame->skill_level(skill_index);
    if (saved < 0) return Status::unsupported_source_boundary;
    result->old_saved_level = static_cast<std::uint16_t>(saved);
    result->new_saved_level = result->old_saved_level;
    if (cap <= saved) {
        result->decision = IncrementDecision::at_or_above_cap;
        result->source_return = 0;
        return Status::complete;
    }

    // CanIncrementSkill itself rereads the save vector and PropertyState.
    status = current_savegame(call, savegame);
    if (status != Status::complete) return status;
    const auto can_increment = can_increment_skill(properties, tables, savegame,
                                                    character, skill_index);
    if (can_increment.status != PredicateStatus::evaluated)
        return can_increment.status == PredicateStatus::owner_mismatch
                   ? Status::invalid_source_fact
                   : Status::unsupported_source_boundary;
    if (!can_increment.value) {
        result->decision = IncrementDecision::cannot_increment;
        result->source_return = 0;
        return Status::complete;
    }
    if (test_only) {
        result->decision = IncrementDecision::test_only_accepted;
        result->source_return = 1;
        return Status::complete;
    }

    status = invoke(call, Operation::add_property, kSkillPointsProperty, -1);
    if (status != Status::complete) return status;

    // The source loads Character+0x14e8 again after AddProp and mutates this
    // refreshed save owner. Never keep the pre-callback save pointer here.
    status = current_savegame(call, savegame);
    if (status != Status::complete) return status;
    if (!valid_savegame(savegame, character) ||
        skill_index >= savegame->skills().size())
        return Status::unsupported_source_boundary;
    const auto old_level = savegame->skill_level(skill_index);
    if (old_level < 0) return Status::unsupported_source_boundary;
    const auto next = static_cast<std::uint16_t>(
        static_cast<std::uint32_t>(static_cast<std::uint16_t>(old_level)) + 1u);
    std::string owner_error;
    if (!savegame->set_skill_level(skill_index, next, owner_error))
        return Status::unsupported_source_boundary;
    result->old_saved_level = static_cast<std::uint16_t>(old_level);
    result->new_saved_level = next;

    status = invoke(call, Operation::update_all_skills);
    if (status != Status::complete) return status;
    status = invoke(call, Operation::recalculate_properties, 1);
    if (status != Status::complete) return status;

    const auto potion_capacity = asr8(properties->resolved[kPotionCapacityProperty]);
    const auto nonnegative = potion_capacity < 0 ? 0u
                                                  : static_cast<std::uint32_t>(potion_capacity);
    status = invoke(call, Operation::set_potion_capacity,
                    static_cast<std::int32_t>(static_cast<std::uint8_t>(nonnegative)));
    if (status != Status::complete) return status;
    status = debug_pair(call, 0x3bcef0, 0x3bcf18);
    if (status != Status::complete) return status;
    result->decision = IncrementDecision::incremented;
    result->source_return = 1;
    return Status::complete;
}

Status initialize_skill_slots(std::uintptr_t character, const Services* services,
                              InitSlotsResult* result) noexcept {
    if (!result || !character || !services ||
        reinterpret_cast<std::uintptr_t>(result) % alignof(InitSlotsResult) ||
        reinterpret_cast<std::uintptr_t>(services) % alignof(Services))
        return Status::invalid_argument;
    *result = {};
    Invocation call{character, *services, nullptr};
    Result scratch{};
    call.result = &scratch;
    struct ResultSync {
        InitSlotsResult* output;
        Result* scratch;
        ~ResultSync() {
            output->service_calls = scratch->service_calls;
            output->last_operation = scratch->last_operation;
        }
    } result_sync{result, &scratch};
    if (!call.services.invoke) return Status::missing_service;

    std::uint32_t raw = 0;
    auto status = invoke(call, Operation::has_skill_slots, 0, 0, 0, 0, &raw);
    if (status != Status::complete) return status;
    if (raw) {
        result->service_calls = scratch.service_calls;
        result->last_operation = scratch.last_operation;
        return Status::complete;
    }
    status = invoke(call, Operation::set_skill_in_slot, 0, 0);
    if (status != Status::complete) return status;
    status = invoke(call, Operation::swap_equipment_set);
    if (status != Status::complete) return status;
    status = invoke(call, Operation::set_skill_in_slot, 0, 0);
    if (status != Status::complete) return status;
    status = invoke(call, Operation::swap_equipment_set);
    if (status != Status::complete) return status;

    if (!call.services.current_savegame) return Status::missing_service;
    data::PlayerSavegameV1* savegame = nullptr;
    status = current_savegame(call, savegame);
    if (status != Status::complete) return status;
    if (savegame && savegame->character() != character)
        return Status::invalid_source_fact;
    const auto level = valid_savegame(savegame, character)
                           ? savegame->skill_level(0)
                           : -1;
    if (level == 0) {
        status = invoke(call, Operation::increment_skill, 0, 0, 0, 0, &raw,
                        savegame);
        if (status != Status::complete) return status;
        result->increment_called = 1;
        // Original _InitSkillsSlots ignores the returned bool.
    }
    result->service_calls = scratch.service_calls;
    result->last_operation = scratch.last_operation;
    return Status::complete;
}

}  // namespace dh2::player_skill_progression_v1
