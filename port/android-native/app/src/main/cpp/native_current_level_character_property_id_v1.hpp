#pragma once

#include "native_current_level_character_population_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::native::current_level_character_property_id_v1 {

namespace population = dh2::native::current_level_character_population_v1;
namespace list = dh2::character::aggro::object_manager_list;
namespace aggro = dh2::character::aggro_search;

// Borrow one already prepared current-Level population binding. This adapter
// invokes the retained Character::SafeGetCharPropsId kernel over that same
// Character factory Record/cache; it never creates another lookup cache.
struct Context {
    const population::Bindings* bindings{};
};

inline bool safe_get(void* raw, std::uintptr_t character_identity,
                     std::int32_t& property_id, std::string& error) {
    property_id = -1;
    if (!raw || !character_identity) {
        error = "Current-Level Character property query has no identity/context";
        return false;
    }
    const auto& context = *static_cast<const Context*>(raw);
    if (!context.bindings) {
        error = "Current-Level Character property binding is unavailable";
        return false;
    }
    const auto& b = *context.bindings;
    if (!b.characters || !b.object_manager || !b.event_owner_identity ||
        !b.level_identity ||
        (!b.safe_get_bindings && !b.safe_get_character_property_id) ||
        !list::validate(b.characters) ||
        !b.object_manager->find_by_identity(character_identity)) {
        error = "Current-Level Character SafeGet owners are stale or incomplete";
        return false;
    }
    const auto generation = b.roster_generation;
    if (b.characters->mutation_generation != generation ||
        b.characters->character_count != b.expected_character_count) {
        error = "Current-Level CharacterList changed before SafeGetCharPropsId";
        return false;
    }

    std::int32_t resolved = -1;
    if (b.safe_get_bindings) {
        population::SafeGetBindings safe{};
        if (!b.safe_get_bindings(b.context, b.event_owner_identity,
                b.level_identity, character_identity, safe, error)) {
            if (error.empty()) error = "Character SafeGet bindings are unavailable";
            return false;
        }
        if (safe.character != character_identity || !safe.property_cache) {
            error = "Character SafeGet bindings do not borrow its canonical cache";
            return false;
        }
        dh2::character_saved_class_v1::Runtime runtime(safe);
        dh2::character_saved_class_v1::Result result{};
        if (runtime.resolve(&result, error) !=
            dh2::character_saved_class_v1::Status::complete) {
            if (error.empty()) error = "Character::SafeGetCharPropsId failed";
            return false;
        }
        resolved = result.value;
    } else if (!b.safe_get_character_property_id(b.context,
                   b.event_owner_identity, b.level_identity,
                   character_identity, &resolved, error)) {
        if (error.empty()) error = "Character::SafeGetCharPropsId failed";
        return false;
    }

    if (b.characters->mutation_generation != generation ||
        b.characters->character_count != b.expected_character_count ||
        !list::validate(b.characters) ||
        !b.object_manager->find_by_identity(character_identity)) {
        error = "Current-Level Character owners changed during SafeGetCharPropsId";
        return false;
    }
    if (resolved < -1 || resolved >= static_cast<std::int32_t>(b.character_table_size)) {
        error = "Character::SafeGetCharPropsId returned an invalid CharacterTable row";
        return false;
    }
    property_id = resolved;
    error.clear();
    return true;
}

} // namespace dh2::native::current_level_character_property_id_v1
