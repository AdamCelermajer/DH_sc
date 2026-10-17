#pragma once
#include <cstdint>

namespace dh2::ais_external_init_vcb {
struct State {std::uintptr_t ais;std::uint32_t flags_b8;};
struct Services {
    void* context;
    // Zero invocation success. Read fresh membership in the source VFTable
    // (unsigned hash-key map), not whether a Lua global function exists.
    // member is a canonical bool, as original IsInVFTable116B returns0/1.
    // The fixed AIS/VM/map and retired backing remain externally retained.
    std::int32_t (*contains)(void*,State*,const char* vf_alias,bool* member);
};
struct Result {std::uint32_t service_calls,writes,last_bit;};
enum class Status : std::int32_t {complete,invalid_argument,service_unavailable,service_failed};

// Complete original AISDefault92B/AISExternal408B InitVCB callers. Reset b8
// before queries; preserve ordered queries, cached accumulation and each store.
// External first runs Default, then queries OnUpdate, OnFriendSpotted, target
// ranges, master ranges. Fresh map mutation affects subsequent membership;
// direct callback writes to b8 are overwritten by the cached source accumulator.
// No script initialization, Lua state allocation, callback execution, Character
// policy flags520, AIS state-table b4 or native binding is owned here.
// One owning thread. Callbacks may mutate live flags/registrations but must not
// change AIS identity, destroy borrowed backing, overwrite services/output, or
// reenter either function on this same State. Independent State/output may nest.
// Services are captured once. Errors retain prior stores/provider effects;
// no rollback/extra source writes or callbacks are added after failure.
Status initialize_default(State*,const Services*,Result*);
Status initialize_external(State*,const Services*,Result*);
} // namespace dh2::ais_external_init_vcb
