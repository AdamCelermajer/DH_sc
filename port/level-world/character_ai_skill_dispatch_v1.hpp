#pragma once
#include "character_skill_state_queries.hpp"
#include <cstdint>

namespace dh2::character_ai_skill_dispatch_v1 {
// Borrowed logical fields; never an ARM32 layout or a second FSM/skill store.
struct Character {
    std::uintptr_t identity;
    const character_skill_state_queries::Machine* machine;
    const std::uint32_t* flags_520;
};
struct Skills {const std::uintptr_t* begin;const std::uintptr_t* end;};
struct State {
    std::uintptr_t ai;
    Character* character;
    const std::int32_t* load_phase_28;
    Skills* skills;
    const std::uint32_t* current_slot_cc;
    const std::uint32_t* assertion_level;
};
enum class Operation : std::uint32_t {loaded,usable,focus,event,blur};
enum class Service : std::uint32_t {using_skill,casting,check_usable,pre,use,post,assertion_log};
struct Request {
    Service service;
    std::uintptr_t subject;
    const character_skill_state_queries::Machine* machine;
    std::uint32_t slot,line;
    const char* format;
    const char* expression;
    const char* filename;
};
struct Response {std::uint32_t word;};
struct Services {
    void* context;
    // 0 normal source return; other values / exceptions stop the port call.
    // Query results and the usable tail word are source values, not status.
    std::int32_t (*invoke)(void*,State*,const Request*,Response*);
};
enum class Decision : std::uint32_t {
    not_started,loaded,using_skill_locked,casting,not_loaded,out_of_range,
    null_skill,dispatched,fatal_assertion
};
struct Result {
    std::uintptr_t skill;
    std::uint32_t value,slot,count,service_calls;
    Service last_service;
    Decision decision;
};
enum class Status : std::int32_t {
    complete,invalid_argument,invalid_source_fact,service_unavailable,
    service_failed,unsupported_source_assertion
};
Status execute(State*,Operation,std::uint32_t requested_slot,const Services*,Result*);
// loaded is signed phase>6. Focus/Event/Blur do NOT query phase or FSM: source
// snapshots end,begin,current and calls only a nonnull in-range skill. Usable
// captures first owner, calls UsingSkill, reloads owner for flags/Casting, then
// tests phase. Assertion level1 logs line181 and rereads the vector afterwards;
// level2 reaches the original null-store trap and is explicitly unsupported.
// Pre/Use/Post raw returns are discarded. Missing/reached providers fail.
// Services can adapt the existing Player Use Runtime: validate Request.subject
// against the same retained preparation Owner slot before check/invoke.
// One thread retains all old/new projections and backing through return. Max
// 65536 native-width slots is a port storage bound. Nonnull fields must be
// aligned/disjoint from controls/output, even on an unvisited source branch.
// Callbacks may mutate/rebind borrowed fields and nest with disjoint outputs;
// no destroyed projections or vector backing during a call. No cleanup,
// rollback, readiness publication, animation timing or FSM transition is added.
} // namespace dh2::character_ai_skill_dispatch_v1
