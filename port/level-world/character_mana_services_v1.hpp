#pragma once

#include "debug_switches_runtime.hpp"
#include "../adam-script-runtime/script_runtime.h"
#include "../game-data/properties.hpp"
#include <cstdint>

namespace dh2::character_mana_services_v1 {

// Borrowed projections of the existing Character, CharProperties and global
// singletons. These are not ARM32 object overlays and own no parallel state.
struct State {
    std::uintptr_t character;
    // Opaque boolean byte read at Character+0x14f0 by UseMana. Its source
    // field producer/semantic name has not been recovered.
    std::uint8_t* mana_exempt_14f0;
    data::PropertyView* properties;
};

struct Globals {
    // The source reads Singleton<Application>::s_inst at the SavedOption call.
    // This pointer is therefore dereferenced freshly at that phase.
    std::uintptr_t* application_singleton;
    debug_switches::Globals* debug_globals;
    const debug_switches::Services* debug_services;
};

enum class Operation : std::uint8_t {
    get_online,
    is_remotely_updated,
    application_is_saved_option_on
};

struct Request {
    Operation operation;
    std::uintptr_t subject;
    const char* text;
};

struct Reply {
    std::uintptr_t identity;
    std::uint32_t word;
};

struct Services {
    void* context;
    // Zero means the required source provider completed. get_online returns
    // its current singleton in Reply.identity and the byte at +5 in Reply.word.
    // The other calls return their source integer/boolean in Reply.word.
    std::int32_t (*invoke)(void*, const Request*, Reply*);
};

struct Result {
    std::uint32_t service_calls;
    Operation last_service;
    std::uint32_t online_queries;
    std::uint32_t remote_queries;
    std::uint32_t application_is_saved_option_on_queries;
    std::uint32_t debug_loads;
    std::uint32_t debug_queries;
    std::uint32_t has_mana_calls;
    std::uintptr_t last_online_identity;
    std::uint32_t last_online_byte;
    std::uint32_t last_remote_word;
    std::uint32_t last_application_is_saved_option_on_word;
    std::int32_t amount;
    std::int32_t mana_before;
    std::uint8_t mana_exempt_14f0_byte;
    std::uint8_t decision;
    std::uint8_t mana_added;
    std::int32_t debug_status;
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed,
    invalid_source_result,
    invalid_property_view,
    unsupported_assertion_domain,
    reentrant_owner
};

// Complete integer helpers for the supported nonnegative amount domain.
// HasMana's online/remote fast path and raw cached property 41 read are kept
// separate from UseMana's GOD_MANA, opaque +0x14f0 byte, nested HasMana, MP debit,
// and trailing statistics-debug lookup sequence.
Status has_mana(const State*, std::int32_t amount, const Services*, bool* result,
                Result* details);
Status use_mana(const State*, const Globals*, std::int32_t amount,
                const Services*, bool* result, Result* details);

// Source native callback bindings. A malformed signature (including a
// non-number argument) is the original silent zero-result no-op. A reached
// missing provider or unsupported amount returns the script runtime's required
// service error; it never fabricates a successful boolean.
struct CallbackContext {
    const State* state;
    const Globals* globals; // ignored by HasMana, required by UseMana
    const Services* services;
};
int has_mana_callback(void*, const dh2_script_value*, std::uint32_t,
                      dh2_script_value*, std::uint32_t, std::uint32_t*,
                      char*, std::size_t);
int use_mana_callback(void*, const dh2_script_value*, std::uint32_t,
                      dh2_script_value*, std::uint32_t, std::uint32_t*,
                      char*, std::size_t);

} // namespace dh2::character_mana_services_v1
