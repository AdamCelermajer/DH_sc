#pragma once

#include <cstdint>

namespace dh2::ais_external_init_callbacks {

enum class Callback : std::uint32_t { init, post, final };
struct State {std::uintptr_t ais;};
struct Request {std::uintptr_t ais; Callback callback; const char* name;};
struct Services {
    void* context;
    // Mandatory synchronous LuaScript::Call(name) provider. The original
    // callers supply no arguments. Alias resolution and discarded ReturnValues
    // conversion belong to this provider. Zero means completed, never a stub.
    std::int32_t (*call)(void*, State*, const Request*);
};
struct Result {
    std::uintptr_t ais;
    const char* name;
    std::uint32_t calls;
    std::uint32_t default_init_completed;
};
enum class Status : std::int32_t {complete,invalid_argument,service_unavailable,service_failed};

// AISDefault's three initialization bodies are actual 4-byte `bx lr` leaves.
// They perform no reads, writes, virtual calls, or script calls. This native
// projection implements those proven empty bodies; not an unavailable service.
void default_init() noexcept;
void default_post() noexcept;
void default_final() noexcept;

// Complete AISExternal OnInit (36B), OnInitPost (16B), OnInitFinal (16B)
// callers. Only OnInit calls AISDefault::OnInit before LuaScript::Call. Post
// and Final directly tail-call their fixed names without flags/membership
// gates. Captured entry AIS identity survives synchronous provider mutation;
// later invocations read the new identity. Effects survive provider failure.
// Borrowed state/services/context remain alive through return; one owning
// thread, disjoint control storage/output and no same-output reentry.
Status invoke(State*, Callback, const Services*, Result*);

} // namespace dh2::ais_external_init_callbacks
