#pragma once

#include <cstdint>

namespace dh2::ais_external_update {

// Borrowed projection of the AIS fields read by AISExternal::OnUpdate and
// AISDefault::OnUpdate. This is not an overlay of either original object.
struct State {
    std::uintptr_t ais;
    std::uintptr_t owner;          // fresh AIS+0x98 owner projection
    std::uint32_t flags_b8;        // source AIS+0xb8 bits, read after default update
    std::uint32_t counter_bc;      // source AIS+0xbc unsigned counter
};

enum class Operation : std::uint32_t {
    pause_character_ai,            // CharAI::AI_PauseUpdate(1000)
    stop_character_controller,     // v2Controller::Cmd_Stop()
    call_ais_on_update,            // LuaScript::Call("OnUpdate")
    call_state_update,              // CharAIScript::CallStateUpdate()
    call_state_conditions,          // CharAIScript::CallStateConditions()
};

enum class ScriptCallback : std::uint32_t {
    none = 0,
    on_update,
    state_update,
    state_conditions,
};

struct Request {
    Operation operation;
    ScriptCallback callback;
    std::uintptr_t ais;
    std::uintptr_t owner;
    std::uint32_t argument;
    std::uint32_t reserved;
};

struct Services {
    void* context;
    // Zero means the synchronous source dependency was completed. A failure
    // is a port boundary: original methods return void and continue normally.
    // `call_state_update` and `call_state_conditions` providers must implement
    // their own live AIS+0xb4 null check and, when present, read the current
    // state's +0x14/+0x2c callback immediately before LuaScript::Call.
    std::int32_t (*invoke)(void*, State*, const Request*);
};

enum class Phase : std::uint32_t {
    not_started = 0,
    pause_update = 1,
    controller_stop = 2,
    script_on_update = 3,
    state_update = 4,
    state_conditions = 5,
    complete = 6,
};

struct Result {
    Phase phase;
    std::uint32_t service_calls;
    std::uint32_t default_pause_due;
    std::uint32_t script_on_update_called;
    std::uint32_t reserved;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    service_unavailable = 2,
    service_failed = 3,
    invalid_source_fact = 4,
};

// Reconstructs the AISExternal::OnUpdate caller and its AISDefault::OnUpdate
// prefix. It compares the unsigned counter against 199, clears it before
// AI_PauseUpdate(1000), then calls Cmd_Stop. After that default method returns,
// it rereads flags bit 0, conditionally calls script OnUpdate, then always
// crosses the two CharAIScript state callback boundaries in source order.
// These last providers perform the original independent +0xb4 checks.
// Callback effects persist on port failure; no rollback or error cleanup is
// added. Owner replacement is observed by the later stop call. Single-threaded
// same-State reentry and destruction of borrowed state during a callback are
// outside the contract.
Status update(State*, const Services*, Result*);

static_assert(sizeof(State) == 24);
static_assert(sizeof(Request) == 32);
static_assert(sizeof(Services) == 16);
static_assert(sizeof(Result) == 20);

}  // namespace dh2::ais_external_update
