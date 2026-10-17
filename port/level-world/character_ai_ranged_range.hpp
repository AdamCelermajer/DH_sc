#pragma once
#include "character_ai_melee_range.hpp"
#include <cstdint>

namespace dh2::character_ai_ranged_range {
using State=character_ai_sight::State;
using Point=character_ai_sight::Point;
using ResolvedObject=character_ai_melee_range::ResolvedObject;
enum class Operation : std::uint32_t {
    resolve_object, interaction_type, can_range_attack, target_position,
    diagnostic_switch, interaction_range,
};
struct Request {
    Operation operation;
    std::uintptr_t subject,other;
    std::uint32_t key;
    std::uint32_t *minimum,*maximum,*projectile;
};
struct Response { const void* view; std::uintptr_t identity; std::uint32_t word; };
struct Services {
    void* context;
    // Zero success. resolve_object is const GetHandle/GetObject(false), returning
    // borrowed ResolvedObject* or null. interaction_type uses resolved virtual90
    // with the fresh owner. target_position returns a borrowed live Point*.
    // can_range_attack invokes the fresh owner's virtual128 overload, preserving
    // the actual output-pointer aliases and source writes in argument order.
    // Close aliases maximum/projectile; ranged has three distinct outputs. Output
    // storage is valid until caller return; its initial contents are unspecified
    // source locals. A truthy result must provide all source outputs. Never keep
    // those pointers beyond return or overwrite the Request itself.
    // diagnostic key1 captures the global object BEFORE its Load/query/string
    // cleanup, returning that retained object identity as well as the predicate.
    // Truthy key1 selects key2; key2 Load/query/cleanup uses the SAME subject,
    // and ignores its predicate. Do not freshly resolve the global for key2.
    // interaction_range is the existing complete source interaction caller on
    // the original AI/candidate, not a distance approximation.
    std::int32_t (*invoke)(void*,State*,const Request*,Response*);
};
struct Result {
    std::uint32_t value,calls,distance_word,minimum_word,maximum_word,projectile_word,
        minimum_square_word,maximum_square_word,minimum_limit_word,
        maximum_limit_word,used_interaction_range;
    std::uintptr_t candidate,resolved,diagnostic_identity;
};
enum class Status : std::int32_t {
    complete,invalid_argument,service_unavailable,service_failed,invalid_source_fact,
};
Status evaluate_close(State*,std::uintptr_t candidate,const Services*,Result*);
Status evaluate_ranged(State*,std::uintptr_t candidate,const Services*,Result*);

// These are the complete 556/496-byte caller orchestrations. Inventory/property,
// handle, virtual methods and debug dependencies are externally supplied. One
// owning thread retains original AI, fixed candidate, resolved identity, fresh
// owners, live point backing, debug object and services through return, including
// retired backing. Facts may change synchronously; reentry into either of these
// callers for the same State, control overwrite and destruction of borrowed
// objects are forbidden. Independent calls and other source modules may nest.
// Error/exception stops without rollback or added cleanup.
// Limits are INTEGER MUL modulo2^32 then signed i2f, never float radius squares.
// Close uses limit>distance; ranged uses minimum<=distance<=maximum.
// Positions use separately rounded binary32 arithmetic. NaN unordered behavior,
// not original imported soft-float NaN payload propagation, is claimed.
} // namespace dh2::character_ai_ranged_range
