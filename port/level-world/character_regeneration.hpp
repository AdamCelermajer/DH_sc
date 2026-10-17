#pragma once
#include <cstdint>

namespace dh2::character_regeneration {
// Stable native owner tokens, not an original ARM overlay. The resolved sheet
// corresponds to Character+0xff4; properties to Character+0x560.
struct State {std::uintptr_t character,properties,resolved_sheet;};
struct Globals {std::uintptr_t debug_switches;};
enum class Operation : std::uint8_t {read_property,debug_load,string_construct,debug_query,string_destroy,add_property};
struct Request {
    Operation operation;
    std::uintptr_t subject,sheet;
    std::uint32_t property,amount;
    const char* text;
};
struct Reply {std::uint32_t word;std::uintptr_t identity;};
struct Services {
    void* context;
    // Zero success. Reads are actual _GetProperty(resolved_sheet,id), not a
    // fresh RecalcProperty. Add uses actual PROPS_Add; the maintained property
    // adapter may reuse dh2_property_add with the live owned PropertyView.
    // Construct creates/retains genuine native string storage and returns its
    // nonzero token in Reply.identity. Query/destroy receive that same token.
    // Debug load/query are real providers, with their own configuration/map
    // effects; successful no-ops are not substitutes. All other return words
    // are discarded. Providers may mutate actor scalar state or global debug
    // selection; captured current/max/positive amount remain unchanged.
    std::int32_t (*invoke)(void*,const Request*,Reply*);
};
struct Result {std::uint32_t calls,last_operation,current,maximum,positive_amount,added;};
enum class Status : std::int32_t {complete,invalid_argument,service_unavailable,service_failed};
// Complete original236B HP/MP caller each. Negative raw requests use maximum;
// wrap32(current+amount) is capped by a signed comparison, and only a positive
// resulting amount reaches debug load -> construct -> query -> destroy -> add.
Status regen_hp(const State*,const Globals*,std::uint32_t raw_amount,const Services*,Result*);
Status regen_mp(const State*,const Globals*,std::uint32_t raw_amount,const Services*,Result*);

// One owning thread retains all records, owners, provider-created string backing
// and retired debug objects until synchronous return. Entry State and Services
// are captured; Globals.debug_switches is read only at the positive debug phase.
// Control records must be aligned/disjoint. Same owner/output or provider-owned
// string reentry is forbidden; independent owners/backends may nest. Errors or
// exceptions stop immediately with preceding effects retained: no rollback,
// additional destructor, property add or debug cleanup is invented. An error
// after successful string construction requires the adapter to retain/manage
// that backing; source C++ exception unwinding is outside this port boundary.
} // namespace dh2::character_regeneration
