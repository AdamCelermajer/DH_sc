#pragma once

#include "../game-data/player_savegame_v1.hpp"
#include "../game-data/properties.hpp"
#include "../game-data/skill_tables.hpp"

#include <cstdint>

namespace dh2::player_skill_progression_v1 {

enum class PredicateStatus : std::uint32_t {
    evaluated,
    missing_projection,
    owner_mismatch,
    source_row_unavailable,
};

struct Predicate {
    PredicateStatus status{};
    std::uint32_t value{};
    std::int32_t character_level{};
    std::int32_t required_level{};
    std::int32_t saved_level{};
};

// The PropertyView's resolved sheet is borrowed live source state. SkillTree
// (28) selects a SkillList, with the source fallback row 3; property Level
// (19) is an 8-bit fixed value and follows Character::GetLevel's ASR #8.
Predicate is_skill_available(const data::PropertyView*,
                             const data::SkillTables*,
                             std::uintptr_t character,
                             std::uint32_t skill_index) noexcept;
Predicate can_increment_skill(const data::PropertyView*,
                              const data::SkillTables*,
                              const data::PlayerSavegameV1*,
                              std::uintptr_t character,
                              std::uint32_t skill_index) noexcept;

enum class Operation : std::uint32_t {
    has_skill_slots,
    set_skill_in_slot,
    swap_equipment_set,
    increment_skill,
    skill_limit,
    unlocked_difficulty,
    add_property,
    update_all_skills,
    recalculate_properties,
    set_potion_capacity,
    debug_load,
    debug_query,
};

struct Request {
    Operation operation{};
    std::uintptr_t character{};
    const data::PlayerSavegameV1* savegame{};
    // Operation-specific source words. Slot assignment uses (slot,row),
    // skill_limit uses (difficulty), property mutation uses (id,delta), and
    // debug requests use the original load/query return addresses.
    std::int32_t arguments[4]{};
};

struct Response {
    std::uint32_t word{};
};

struct Services {
    void* context{};
    // Reads Character+0x14e8 at each source read point. Return zero on a
    // successful read (including a null source pointer); nonzero is an adapter
    // failure. Returned storage must remain live until its use finishes.
    int (*current_savegame)(void*, std::uintptr_t character,
                            data::PlayerSavegameV1** savegame){};
    // Executes a named source boundary synchronously. Nonzero is a port
    // service failure, distinct from a normally returned source false value.
    int (*invoke)(void*, const Request*, Response*){};
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    missing_service,
    service_failed,
    invalid_source_fact,
    unsupported_source_boundary,
};

enum class IncrementDecision : std::uint32_t {
    incremented,
    test_only_accepted,
    no_skill_points,
    skill_unavailable,
    at_or_above_cap,
    cannot_increment,
};

struct Result {
    IncrementDecision decision{};
    std::uint32_t source_return{};
    std::uint32_t service_calls{};
    std::uint32_t last_operation{};
    std::uint16_t old_saved_level{};
    std::uint16_t new_saved_level{};
    std::int32_t skill_points_before{};
    std::int32_t skill_cap{};
    std::int32_t skill_list_index{};
};

// Bounded Character::IncSkill(0x3bcc58) orchestration over the one live
// PlayerSavegameV1 and PropertyView. Property/debug/design/CharAI services are
// required at their source call sites. Reached mutations are not rolled back
// when a later provider fails. Missing-save Debug continuation and unsupported
// native calls fail closed.
Status increment_skill(const data::PropertyView*, const data::SkillTables*,
                       std::uintptr_t character, std::uint32_t skill_index,
                       bool test_only, const Services*, Result*) noexcept;

struct InitSlotsResult {
    std::uint32_t service_calls{};
    std::uint32_t last_operation{};
    std::uint32_t increment_called{};
};

// Exact Character::_InitSkillsSlots(0x3b3a90) ordered body. The saved slot
// writer and equipment-set swap remain mandatory source services; this does
// not choose or synthesize a second equipment-set owner.
Status initialize_skill_slots(std::uintptr_t character, const Services*,
                              InitSlotsResult*) noexcept;

}  // namespace dh2::player_skill_progression_v1
