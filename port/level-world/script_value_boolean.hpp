#pragma once
#include <cstdint>
namespace dh2::script_value_boolean {
// Logical source Value fields, never an ARM/native object overlay:
// type+4, binary32 bits+8, object identity+6c, retained C-string pointer+20.
struct Value {
    std::uint32_t type,number_word;
    std::uintptr_t object_identity;
    const char* string;
};
enum class Operation : std::uint32_t {new_state,push_string,to_boolean,close_state};
struct Request {
    Operation operation;
    std::uintptr_t lua;
    const char* text;
    std::int32_t index;
};
struct Response {std::uintptr_t lua;std::uint32_t raw_boolean;};
struct Services {
    void* context;
    // Each operation must execute the real named Lua dependency. Zero means
    // port success; nonzero is a separate error, not a raw void Lua return.
    // new_state returns its real Lua identity; to_boolean returns its raw word.
    // No successful allocation/push/close no-op or generic truthiness shortcut.
    std::int32_t (*invoke)(void*,const Value*,const Request*,Response*);
};
enum class Phase : std::uint32_t {not_started,new_state,push_string,to_boolean,close_state,complete};
struct Result {
    std::uintptr_t lua;
    Phase phase;
    std::uint32_t captured_type,value,raw_lua_boolean,service_calls,closed;
};
enum class Status : std::int32_t {complete,invalid_argument,service_unavailable,service_failed,invalid_source_fact};
// Complete source Value::getBool172B. Capture type once. Types1/3 compare the
// binary32 word to zero exactly; sign-zero false and all other words true.
// Types2/7 read nonnull object identity. Type4 creates temporary Lua state,
// rereads Value.string AFTER allocation, pushes it, queries bool(-1), captures
// canonical result BEFORE close, then closes. Nil and unknown tags are false.
// Services can be null for nonstrings; string operations are all mandatory.
Status execute(const Value*,const Services*,Result*);
// One owning thread retains controls/string bytes and callback context through
// synchronous return. Type is captured; an allocation callback may replace the
// live string, whose old/new backing remains owned. Binding/context are copied
// once and created Lua identity is captured. No same-Value/output reentry or
// destruction during callbacks; independent resources may nest. Controls must
// be aligned/disjoint. Invalid port inputs leave output unchanged. Port failure
// preserves prior effects without added close/rollback; created Lua resources
// remain provider-owned until outer teardown. Null newstate is an explicit
// invalid boundary before the original would dereference a null Lua state.
} // namespace dh2::script_value_boolean
