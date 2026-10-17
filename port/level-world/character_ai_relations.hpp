#pragma once

#include "../game-data/ai.hpp"

namespace dh2::character_ai_relations {

enum class Relation : std::uint32_t { enemy, friend_, neutral };
struct State {
    std::uintptr_t ai;
    std::uintptr_t owner;
    std::uintptr_t target_40;
};

// Borrowed views, not overlays of the original 12-byte ARM32 row/entry.
// count/entries remain live until their later source read after the final
// target faction getter. capacity/row_capacity are port memory bounds.
struct FactionRow {
    const dh2::data::AiFactionEntry* entries;
    std::uint32_t count;
    std::uint32_t capacity;
};
struct FactionTable {
    const FactionRow* rows;
    std::uint32_t row_capacity;
};
struct Services {
    void* context;
    // All providers return zero on success; source values are separate outputs.
    // Source const ObjectBase::GetHandle followed by GetObject(false), not an
    // RTTI cast. A null handle result takes the generic/default branch; word
    // +0xf4 ==0 on a live resolved object identifies the Character path.
    std::int32_t (*resolve_object_handle)(void*, State*, std::uintptr_t object,
                                         std::uintptr_t* resolved_object);
    std::int32_t (*read_object_word_f4)(void*, State*, std::uintptr_t resolved_object,
                                      std::uint32_t* word);
    // Actual GetCharAIFactionId semantics, including its source fallback10,
    // evaluated freshly on each call. Do not supply a cached raw property.
    std::int32_t (*get_faction_id)(void*, State*, std::uintptr_t character,
                                  std::int32_t* id);
    std::int32_t (*faction_count)(void*, State*, std::int32_t* count);
    std::int32_t (*is_player)(void*, State*, std::uintptr_t character,
                             std::uint32_t* value);
    // Capture the current original global faction table. The caller retains
    // this view and its backing storage until return, including after global
    // table replacement. Later row count/entries reads are still fresh.
    std::int32_t (*capture_faction_table)(void*, State*, const FactionTable**);
    std::int32_t (*is_interactive)(void*, State*, std::uintptr_t object,
                                  std::uintptr_t owner, std::uint32_t* value);
    // Resolves current object virtual +0x90 with fresh owner, not +0x94 radius.
    std::int32_t (*interaction_type)(void*, State*, std::uintptr_t object,
                                    std::uintptr_t owner, std::int32_t* type);
};
struct Result {
    std::uint32_t value;
    std::uint32_t resolution_calls;
    std::uint32_t word_f4_reads;
    std::uint32_t faction_getter_calls;
    std::uint32_t faction_count_reads;
    std::uint32_t player_queries;
    std::uint32_t table_captures;
    std::uint32_t interactive_queries;
    std::uint32_t interaction_type_queries;
    std::uint32_t faction_row_count;
    std::uintptr_t candidate;
    std::uintptr_t resolved_object;
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3, invalid_source_fact = 4,
};

// Three bounded source relation query bodies. Null candidate uses current
// AI+0x40; the candidate/handle result are then fixed, owner is read at each source
// point. Assertion/crash/debug branches for invalid faction IDs are excluded:
// those facts fail explicitly. No float/group/design predicates are invented.
// Enemy faction-row sign lookup reuses dh2_ai_enemy; friend/neutral use their
// actual first-match predicates. Providers must keep all exposed characters,
// table/row/entry views and context live through return. One owning thread;
// independent nested outputs are allowed, but providers must not overwrite this
// Result/service table or invalidate captured views. Errors retain prior effects.
Status query(Relation, State*, std::uintptr_t candidate, const Services*, Result*);

}  // namespace dh2::character_ai_relations
