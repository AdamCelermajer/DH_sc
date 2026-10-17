#pragma once

#include <cstdint>

namespace dh2::character_ai_classification {

// Logical native views of the cached Character +ff8/+ffc words, +44 name
// pointer and +1449 dead byte. No ARM32 object overlay or identity narrowing.
struct State {
    std::uintptr_t character;
    std::int32_t ai_id, faction_id;
    const char* name;
    std::uint8_t dead;
};
struct AiRow { std::uint32_t flags; std::int32_t type; };
struct AiTable { const AiRow* rows; std::uint32_t capacity; };
enum class Operation : std::uint32_t { ai_count, faction_count, ai_table, find_player_name };
struct Request { Operation operation; std::uintptr_t character; const char* name; };
struct Response { std::int32_t count; const AiTable* table; const char* match; };
struct Services {
    void* context;
    // Zero success. Counts are signed original globals. ai_table captures the
    // current original backing BEFORE GetCharAIId reads the cached ID/count.
    // find_player_name is actual strstr(captured_name,"PlayerCharacter"); match
    // retains the pointer identity, including null. All captured storage lives
    // through return. These boundaries model reads/imports, not game policies.
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
};
enum class Query : std::uint32_t {
    ai_id, faction_id, ai_row, type, monster, follower, faerie, summoned,
    merchant, invisible_man, cleaner, npc, player, miniboss, boss,
    sitting, dead,
};
struct Result {
    std::uint32_t word, calls, table_captures, type_reads;
    const AiRow* row;
};
enum class Status : std::int32_t {
    complete, invalid_argument, service_unavailable, service_failed,
    invalid_source_fact,
};

// Complete bounded source getters/predicates. NPC performs fresh type reads
// in the original short-circuit order (6, then merchant7, then cleaner8).
// Player type0 tests strstr's returned pointer against the captured name;
// type1 is true and every other type false. Dead returns the raw byte.
// Errors retain effects, without fabricated classifications or rollback.
// Independent sessions may nest; same-state reentry/borrowed destruction and
// mutation of output/services from a callback are outside the contract.
Status query(Query, State*, const Services*, Result*);

}  // namespace dh2::character_ai_classification
