#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::character_update_script_scheduler {

// Logical source facts. They are not ARM32 overlays. Update enters this block
// only after its earlier state!=0 branch at 0x3abf84; that outer branch and
// the separate +0x520 test at 0x3ac2b8 are deliberately outside this unit.
struct Character {
    std::uintptr_t identity;
    std::uintptr_t controller;
    std::uintptr_t ai;
    std::uint32_t state_id;
    // Character embeds CharAI at +0x3c8; +0x3e4 is CharAI's +0x1c active-AIS
    // association word/pointer. Keep the host identity full width.
    std::uintptr_t active_ais_3e4;
    std::uint8_t field_1480;
    std::uint8_t field_3ec;
    std::uint8_t reserved[2];
};

// Source s_concurrentAI is std::map<int, Character*>. This port-facing view
// stores its unique keys in signed ascending order; key collisions replace the
// mapped Character because Update writes the returned node's +0x14 value even
// when _M_insert_unique reports an existing node.
struct Entry {
    std::int32_t key;
    Character* character;
};
struct ConcurrentMap {
    Entry* entries;
    std::uint32_t size;
    std::uint32_t capacity;
    std::size_t entries_extent;
};

enum class Operation : std::uint32_t {
    current_level,
    controller_kill,
    unload_script_process,
    load_n_init_script_process,
    is_monster,
    is_mini_boss,
    is_boss,
    get_online_byte,
    real_time_ms,
};

struct Request {
    Operation operation;
    Character* subject_character;
    std::uintptr_t subject_identity;
    std::uint32_t argument0;
    std::uint32_t argument1;
};
struct Response {
    // current_level: identity is the live Level object and raw is its +0x3c OID.
    // get_online_byte: identity is the source return pointer and raw is byte+5.
    // All other operations use raw as their unnormalized source return word.
    std::uint32_t raw;
    std::uintptr_t identity;
};
struct Services {
    void* context;
    // Byte extent of the projection object at context (zero only for a
    // stateless callback). Secondary allocations referenced by this object
    // remain caller-owned and must not alias the live projections below.
    std::size_t context_extent;
    // Return zero for a completed provider operation. It may mutate the live
    // Character facts. Map structure may only change through this kernel's
    // eviction/insertion or a provider that truly owns the corresponding
    // source mutation; storage and object identities must remain live.
    std::int32_t (*invoke)(void*, Character*, ConcurrentMap*,
                           const Request*, Response*);
};

enum class Decision : std::uint32_t {
    not_started,
    state_12_skip,
    state_2_skip,
    field_1480_skip,
    field_3e4_skip,
    load_failed,
    type_gate_skip,
    online_skip,
    field_3ec_skip,
    inserted,
    timestamp_replaced,
};
struct Result {
    Decision decision;
    std::uint32_t service_calls;
    std::uint32_t eviction_threshold;
    std::uint32_t eviction_attempted;
    std::uint32_t map_size_before;
    std::uint32_t map_size_after;
    std::int32_t evicted_key;
    std::uintptr_t evicted_character;
    std::uint32_t timestamp_word;
    std::int32_t timestamp_key;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    service_unavailable = 2,
    service_failed = 3,
    invalid_source_fact = 4,
    map_storage_exhausted = 5,
    reentrant_call = 6,
};

// Executes the bounded Character::Update block beginning at 0x3ac34c and the
// reached OID-29 cap tail at 0x3aca48. Caller must already have taken the
// enclosing state!=0 branch. This does not implement Character::Update as a
// whole, the prior +0x520 branch, map allocator internals, or the outer
// Character::UnLoadScriptProcess body.
//
// At OID 29 the source evicts when map.size()>24 (CMP #0x18 at 0x3aca54);
// all other OIDs evict when map.size()>8 (CMP #8 at 0x3ac3a8). The map key is
// the raw real-time word interpreted as signed int32. Calls over the same
// Character or map are rejected as reentrant; callers serialize other
// overlapping work. All borrowed Characters, map entries, services,
// context projection, and provider backings must remain live for the
// synchronous call. Port errors
// preserve source effects already performed and do not roll them back.
Status run(Character*, ConcurrentMap*, const Services*, Result*);

}  // namespace dh2::character_update_script_scheduler
