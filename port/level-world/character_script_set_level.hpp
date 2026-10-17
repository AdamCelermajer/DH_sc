#pragma once
#include <cstdint>

namespace dh2::character_script_set_level {
struct Character {std::uintptr_t identity,properties_identity;std::uint32_t base_level_5b8;};
// Borrowed projections, not original ARM overlays. Arguments::getAt(0) and the
// live Value::getNumber conversion are performed by the get_number service.
struct Arguments {std::uintptr_t identity;std::uint32_t count,first_type;};
struct Application {std::uintptr_t identity,design_manager;};
struct Globals {const Application* application;};
enum class Operation : std::uint8_t {get_number,design_max_level,float_to_signed,recalc_properties,regen_hp,regen_mp};
struct Request {
    Operation operation;
    std::uintptr_t subject;
    std::uint32_t argument,number_bits;
    const char* category;
    const char* key;
};
struct Services {
    void* context;
    // Zero success. get_number returns actual binary32 Value number bits, with
    // fresh Arguments::getAt(0)/Value::getNumber semantics each time. design query
    // returns raw signed32 GetInt bits. float_to_signed is the original pure
    // __aeabi_f2iz dependency, not a guessed/clamped numeric conversion body.
    // Its native adapter must implement that helper or explicitly reject an
    // unresolved/out-of-domain conversion; it must not invent a successful value.
    // RecalcProperties(true), RegenHP(-1) and RegenMP(-1) are real owned providers.
    // Their complete dependency bodies are not claimed by this source caller.
    std::int32_t (*invoke)(void*,Character*,const Request*,std::uint32_t* word);
};
struct Result {std::uint32_t calls,last_operation,applied,clamped;};
enum class Status : std::int32_t {complete,invalid_argument,service_unavailable,service_failed};

// Complete original260B _SetLevel callback caller with named dependency bodies.
// Empty or initial non-number(type!=3) arguments silently complete with no effects.
// Application is read after first get_number and retained; manager+2c is reread
// on each GetInt call. Compare shifted first max as signed32; upper branch uses
// a fresh second max, otherwise obtains/converts argument0 again. Store raw base
// Level word before RecalcProperties(true), RegenHP(-1), then RegenMP(-1).
Status set_level(Character*,const Arguments*,const Globals*,const Services*,Result*);

// One owning thread retains Character/properties/Arguments/Globals/backend and
// old Application backing through return. Entry identities are stable and services
// are captured once; changing the caller service record does not replace the
// retained backend. Scalar arguments, base Level, globals/app selection and the
// retained application's manager may mutate; original later reads/stores stay
// fresh. Control storage must stay live/disjoint; do not overwrite result or
// reenter same Character/output. Independent owners may nest. Missing/error/
// throwing providers stop at their phase, retaining prior source/provider effects
// without rollback, regeneration or cleanup. Null/misaligned/aliased projections
// reject; Application validity is checked only when the source reaches its read.
} // namespace dh2::character_script_set_level
