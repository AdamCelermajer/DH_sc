#pragma once
#include <cstdint>

namespace dh2::ais_state_callbacks {

// Borrowed source-field projections, not overlays of the original 32-bit
// CharAIScript::State/std::string or CharAIScript objects. The original names
// are the string data pointers at +14/+2c/+44/+5c. Their storage and the table
// must remain alive throughout the synchronous LuaScript::Call boundary.
struct Table {
    std::uintptr_t identity;
    const char* update;
    const char* conditions;
    const char* init;
    const char* post;
};
struct State { std::uintptr_t ais; const Table* active; };
enum class Callback : std::uint32_t { update, conditions, init, post };
struct Request {
    std::uintptr_t ais;
    std::uintptr_t table;
    const char* name;
    Callback callback;
    std::uint32_t source_name_offset;
};
struct Services {
    void* context;
    std::int32_t (*call)(void*, State*, const Request*);
};
struct Result {
    std::uint32_t called;
    std::uint32_t source_name_offset;
    std::uintptr_t table;
    const char* name;
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument, service_unavailable, service_failed,
};

// Exact four leaf wrappers: capture the current +b4 table; null returns
// without Lua work; otherwise capture its selected name and tail-call Call.
// A later invocation rereads State::active. Null/empty names are passed to
// Call unchanged, since the original wrapper does not validate their value.
// Callback effects survive failure; no rollback, alias resolution, table
// registration, script invocation policy or state transition is invented.
Status invoke(State*, Callback, const Services*, Result*);

static_assert(sizeof(Table) == 40 && sizeof(State) == 16);
static_assert(sizeof(Request) == 32 && sizeof(Services) == 16 && sizeof(Result) == 24);
}
