#pragma once
#include <cstdint>
namespace dh2::character_ai_skill_script_update {
struct Character {std::uintptr_t identity,lua_script;};
struct State {std::uintptr_t identity;Character* owner;};
// Borrowed live projection of ReturnValues+0x24. Endpoints name real retained
// value storage owned by the provider; no original112B Value overlay is used.
struct ValueVector {std::uintptr_t begin,end;};
struct ReturnValues {
    std::uintptr_t resource; // stable real provider-owned ReturnValues handle
    std::uint32_t error;    // original ReturnValues+8 / Error+4
    ValueVector* values;
};
enum class Operation : std::uint32_t {
    construct_values,call_set_skill,erase_values,call_on_skill_update,destroy_values,
    call_on_skill_cleanup
};
struct Request {
    Operation operation;
    std::uintptr_t skill,script,arguments,resource,first,last;
    const char* function;
};
struct Services {
    void* context;
    // Zero completes a real synchronous source dependency. Nonzero is a
    // separate port error, not the raw/unspecified r0 of the original callees.
    // A normal Lua error is reported in ReturnValues.error with port success;
    // it must not be converted to a provider failure that skips source cleanup.
    // Construct must own a real ReturnValues resource; Call executes the real
    // Lua overload; erase/destroy perform real value/resource effects. Unknown
    // or missing implementations fail, never return successful empty no-ops.
    std::int32_t (*invoke)(void*,State*,const Request*,ReturnValues*);
};
enum class Phase : std::uint32_t {not_started,construct,set_skill,erase,update,destroy,complete,cleanup};
enum class Decision : std::uint32_t {updated,no_script,set_skill_error,cleaned};
struct Result {
    std::uintptr_t resource;
    Phase phase;Decision decision;
    std::uint32_t service_calls,set_skill_error,erased,updated,constructed,destroyed,cleaned;
};
enum class Status : std::int32_t {complete,invalid_argument,invalid_source_fact,service_unavailable,service_failed};
// Complete original OnSkillUpdate212B caller. Capture this; construct temporary
// ReturnValues; fresh owner/script null gate; SetSkill(embedded Arguments+0c);
// test Error.word; clear nonempty return-value range; fresh owner/script;
// OnSkillUpdate(no arguments); destroy. No state/skill-ID/cooldown write is
// invented. SetSkill and OnSkillUpdate Lua bodies remain provider boundaries.
Status update(State*,const Services*,Result*);
// Source CharAI::_SkillCleanUp/_SpellCleanUp calls OnSkillCleanUp through the
// same SetSkill, ReturnValues, erase and no-argument callback sequence.
Status cleanup(State*,const Services*,Result*);
// One owning thread retains controls, actors, scripts, ReturnValues and retired
// vector backing through synchronous return, including port errors. Provider
// context retains resources after a failed operation; no added destructor or
// rollback runs. Bindings/context and skill identity are captured once. Owner
// replacement is allowed and read fresh; resource identity stays fixed, while
// error/vector may change. No same-State/resource reentry or destruction during
// callbacks; independent owners/resources may nest. Controls/live metadata are
// aligned and disjoint. Missing providers fail only at a reached operation.
// The source has no second script-null check: a missing owner or script at that
// point is an explicit invalid source boundary, not a successful skipped update.
} // namespace dh2::character_ai_skill_script_update
