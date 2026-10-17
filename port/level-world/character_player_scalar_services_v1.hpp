#pragma once
#include "../adam-script-runtime/script_runtime.h"
#include <cstdint>
#include <map>

namespace dh2::character_player_scalar_services_v1 {
// LuaScript+0x1c private integer dictionary. Borrow the one dictionary owned
// alongside the existing AIS/VM, never Character properties or a global map.
using IntegerMap=std::map<std::uint32_t,std::int32_t>;
struct State {std::uintptr_t ais;IntegerMap* integers;};
struct Arguments {const dh2_script_value* values;std::uint32_t count;};
struct Services {
    void* context;
    // Non-string keys and non-number values require genuine source Value
    // coercion. Converted string storage remains live through dictionary use.
    // Zero is normal return; nonzero/throw preserves completed effects.
    int (*get_string)(void*,const dh2_script_value*,const char**);
    int (*get_number)(void*,const dh2_script_value*,float*);
};
enum class Phase : std::uint32_t {none,string,number,dictionary,complete};
enum class Status : std::int32_t {complete=0,invalid_argument=-1,provider_failed=-2,unsupported_domain=-3};
struct Result {
    std::uintptr_t ais;
    Phase phase;
    std::uint32_t conversions,hash_reads,inserted,returned;
    std::int32_t value;
};
Status get(const State*,const char* name,std::int32_t*,Result*);
Status set(const State*,const char* name,std::int32_t,Result*);
Status get_callback(const State*,const Arguments*,const Services*,Result*);
Status set_callback(const State*,const Arguments*,const Services*,Result*);
struct Bindings {const State* state;const Services* services;};
int get_int(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t) noexcept;
int set_int(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t) noexcept;
// Source missing GetInt inserts zero, and hashes again in nested SetInt. Keys
// are unsigned hashes; first NUL ends a name, including hash collisions.
// Supported native direct coercions: source strings and finite numeric values
// in signed32 range (truncation toward zero). Others require services or fail.
// One thread, no same-output/owner destruction/rebinding/reentry. The borrowed
// map, VM callback controls and provider string backing survive VM close.
// Distinct outputs may nest in coercion callbacks; live Arguments are reread
// after first key conversion by SetInt. Map/source allocator internals are
// represented by std::map; no original allocator/tree-body credit is claimed.
// Provider contexts and error output must stay live through synchronous return
// and must not alias control/output storage. Names require a NUL within 4096
// bytes; out-of-range/nonfinite numeric coercions are unsupported explicitly.
}
