#pragma once
#include <cstdint>

namespace dh2::character_ai_in_combat {

struct State { std::uintptr_t ai, owner; };
enum class Query : std::uint32_t {
    has_aggro, is_aggroed, state_is_attacking, state_is_using_skill,
    state_is_casting,
};
struct Services {
    void* context;
    // Return0 means invocation succeeded. Preserve its raw source word in
    // value. subject is the captured AI for the first two queries, or the
    // freshly captured Character owner for each state-machine query.
    std::int32_t (*invoke)(void*, State*, Query, std::uintptr_t subject,
                           std::uint32_t* value);
};
struct Result { std::uint32_t value, service_calls, last_query; };
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3,
};

// Original AI_IsInCombat caller at0x3d4bc4. Queries outgoing/incoming aggro,
// then Attacking, UsingSkill and Casting. The first four truthy results
// return canonical1; the final tail call returns its raw word. Owner is read
// freshly for each state query. No Block predicate or cached combat snapshot.
// Callee bodies/storage remain genuine synchronous borrowed services. All
// owners, state, services and output stay live for one owning thread. Services
// may replace owner; errors retain completed effects. Independent outputs are
// required for nested invocations. Invalid aliases reject before effects.
Status evaluate(State*, const Services*, Result*);

}  // namespace dh2::character_ai_in_combat
