#include "character_ai_relations.hpp"

#include <cstddef>
#include <limits>

namespace dh2::character_ai_relations {
namespace {
struct Range { std::uintptr_t begin, end; };
bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) {
    const auto at = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || at % alignment || at > UINTPTR_MAX - size) return false;
    out = {at, at + size}; return true;
}
bool overlap(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
}  // namespace

Status query(Relation relation, State* state, std::uintptr_t candidate,
             const Services* services, Result* result) {
    Range objects[3];
    if (static_cast<unsigned>(relation) > 2 ||
        !range(state, sizeof(*state), alignof(State), objects[0]) ||
        !range(services, sizeof(*services), alignof(Services), objects[1]) ||
        !range(result, sizeof(*result), alignof(Result), objects[2])) return Status::invalid_argument;
    for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlap(objects[i], objects[j])) return Status::invalid_argument;
    if (!state->ai) return Status::invalid_argument;
    const Services bound = *services;
    *result = {};
    auto done = [&](bool value) { result->value = value; return Status::complete; };
    auto invoke = [&](auto provider, std::uint32_t& calls, auto... arguments) {
        if (!provider) return Status::service_unavailable;
        ++calls;
        try {
            return provider(bound.context, state, arguments...) ? Status::service_failed : Status::complete;
        } catch (...) { return Status::service_failed; }
    };
    auto faction = [&](std::uintptr_t character, std::int32_t& id) {
        if (!character) return Status::invalid_source_fact;
        return invoke(bound.get_faction_id, result->faction_getter_calls, character, &id);
    };
    auto validate_faction = [&](std::uintptr_t fixed_character, bool owner) {
        std::int32_t id = 0;
        auto status = faction(owner ? state->owner : fixed_character, id);
        if (status != Status::complete) return status;
        if (id < 0) return Status::invalid_source_fact;
        status = faction(owner ? state->owner : fixed_character, id);
        if (status != Status::complete) return status;
        std::int32_t count = 0;
        status = invoke(bound.faction_count, result->faction_count_reads, &count);
        if (status != Status::complete) return status;
        // Source assertion bodies for invalid IDs are excluded. No fallback or
        // clamping is invented here; the actual getter owns fallback10 already.
        return id >= 0 && id < count ? Status::complete : Status::invalid_source_fact;
    };
    if (!candidate) candidate = state->target_40;
    result->candidate = candidate;
    if (!candidate) return done(relation == Relation::neutral);
    std::uintptr_t character = 0;
    auto status = invoke(bound.resolve_object_handle, result->resolution_calls, candidate, &character);
    if (status != Status::complete) return status;
    result->resolved_object = character;
    std::uint32_t value = 0;
    if (character) {
        status = invoke(bound.read_object_word_f4, result->word_f4_reads, character, &value);
        if (status != Status::complete) return status;
    }
    if (!character || value) {
        if (relation != Relation::enemy) return done(relation == Relation::neutral);
        const auto owner = state->owner;
        if (!owner) return Status::invalid_source_fact;
        value = 0;
        status = invoke(bound.is_interactive, result->interactive_queries, candidate, owner, &value);
        if (status != Status::complete) return status;
        if (!value) return done(false);
        const auto current_owner = state->owner;
        if (!current_owner) return Status::invalid_source_fact;
        std::int32_t type = 0;
        status = invoke(bound.interaction_type, result->interaction_type_queries,
                        candidate, current_owner, &type);
        if (status != Status::complete) return status;
        return done(type == 8);
    }
    status = validate_faction(character, false);
    if (status != Status::complete) return status;
    status = validate_faction(0, true);
    if (status != Status::complete) return status;
    if (relation == Relation::enemy) {
        const auto owner = state->owner;
        if (!owner) return Status::invalid_source_fact;
        value = 0;
        status = invoke(bound.is_player, result->player_queries, owner, &value);
        if (status != Status::complete) return status;
        if (value) {
            value = 0;
            status = invoke(bound.is_player, result->player_queries, character, &value);
            if (status != Status::complete) return status;
            if (value) return done(false);
        }
    }
    // Source captures owner BEFORE loading the global table, then executes
    // that captured owner's third getter, followed by the fixed target's third.
    const auto owner = state->owner;
    if (!owner) return Status::invalid_source_fact;
    const FactionTable* table = nullptr;
    status = invoke(bound.capture_faction_table, result->table_captures, &table);
    if (status != Status::complete) return status;
    Range table_range{};
    if (!range(table, sizeof(*table), alignof(FactionTable), table_range)) return Status::invalid_source_fact;
    for (const auto& object : objects) if (overlap(object, table_range)) return Status::invalid_source_fact;
    // Table backing identity is captured before either getter; row metadata is
    // addressed after the owner's getter, while count/entries are read after
    // the target's getter, as the original instructions do.
    const auto* rows = table->rows;
    const auto row_capacity = table->row_capacity;
    Range rows_range{};
    if (!row_capacity || row_capacity > 4096 ||
        !range(rows, sizeof(*rows) * row_capacity, alignof(FactionRow), rows_range))
        return Status::invalid_source_fact;
    for (const auto& object : objects) if (overlap(object, rows_range)) return Status::invalid_source_fact;
    std::int32_t owner_id = 0, target_id = 0;
    status = faction(owner, owner_id);
    if (status != Status::complete) return status;
    if (owner_id < 0 || static_cast<std::uint32_t>(owner_id) >= row_capacity) return Status::invalid_source_fact;
    const auto* row = rows + owner_id;
    status = faction(character, target_id);
    if (status != Status::complete) return status;
    const auto count = row->count;
    const auto* entries = row->entries;
    if (count > row->capacity || row->capacity > 4096) return Status::invalid_source_fact;
    result->faction_row_count = count;
    if (!count) return done(relation == Relation::neutral);
    Range entries_range{};
    if (!range(entries, sizeof(*entries) * count, alignof(dh2::data::AiFactionEntry), entries_range))
        return Status::invalid_source_fact;
    for (const auto& object : objects) if (overlap(object, entries_range)) return Status::invalid_source_fact;
    if (relation == Relation::enemy)
        return done(dh2_ai_enemy(entries, count, target_id, 0, 0) != 0);
    for (std::uint32_t i = 0; i < count; ++i)
        if (entries[i].id == target_id)
            return done(relation == Relation::friend_ ? entries[i].value > 0 : entries[i].value == 0);
    return done(relation == Relation::neutral);
}

}  // namespace dh2::character_ai_relations
