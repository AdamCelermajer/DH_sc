#pragma once
#include "character_ai_skill_script_update.hpp"
#include <cstdint>
namespace dh2::character_ai_skill_script_check {
using State=character_ai_skill_script_update::State;
using Character=character_ai_skill_script_update::Character;
using ReturnValues=character_ai_skill_script_update::ReturnValues;
using ValueVector=character_ai_skill_script_update::ValueVector;
enum class Check : std::uint32_t {usable,active};
enum class Operation : std::uint32_t {
    construct_values,call_set_skill,erase_values,call_skill_check,
    operator_index,get_bool,destroy_values
};
struct Request {
    Operation operation;Check check;
    std::uintptr_t skill,script,arguments,resource,first,last,value;
    std::uint32_t index;
    const char* function;
};
struct Response {std::uintptr_t value;std::uint32_t boolean;};
struct Services {
    void* context;
    // Actual byte stride of the provider-owned contiguous native Value records.
    // No ARM112-byte overlay. Positive stride<=4096 and count<=1e6 are port
    // storage bounds; provider owns all values and preserves their identities.
    std::uint32_t native_value_stride;
    std::int32_t (*invoke)(void*,State*,const Request*,ReturnValues*,Response*);
};
enum class Phase : std::uint32_t {not_started,construct,set_skill,erase,check,index,boolean,destroy,complete};
enum class Decision : std::uint32_t {converted,no_script,set_skill_error,check_error,insufficient_values};
struct Result {
    std::uintptr_t resource,value;
    Phase phase;Decision decision;
    std::uint32_t service_calls,set_skill_error,check_error,value_count,
                  boolean,erased,constructed,destroyed;
};
enum class Status : std::int32_t {complete,invalid_argument,invalid_source_fact,service_unavailable,service_failed};
// Complete source OnSkillCheck_Usable288B and OnSkillCheck_Active284B callers.
// Shared construct/SetSkill/error/erase/fresh script prefix; Call OnSkillCheck
// with no explicit arguments; normal Error/insufficient results return false.
// Usable selects operator[](0); Active directly selects second record. The
// mandatory get_bool provider executes exact sfc Value coercion, not generic
// Lua truthiness or bool(number): numeric +/-0 false, NaN true; type2/7 pointer
// nonnull; string creates a temporary Lua state, pushes its fresh retained
// C-string pointer, reads Lua boolean and closes that state; other types false.
Status check(State*,Check,const Services*,Result*);
// Services perform real resource and Lua operations. A normal Lua Call error
// sets ReturnValues.error with port success; cleanup still follows. Unknown or
// absent native operations fail explicitly. Raw source callee returns other
// than getBool are discarded. getBool must return canonical0/1; its value is
// captured before the normal destructor. No FSM/skill-ID store is invented.
// One owning thread retains controls, old/new owners/scripts and all value
// backing through synchronous return. Bindings/context/stride and skill+0c
// identity are captured once. Owner/vector can change at provider boundaries;
// ReturnValues resource identity is fixed. No same-State/resource reentry or
// destruction during callbacks; independent resources may nest. No added
// destructor/rollback on port error; retained resources stay adapter-owned.
} // namespace dh2::character_ai_skill_script_check
