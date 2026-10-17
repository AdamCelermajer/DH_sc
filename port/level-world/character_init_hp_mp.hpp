#pragma once
#include <cstdint>
namespace dh2::character_init_hp_mp {
struct Owner {std::uintptr_t character;};
struct Request {std::uintptr_t character;std::uint32_t raw_amount;};
enum class Operation : std::uint32_t {hp,mp};
struct Services {
    void* context;
    // Zero is successful source regeneration; nonzero is a separate port
    // error, not the unspecified r0 left by the original void callee.
    std::int32_t (*regen_hp)(void*,const Request*);
    std::int32_t (*regen_mp)(void*,const Request*);
};
struct Result {
    std::uintptr_t captured_character;
    std::uint32_t calls,last_operation,hp_completed,mp_completed;
};
enum class Status : std::int32_t {complete,invalid_argument,service_unavailable,service_failed};
// Complete original Character::_InitHpMp32B caller: capture this, mandatory
// RegenHP(-1), then mandatory tail RegenMP(-1) using the same captured identity.
// No stat write, clamp, debug query, class recalculation, SetLevel, skill stage,
// or final/post initialization is added here. Regeneration providers execute
// the real reconstructed callers and effects; successful no-ops are invalid.
Status execute(const Owner*,const Services*,Result*);
// One owning thread retains controls/context and captured Character/backing
// through synchronous return. Controls are aligned/disjoint. Owner.character
// may change through HP; retired original backing must remain live for MP.
// Bindings and context are captured once. No same Character/output reentry or
// destruction; independent owners may nest. Errors/throws preserve prior HP/MP
// effects without rollback, added MP call, destructor or other cleanup. Missing
// providers fail only when reached. Calls counts entered callbacks, while
// last_operation identifies the reached phase even if its provider is missing.
} // namespace dh2::character_init_hp_mp
