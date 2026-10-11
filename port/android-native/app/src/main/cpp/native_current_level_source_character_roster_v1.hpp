#pragma once

#include "native_character_list.hpp"
#include "../../../../../../port/level-world/object_manager_runtime_owner_v1.hpp"
#include "../../../../../../port/level-world/world.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <unordered_set>
#include <vector>

namespace dh2::native::current_level_source_character_roster_v1 {

using CharacterList = dh2::character::aggro::object_manager_list::Owner;
using ObjectManager = dh2::object_manager_runtime_owner_v1::Owner;

// One row from the source Module/MGP Character census, joined to the same
// Level's ObjectManager handle ledger. `source_row_eligibility_resolved` is
// true only when the selected Level importer/runtime owner proved whether this
// row reached ObjectManager::Add. An unresolved conditional, scripted, or
// duplicate-name path prevents a completeness claim.
struct CensusRow {
    std::uint32_t module_index{};
    std::uint32_t source_record{};
    std::int32_t source_handle{-1};
    std::int32_t source_module_id{-1};
    const char* source_name{};
    std::uintptr_t character_identity{};
    bool duplicate_name{};
    bool source_row_eligibility_resolved{};
    bool should_be_in_character_list{};
};

struct Proof {
    std::size_t census_rows{};
    std::size_t source_rows_expected{};
    std::size_t dynamic_rows_expected{};
    std::size_t character_list_rows{};
    std::uint64_t character_list_generation{};
};

// Resolve the retained authored SWAMP MGP Character census against the
// already-active Level ObjectManager. This is deliberately level-specific:
// every selected SWAMP Character row has spawn_prob 100 (or inherits the
// source default 100), so Character::InitPost cannot randomly remove a row.
// Conditions and auto_spawn do not remove initial ObjectManager membership.
// Duplicate names remain census entries, but source ObjectManager::Add keeps
// the first object's one handle/identity and destroys later allocations.
inline bool build_swamp_census_rows(
    const std::vector<dh2::world::SourceCharacterCensusV1>& source_rows,
    const dh2::world::source_handle_ledger_v1::Ledger& source_ledger,
    const ObjectManager& object_manager, std::vector<CensusRow>& output,
    std::string& error) {
    output.clear();
    if (source_rows.empty()) {
        error = "Retained SWAMP MGP Character census is unavailable";
        return false;
    }
    try { output.reserve(source_rows.size()); }
    catch (...) {
        error = "SWAMP Character census projection allocation failed";
        return false;
    }
    for (const auto& source : source_rows) {
        if (source.source_handle < 0 || source.name.empty() ||
            (!source.spawn_probability.empty() &&
             source.spawn_probability != "100")) {
            output.clear();
            error = "SWAMP Character source row has unresolved handle, name, or spawn probability";
            return false;
        }
        const dh2::world::source_handle_ledger_v1::Occurrence* occurrence = nullptr;
        for (const auto& candidate : source_ledger.occurrences) {
            const auto& provenance = candidate.source;
            if (provenance.kind != dh2::world::RecordKind::mgp ||
                provenance.module_index != source.module_index ||
                provenance.record_index != source.source_record) continue;
            if (occurrence) {
                output.clear();
                error = "SWAMP Character source tuple maps to multiple handle-ledger occurrences";
                return false;
            }
            occurrence = &candidate;
        }
        if (!occurrence || occurrence->handle != source.source_handle ||
            occurrence->duplicate_name != source.duplicate_name ||
            occurrence->source.name != source.name ||
            occurrence->source.gametype != "Character") {
            output.clear();
            error = "SWAMP Character census row differs from its exact MGP handle-ledger occurrence";
            return false;
        }
        const dh2::world::source_handle_ledger_v1::Entry* retained = nullptr;
        for (const auto& candidate : source_ledger.entries) {
            if (candidate.handle != occurrence->handle) continue;
            if (retained) {
                output.clear();
                error = "SWAMP source handle is duplicated in the retained ObjectManager ledger";
                return false;
            }
            retained = &candidate;
        }
        if (!retained || retained->retained_source.name != source.name) {
            output.clear();
            error = "SWAMP Character row does not resolve to its first retained ObjectManager name owner";
            return false;
        }
        CensusRow row{};
        row.module_index = source.module_index;
        row.source_record = source.source_record;
        row.source_handle = source.source_handle;
        row.source_name = source.name.c_str();
        row.duplicate_name = source.duplicate_name;
        row.source_row_eligibility_resolved = true;
        row.should_be_in_character_list = !source.duplicate_name;
        const auto* object = object_manager.find_by_source_handle(
            source.source_handle);
        if (!object || object->source_handle != source.source_handle ||
            object->name != retained->retained_source.name ||
            object->module_id_64 != static_cast<std::int32_t>(
                retained->retained_source.module_index)) {
            output.clear();
            error = "SWAMP MGP Character handle differs from the exact retained ObjectManager winner";
            return false;
        }
        row.source_module_id = object->module_id_64;
        if (!source.duplicate_name) {
            if (!object || !object->identity ||
                row.source_module_id !=
                    static_cast<std::int32_t>(source.module_index)) {
                output.clear();
                error = "Eligible SWAMP MGP Character is missing its exact canonical ObjectManager owner";
                return false;
            }
            row.character_identity = object->identity;
        }
        try { output.push_back(row); }
        catch (...) {
            output.clear();
            error = "SWAMP Character census projection allocation failed";
            return false;
        }
    }
    error.clear();
    return true;
}

// Compare the whole retained source Module/MGP Character census with the
// actual current ObjectManager+0x60 CharacterList. Imported rows are keyed by
// (module, source record, ObjectManager source handle); renderer order and
// matching counts alone are never treated as proof. Dynamic owner identities
// must be supplied explicitly for characters such as the current Player or a
// factory-created Faery. Any unclassified source row fails closed.
inline bool prove(
    const CharacterList& characters, const ObjectManager& object_manager,
    const CensusRow* census, std::size_t census_count,
    const std::uintptr_t* dynamic_identities, std::size_t dynamic_count,
    Proof& output, std::string& error) {
    output = {};
    if ((census_count && !census) || (dynamic_count && !dynamic_identities) ||
        !dh2::character::aggro::object_manager_list::validate(&characters)) {
        error = "Source Character census or current CharacterList is unavailable";
        return false;
    }
    constexpr auto max_population = static_cast<std::size_t>(
        std::numeric_limits<std::int32_t>::max());
    if (census_count > max_population ||
        dynamic_count > max_population - census_count) {
        error = "Source Character census exceeds the bounded current-Level population";
        return false;
    }

    std::unordered_set<std::uintptr_t> expected;
    try { expected.reserve(census_count + dynamic_count); }
    catch (...) {
        error = "Source Character census identity set allocation failed";
        return false;
    }
    auto list_occurrences = [&](std::uintptr_t identity,
                                std::size_t& occurrences) -> bool {
        occurrences = 0;
        std::size_t visited = 0;
        auto* node = characters.sentinel.next;
        while (node != &characters.sentinel) {
            if (!node || visited >= characters.character_count || !node->character)
                return false;
            const auto* character = static_cast<const
                dh2::character::aggro_search::Character*>(node->character);
            if (!character->identity || !character->object ||
                character->object->identity != character->identity)
                return false;
            if (character->identity == identity) ++occurrences;
            ++visited;
            node = node->next;
        }
        return visited == characters.character_count;
    };

    std::size_t source_expected = 0;
    for (std::size_t i = 0; i < census_count; ++i) {
        const auto& row = census[i];
        if (row.source_handle < 0 || row.source_module_id < -1 ||
            !row.source_name || !row.source_row_eligibility_resolved ||
            (row.duplicate_name && row.should_be_in_character_list)) {
            error = "Source Module Character row has unresolved add/duplicate eligibility";
            return false;
        }
        for (std::size_t j = 0; j < i; ++j) {
            if (census[j].module_index == row.module_index &&
                census[j].source_record == row.source_record) {
                error = "Source Module Character census repeats a source row";
                return false;
            }
        }
        if (row.duplicate_name) {
            const auto* winner = object_manager.find_by_source_handle(
                row.source_handle);
            if (!winner || winner->name != row.source_name ||
                winner->module_id_64 != row.source_module_id) {
                error = "Duplicate-name source row does not resolve to its retained ObjectManager winner";
                return false;
            }
        }
        const auto* object = object_manager.find_by_source_handle(row.source_handle);
        if (row.should_be_in_character_list) {
            if (!row.character_identity || !object ||
                object->identity != row.character_identity ||
                object->source_handle != row.source_handle ||
                object->module_id_64 != row.source_module_id ||
                object->name != row.source_name) {
                error = "Eligible source Character row does not resolve to its canonical ObjectManager object";
                return false;
            }
            std::size_t occurrences = 0;
            if (!list_occurrences(row.character_identity, occurrences) ||
                occurrences != 1) {
                error = "Eligible source Character row is absent or duplicated in CharacterList";
                return false;
            }
            try {
                if (!expected.insert(row.character_identity).second) {
                    error = "Source census rows resolve to a duplicate Character identity";
                    return false;
                }
            } catch (...) {
                error = "Source Character census identity set allocation failed";
                return false;
            }
            ++source_expected;
        } else if (row.character_identity ||
                   (!row.duplicate_name && object &&
                    object->name == row.source_name &&
                    object->module_id_64 == row.source_module_id)) {
            error = "Source Character row is present despite its resolved non-add status";
            return false;
        }
    }

    for (std::size_t i = 0; i < dynamic_count; ++i) {
        const auto identity = dynamic_identities[i];
        const auto* object = identity ? object_manager.find_by_identity(identity) : nullptr;
        std::size_t occurrences = 0;
        if (!identity || !object || object->identity != identity ||
            !list_occurrences(identity, occurrences) || occurrences != 1) {
            error = "Dynamic Character owner is absent or duplicated in the active Level";
            return false;
        }
        try {
            if (!expected.insert(identity).second) {
                error = "Dynamic Character duplicates a source census identity";
                return false;
            }
        } catch (...) {
            error = "Source Character census identity set allocation failed";
            return false;
        }
    }

    std::size_t visited = 0;
    auto* node = characters.sentinel.next;
    while (node != &characters.sentinel) {
        if (!node || visited >= characters.character_count || !node->character) {
            error = "Current CharacterList contains malformed links or a stale entry";
            return false;
        }
        const auto* character = static_cast<const
            dh2::character::aggro_search::Character*>(node->character);
        if (!expected.count(character->identity) ||
            !object_manager.find_by_identity(character->identity)) {
            error = "Current CharacterList contains a row outside retained source owners";
            return false;
        }
        ++visited;
        node = node->next;
    }
    if (visited != characters.character_count ||
        expected.size() != characters.character_count) {
        error = "Current CharacterList does not equal the source census and dynamic owner set";
        return false;
    }
    output.census_rows = census_count;
    output.source_rows_expected = source_expected;
    output.dynamic_rows_expected = dynamic_count;
    output.character_list_rows = characters.character_count;
    output.character_list_generation = characters.mutation_generation;
    error.clear();
    return true;
}

} // namespace dh2::native::current_level_source_character_roster_v1
