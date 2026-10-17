#pragma once

#include <cstdint>

namespace dh2::character_ai_update_all_skills {

enum class List : std::uint32_t { none = 0, skill = 1, faery = 2 };
enum class Operation : std::uint32_t {
    is_using_skill = 0,
    is_casting = 1,
    on_skill_update = 2,
};

// The two original CharAI vectors are projected as live pointer ranges. Their
// `begin` can change during OnSkillUpdate; the source reloads it for every slot
// after the first. `end` is captured only when each phase starts.
struct ScriptVector {
    const std::uintptr_t* begin;
    const std::uintptr_t* end;
};

struct State {
    std::uintptr_t ai;
    std::uintptr_t owner;
    ScriptVector skills;
    ScriptVector faeries;
};

struct Request {
    Operation operation;
    List list;
    std::uintptr_t subject;
    std::uint32_t index;
};
struct Response { std::uint32_t word; };
struct Services {
    void* context;
    // Synchronous borrowed source providers. The two FSM predicates preserve
    // their raw word/truthiness. OnSkillUpdate is required for each nonnull
    // slot; its missing implementation is an error, never a successful no-op.
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
};

enum class Decision : std::uint32_t {
    completed = 0,
    skipped_while_using_skill = 1,
    skipped_while_casting = 2,
};
struct Result {
    Decision decision;
    std::uint32_t service_calls;
    std::uint32_t skill_slots;
    std::uint32_t faery_slots;
    std::uint32_t script_updates;
    std::uint32_t using_skill_word;
    std::uint32_t casting_word;
    std::uint32_t last_operation;
};
enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_source_fact = 2,
    service_unavailable = 3,
    service_failed = 4,
};

// Source CharAI::UpdateAllSkills at ELF 0x3d8894. The called FSM and skill
// script bodies are typed borrowed services. Counts are snapshotted once per
// phase; vector begin pointers are reloaded per slot. Callback effects are
// retained on failure. The caller must keep all actor/script/vector storage
// live through return, and callbacks must not reenter this same State or alias
// the control/result records.
Status update(State*, const Services*, Result*);

} // namespace dh2::character_ai_update_all_skills
