#pragma once
#include <cstdint>
namespace dh2::character_skill_state_queries {
// Logical projection of CharStateMachine+0x20 -> current-state word+0;
// never a native or ARM32 memory overlay. Null means no installed state.
struct Machine {const std::int32_t* current_state_id;};
enum class Query : std::uint32_t {using_skill,casting};
struct Result {std::uint32_t state_word,value;};
enum class Status : std::int32_t {complete,invalid_argument};
// Complete source predicates: one fresh SM_GetState read, then exact equality
// to 6 (UsingSkill) or 7 (Casting). A null current state yields ffffffff/false.
// The separate UsingSkill(unsigned skill_id) overload is not implemented here.
Status query(Query,const Machine*,Result*);
inline Status is_using_skill(const Machine* m,Result* r){return query(Query::using_skill,m,r);}
inline Status is_casting(const Machine* m,Result* r){return query(Query::casting,m,r);}
// One owning thread retains Machine/current-state/output backing throughout
// synchronous return. Aligned controls and the state word must be disjoint.
// A source state can be replaced between queries; no pointer/value is cached.
// There are no callbacks, ownership transfers, state writes or timer effects.
// Invalid port pointers/enums/aliases leave output unchanged. A null Machine
// is a port error (the original dereferences this), distinct from null current.
// Existing coordinator State.current can be passed by address when it projects
// the installed source state ID; its -1 sentinel also produces false predicates.
} // namespace dh2::character_skill_state_queries
