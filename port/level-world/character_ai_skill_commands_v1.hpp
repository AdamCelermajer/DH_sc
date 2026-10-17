#pragma once

#include "character_skill_state_queries.hpp"

#include <cstdint>

namespace dh2::character_ai_skill_commands_v1 {

// These are logical source projections, not ARM32 memory overlays. The row
// fields remain borrowed pointers so BeginSkill can retain GetCharSkill's
// first result across Active/Usable/Pre callbacks and reread moving at the
// original point in the caller.
struct SkillRow {
    std::uintptr_t identity;
    const std::int32_t* animation_04;
    const bool* moving_08;
    const std::int32_t* type_48;
};

struct Machine {
    std::uintptr_t identity;
    std::uintptr_t owner_04;
    const character_skill_state_queries::Machine* state_query;
    std::int32_t* animation_28;
    std::uint32_t* skill_index_54;
    std::uint8_t* moving_58;
};

struct Character {
    std::uintptr_t identity;
    Machine* skill_machine_4fc;
    std::uintptr_t stop_skill_loop_receiver_49c;
};

struct OwnerSlot { Character* current; };
struct SkillVector { const std::uintptr_t* begin; const std::uintptr_t* end; };
struct Fields {
    std::uint32_t* current_slot_cc;
    std::uint8_t* continued_d0;
    std::uint8_t* last_d1;
};
struct State {
    std::uintptr_t ai;
    OwnerSlot* owner_04;
    SkillVector* skill_vector_b4;
    Fields* fields;
    std::uint32_t assertion_level;
    std::uint32_t reserved;
};

enum class Operation : std::uint32_t {
    get_skill_row,
    check_active,
    check_usable,
    pre,
    get_constant,
    get_anim_stance,
    raise_state_event,
    set_state,
    is_player,
    property_add_int,
    property_get_int,
    trophy_manager,
    is_local_player,
    trophy_names,
    unlock_trophy,
    stop_skill_loop,
    assertion_log
};

struct Request {
    Operation operation;
    std::uintptr_t subject;
    std::uintptr_t related;
    std::uintptr_t row;
    std::uint32_t index;
    std::uint32_t value;
    std::int32_t signed_value;
    std::uint32_t argument;
    const char* text;
};

struct TrophyNames {
    const char* const* items;
    std::uint32_t count;
};

struct Response {
    std::uint32_t word;
    std::int32_t signed_word;
    std::uintptr_t identity;
    SkillRow row;
    TrophyNames trophy_names;
};

struct Services {
    void* context;
    // 0 is a normal source return; nonzero/exception is a port-provider
    // failure. Query return words are preserved exactly. Active/Usable/Pre
    // should bind the existing selected source dispatch and sole Player VM.
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
};

enum class Phase : std::uint32_t {
    not_started, begin_row, active, pre, usable, write_ai_fields, set_state,
    player, property_add, trophy_manager, property_get, local_player,
    trophy_names, unlock_trophy, using_skill, end_using_skill, end_row,
    end_state, complete
};
enum class Status : std::int32_t {
    complete, invalid_argument, invalid_source_fact, service_unavailable,
    service_failed, unsupported_assertion
};
enum class Command : std::uint32_t { begin, end, use };

struct Result {
    std::uint32_t value;
    std::uint32_t service_calls;
    std::uint32_t field_writes;
    std::uint32_t trophy_name_comparisons;
    Phase phase;
    Operation last_operation;
    std::uintptr_t begin_row;
    std::uintptr_t setter_row;
    std::uintptr_t trophy_manager;
    std::int32_t trophy_index;
};

// SM_SetSkillState is the real nested source setter: it performs its own fresh
// GetCharSkill, captures animation before AnimStancedAnim/SL__LIST_IPHONE,
// conditionally reads actual stance for mask 0x200000, writes machine 28/54/58,
// then emits RaiseStateEvent(0xc355,payload) or _SetState(6,0xc355,payload).
Status set_skill_state(State*, Machine*, std::uint32_t index,
                       std::uint8_t moving, std::uintptr_t payload,
                       bool force, const Services*, Result*);

// Full bounded CharAI::AI_IsSkillActive prefix and tail. It uses the same
// current FSM, cc slot, script vector and retained Player VM as the caller.
Status is_skill_active(State*, std::uint32_t index,
                       const Services*, Result*);

// AI_BeginSkill/AI_EndSkill/AI_UseSkill source callers. The outer Begin row
// pointer is retained through Active/Usable/Pre; the nested setter makes a
// second fresh row lookup. Normal zero Usable returns before cc/d0/d1 writes.
// Player property 216/trophy flow is kept in exact source order. Reached
// trophy, property, FSM and event actions are mandatory services, never no-op
// success. Errors preserve completed source writes/callback effects.
Status execute(State*, Command, std::uint32_t index,
               const Services*, Result*);

// Borrowed State, owner/machine/row/table/trophy backing and provider context
// must remain alive through synchronous return. Callback code may mutate live
// source fields; row identity returned at Begin entry and its backing remain
// stable. No same-State reentry, rollback, timer, animation or native runtime
// ownership is introduced here. Out-of-range assertions preserve the level-1
  // logger; level-2's original null-store crash is an explicit unsupported edge.
}
