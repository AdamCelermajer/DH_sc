#pragma once
#include <array>
#include <cstdint>

namespace dh2::character_ai_initialization {

// Native ownership projections, not a packed ARM32 object. The original
// constructor initializes three empty tree headers and one circular list.
struct TreeHeader {
    std::uint8_t color;
    std::uintptr_t parent, left, right;
    std::uint32_t count;
};
struct ListHeader { std::uintptr_t next, previous; };
struct State {
    std::uintptr_t identity, dispatch_table, owner_04;
    std::uint32_t word_08, word_0c, word_10, word_14;
    std::uint8_t paused_18;
    std::uintptr_t active_ais_1c, alternate_ais_20;
    std::uint8_t byte_24;
    std::uintptr_t pointer_28;
    std::uint8_t byte_2c;
    std::uintptr_t script_name_30;
    // Native-width semantic projection of the optional Character+0x3fc /
    // CharAI+0x34 GroupInfo pointer. The source constructor initializes it
    // null; a later AddToGroup-equivalent owner may retain a stable pointee.
    // Preserve the original constructor-initialized 32-bit source word for
    // layout comparison. The native GroupInfo pointer is retained separately
    // below because a host pointer cannot fit in this ARM32 slot.
    std::uint32_t word_34, word_38;
    std::uintptr_t requested_target_3c, target_40, last_target_44;
    // +48 is deliberately not initialized by this original constructor.
    std::uint8_t alive_48, sight_49, byte_4a, byte_4b, sticky_4c, targetable_4d;
    std::uintptr_t master_50;
    std::uint8_t byte_54, byte_55;
    std::uint32_t word_58;
    TreeHeader tree_5c, tree_7c, tree_94;
    ListHeader list_ac;
    std::array<std::uintptr_t, 6> words_b4_to_c8;
    std::uint32_t word_cc;
    std::uint8_t byte_d0, byte_d1;
    std::uintptr_t group_info_identity{};
};
struct Services {
    void* context;
    // Source deque push_back(this) is the final constructor action. A native
    // queue implementation must append this stable identity exactly once in
    // Character construction order; it must not fill owner/active AIS here.
    // Zero is success. State and retired borrowed storage live through return.
    std::int32_t (*append_queue)(void*, State*, std::uintptr_t ai);
};
struct Result { std::uint32_t queue_calls, queued; };
enum class Status : std::int32_t {
    complete, invalid_argument, service_unavailable, service_failed,
};

// Complete scalar/container initialization and queue-registration boundary of
// both original 352-byte CharAI constructors. Owner+4 and alive+48 are retained;
// association with Character, choosing/initializing AIS and deque allocation
// belong to separate original callers. The dispatch table is the native
// CharAI class's table identity, not an address into the original binary.
// Missing/failed registration preserves the initialization already performed.
Status construct(State*, std::uintptr_t dispatch_table, const Services*, Result*);

} // namespace dh2::character_ai_initialization
