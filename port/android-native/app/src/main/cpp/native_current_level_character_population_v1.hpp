#pragma once

#include "native_character_list.hpp"
#include "native_quest_owner.hpp"
#include "../../../../../../port/level-world/character_oid_cache_v1.hpp"
#include "../../../../../../port/level-world/character_runtime_factory_v1.hpp"
#include "../../../../../../port/level-world/character_saved_class_v1.hpp"
#include "../../../../../../port/level-world/object_manager_runtime_owner_v1.hpp"
#include "../../../../../../port/level-world/source_level_owner_v1.hpp"

#include <cstdint>
#include <limits>
#include <string>
#include <unordered_set>

namespace dh2::native::current_level_character_population_v1 {

namespace list = dh2::character::aggro::object_manager_list;
namespace aggro = dh2::character::aggro_search;
using ObjectManager = dh2::object_manager_runtime_owner_v1::Owner;
using OidCache = dh2::character_oid_cache_v1::Owner;
using SafeGetBindings = dh2::character_saved_class_v1::Bindings;

struct ImportedCharacterIdentityV1 {
    std::uintptr_t identity{};
    std::int32_t source_handle{-1};
    std::int32_t source_module_id{-1};
};

// The retained source Level import is the expected population for authored
// Characters. Dynamic source Characters (for example, the player and a
// factory-created Faery) must be listed explicitly by their canonical owner.
// This proof never promotes a count-only approximation: every imported and
// dynamic identity must occur once in both the active ObjectManager and its
// CharacterList, and every live CharacterList entry must be in that expected
// identity set.
inline bool prove_imported_character_roster_complete(
    const list::Owner& characters, const ObjectManager& object_manager,
    const ImportedCharacterIdentityV1* imported, std::size_t imported_count,
    const std::uintptr_t* dynamic_identities, std::size_t dynamic_count,
    std::string& error) {
    if ((imported_count && !imported) || (dynamic_count && !dynamic_identities) ||
        imported_count > std::numeric_limits<std::size_t>::max() - dynamic_count ||
        !list::validate(&characters) ||
        characters.character_count != imported_count + dynamic_count) {
        error = "Active CharacterList does not match retained imported Character rows";
        return false;
    }
    const auto expected = [&](std::uintptr_t identity) {
        if (!identity) return false;
        for (std::size_t i = 0; i < imported_count; ++i)
            if (imported[i].identity == identity) return true;
        for (std::size_t i = 0; i < dynamic_count; ++i)
            if (dynamic_identities[i] == identity) return true;
        return false;
    };
    const auto listed_once = [&](std::uintptr_t identity) {
        std::size_t occurrences = 0;
        auto* node = characters.sentinel.next;
        while (node != &characters.sentinel) {
            if (!node || !node->character) return false;
            const auto* character = static_cast<const aggro::Character*>(node->character);
            if (character->identity == identity) ++occurrences;
            node = node->next;
        }
        return occurrences == 1;
    };
    const auto manager_has = [&](std::uintptr_t identity) {
        const auto* object = object_manager.find_by_identity(identity);
        return object && object->identity == identity;
    };
    for (std::size_t i = 0; i < imported_count; ++i) {
        const auto identity = imported[i].identity;
        const auto* object = identity ? object_manager.find_by_identity(identity) : nullptr;
        if (!identity || imported[i].source_handle < 0 || !object ||
            object->identity != identity ||
            object->source_handle != imported[i].source_handle ||
            object->module_id_64 != imported[i].source_module_id ||
            !listed_once(identity)) {
            error = "Imported Character row is absent or duplicated in the active Level owners";
            return false;
        }
        for (std::size_t j = 0; j < i; ++j)
            if (imported[j].identity == identity) {
                error = "Retained source import contains a duplicate Character identity";
                return false;
            }
    }
    for (std::size_t i = 0; i < dynamic_count; ++i) {
        const auto identity = dynamic_identities[i];
        if (!identity || !manager_has(identity) || !listed_once(identity)) {
            error = "Dynamic Character is absent or duplicated in the active Level owners";
            return false;
        }
        for (std::size_t j = 0; j < imported_count; ++j)
            if (imported[j].identity == identity) {
                error = "Dynamic Character duplicates a retained source import row";
                return false;
            }
        for (std::size_t j = 0; j < i; ++j)
            if (dynamic_identities[j] == identity) {
                error = "Dynamic Character owner supplied a duplicate identity";
                return false;
            }
    }
    std::size_t visited = 0;
    auto* node = characters.sentinel.next;
    while (node != &characters.sentinel) {
        if (!node || !node->character || visited >= characters.character_count) {
            error = "Active CharacterList has malformed links or stale entries";
            return false;
        }
        const auto* character = static_cast<const aggro::Character*>(node->character);
        if (!character->identity || !character->object ||
            character->object->identity != character->identity ||
            !expected(character->identity) || !manager_has(character->identity)) {
            error = "Active CharacterList contains a Character outside retained source owners";
            return false;
        }
        ++visited;
        node = node->next;
    }
    if (visited != characters.character_count) {
        error = "Active CharacterList traversal differs from its retained count";
        return false;
    }
    error.clear();
    return true;
}

inline bool bind_existing_character_property_cache(
    std::uintptr_t character_identity, std::int16_t* property_cache,
    SafeGetBindings& output, std::string& error) {
    if (!character_identity || !property_cache) {
        error = "Character SafeGet cache binding is incomplete";
        return false;
    }
    output = {};
    output.character = character_identity;
    output.property_cache = property_cache;
    error.clear();
    return true;
}

inline bool bind_existing_character_caches(
    std::uintptr_t character_identity, std::int16_t* property_cache,
    std::int16_t* template_cache, SafeGetBindings& output,
    std::string& error) {
    if (!bind_existing_character_property_cache(character_identity,
            property_cache, output, error)) return false;
    if (!template_cache || template_cache == property_cache) {
        output = {};
        error = "Character SafeGet template cache binding is incomplete";
        return false;
    }
    output.template_cache = template_cache;
    return true;
}

// Adapter for the existing ObjectManager-keyed Character factory. The factory
// Record owns the source +0x13c8 halfword cache; this helper borrows it and
// deliberately supplies no invented template/RNG/Save services. A cache hit
// therefore follows the full original SafeGet runtime, while an uncached
// Record fails closed until its real source services are bound by its owner.
inline bool bind_factory_record_property_cache(void* context,
    std::uintptr_t event_owner, std::uintptr_t level_identity,
    std::uintptr_t character_identity, SafeGetBindings& output,
    std::string& error) {
    auto* factory = static_cast<dh2::character_runtime_factory_v1::Owner*>(context);
    if (!factory || !event_owner || !level_identity || !character_identity) {
        error = "Character factory SafeGet binding identity is incomplete";
        return false;
    }
    auto* record = factory->find_character(character_identity);
    if (!record || record->character.identity != character_identity ||
        record->game_object.identity != character_identity) {
        error = "CharacterList identity has no canonical Character factory Record";
        return false;
    }
    return bind_existing_character_property_cache(character_identity,
        &record->property_id_13c8,output,error);
}

// The original HasEnemyOfTypeLoaded walks the Level ObjectManager's flat
// CharacterList without player, faction, dead, or room filters. SafeGetCharPropsId
// is delegated to the canonical Character/property owner for each identity.
// A caller may set roster_complete only after its Level population owner has
// completed every source Character add/load path; the current Crypt projection
// does not yet provide that proof and must leave it false.
struct Bindings {
    void* context{};
    const list::Owner* characters{};
    const ObjectManager* object_manager{};
    const OidCache* oid_cache{};
    std::uintptr_t event_owner_identity{};
    std::uintptr_t level_identity{};
    std::int32_t level_id{-1};
    std::size_t expected_character_count{};
    std::uint64_t roster_generation{};
    std::uint32_t character_table_size{};
    std::uint64_t oid_cache_generation{};
    bool roster_complete{};
    bool (*safe_get_character_property_id)(void*, std::uintptr_t event_owner,
        std::uintptr_t level_identity, std::uintptr_t character_identity,
        std::int32_t* property_id, std::string& error){};
    // Preferred production path: resolve this identity to bindings over its
    // existing Character+0x13c8/+0x13ca caches, CharacterTable/templates,
    // RNG, and Save/LoadOwner. Provider runs the existing SafeGet runtime.
    bool (*safe_get_bindings)(void*, std::uintptr_t event_owner,
        std::uintptr_t level_identity, std::uintptr_t character_identity,
        SafeGetBindings&, std::string& error){};
};

// Assemble the population view from the application Level's existing
// ObjectManager and the shared native CharacterList. The source roster
// completeness fact is deliberately supplied by the lifecycle owner; counts
// and identity checks here cannot promote a partial actor projection to a
// complete source CharacterList.
inline bool make_active_level_bindings(
    const dh2::source_level_owner_v1::Owner& level_owner,
    const dh2::native::character_list::Owner& characters,
    const OidCache& oid_cache, std::uintptr_t event_owner_identity,
    std::int32_t level_id, std::uint32_t character_table_size,
    bool roster_complete, void* safe_get_context,
    bool (*safe_get_character_property_id)(void*,std::uintptr_t,
        std::uintptr_t,std::uintptr_t,std::int32_t*,std::string&),
    bool (*safe_get_bindings)(void*,std::uintptr_t,std::uintptr_t,
        std::uintptr_t,SafeGetBindings&,std::string&),
    Bindings& output, std::string& error) {
    output = {};
    const auto level = level_owner.snapshot();
    const auto& roster = characters.source();
    const auto& manager = level_owner.object_manager();
    if (level.phase != dh2::source_level_owner_v1::Phase::active ||
        !level.renderer_projection || !level.source_level ||
        !level.object_manager_identity ||
        level.object_manager_identity != reinterpret_cast<std::uintptr_t>(&manager) ||
        !event_owner_identity || level_id < 0 || !character_table_size ||
        (!safe_get_character_property_id && !safe_get_bindings)) {
        error = "Active Level Character population owners are incomplete";
        return false;
    }
    if (oid_cache.level_identity() != level.source_level ||
        !oid_cache.generation() ||
        oid_cache.character_table_size() != character_table_size) {
        error = "Character OID cache is not bound to this source Level and CharacterTable";
        return false;
    }
    if (!roster.mutation_generation || !list::validate(&roster) ||
        roster.character_count > manager.object_count()) {
        error = "Active Level CharacterList is malformed or exceeds its ObjectManager";
        return false;
    }
    output.context = safe_get_context;
    output.characters = &roster;
    output.object_manager = &manager;
    output.oid_cache = &oid_cache;
    output.event_owner_identity = event_owner_identity;
    output.level_identity = level.source_level;
    output.level_id = level_id;
    output.expected_character_count = roster.character_count;
    output.roster_generation = roster.mutation_generation;
    output.character_table_size = character_table_size;
    output.oid_cache_generation = oid_cache.generation();
    output.roster_complete = roster_complete;
    output.safe_get_character_property_id = safe_get_character_property_id;
    output.safe_get_bindings = safe_get_bindings;
    error.clear();
    return true;
}

// Production overload: the caller supplies identities read from the active
// Level's retained source import and identities retained by the dynamic
// Character owners. `roster_complete` is derived only after the complete live
// ObjectManager/CharacterList set matches those owners exactly.
inline bool make_active_level_bindings_from_imported_roster(
    const dh2::source_level_owner_v1::Owner& level_owner,
    const dh2::native::character_list::Owner& characters,
    const OidCache& oid_cache, std::uintptr_t event_owner_identity,
    std::int32_t level_id, std::uint32_t character_table_size,
    const ImportedCharacterIdentityV1* imported, std::size_t imported_count,
    const std::uintptr_t* dynamic_identities, std::size_t dynamic_count,
    void* safe_get_context,
    bool (*safe_get_character_property_id)(void*,std::uintptr_t,
        std::uintptr_t,std::uintptr_t,std::int32_t*,std::string&),
    bool (*safe_get_bindings)(void*,std::uintptr_t,std::uintptr_t,
        std::uintptr_t,SafeGetBindings&,std::string&),
    Bindings& output, std::string& error) {
    output = {};
    const auto level = level_owner.snapshot();
    if (level.phase != dh2::source_level_owner_v1::Phase::active ||
        !level.source_level || !level.object_manager_identity ||
        level.object_manager_identity != reinterpret_cast<std::uintptr_t>(
            &level_owner.object_manager())) {
        error = "Active Level ObjectManager is unavailable for source roster proof";
        return false;
    }
    if (!prove_imported_character_roster_complete(
            characters.source(), level_owner.object_manager(), imported, imported_count,
            dynamic_identities, dynamic_count, error)) return false;
    return make_active_level_bindings(level_owner, characters, oid_cache,
        event_owner_identity, level_id, character_table_size, true,
        safe_get_context, safe_get_character_property_id, safe_get_bindings,
        output, error);
}

class Provider final {
    Bindings bindings_{};
    bool busy_{};

    bool query(std::uintptr_t expected_event_owner,
               std::uintptr_t expected_level_identity,
               std::int32_t selector, bool by_template,
               quests::ClearEnemiesPopulationSnapshotV1& snapshot,
               std::string& error) {
        if (busy_) { error = "Current-Level Character population provider is reentrant"; return false; }
        struct Guard { bool& value; explicit Guard(bool& v):value(v){value=true;} ~Guard(){value=false;} } guard{busy_};
        snapshot = {};
        const auto& b = bindings_;
        if (!b.characters || !b.object_manager || !b.oid_cache ||
            (!b.safe_get_bindings&&!b.safe_get_character_property_id) || !b.event_owner_identity ||
            !b.level_identity || b.level_id < 0 || !b.character_table_size) {
            error = "Current-Level Character population bindings are incomplete"; return false;
        }
        if (expected_event_owner != b.event_owner_identity ||
            expected_level_identity != b.level_identity ||
            b.oid_cache->level_identity() != b.level_identity ||
            !b.oid_cache_generation ||
            b.oid_cache->generation() != b.oid_cache_generation ||
            b.oid_cache->character_table_size() != b.character_table_size) {
            error = "Current-Level Character population owner is stale"; return false;
        }
        if (by_template && !b.safe_get_bindings) {
            error = "TestCharTemplate requires the canonical SafeGet template-cache provider"; return false;
        }
        if ((!by_template && (selector < 0 ||
                static_cast<std::uint32_t>(selector) >= b.character_table_size)) ||
            (by_template && (selector < 0 || selector > 32767))) {
            error = by_template ? "Current-Level Character template selector is outside source halfword range" :
                "Current-Level Character property selector is outside CharacterTable"; return false;
        }
        if (!b.roster_complete) {
            error = "Current-Level Character roster is not source-complete"; return false;
        }
        if (!b.roster_generation ||
            b.characters->mutation_generation != b.roster_generation ||
            !list::validate(b.characters) ||
            b.characters->character_count != b.expected_character_count) {
            error = "Current-Level CharacterList is malformed or incomplete"; return false;
        }

        std::int64_t matches = 0;
        std::size_t visited = 0;
        // ObjectManager::Add must publish a Character exactly once. A second
        // list node with the same Character identity is a malformed/stale
        // roster, not a second source enemy. Fail closed before property
        // lookup so duplicate nodes can never inflate a QEST population.
        std::unordered_set<std::uintptr_t> seen;
        try { seen.reserve(b.expected_character_count); }
        catch (...) { error = "Current-Level Character identity set allocation failed"; return false; }
        auto* node = b.characters->sentinel.next;
        while (node != &b.characters->sentinel) {
            if (!node || visited >= b.expected_character_count || !node->character) {
                error = "Current-Level CharacterList contains a stale Character"; return false;
            }
            auto* character = static_cast<aggro::Character*>(node->character);
            if (!character->identity || !character->object ||
                character->object->identity != character->identity) {
                error = "Current-Level Character has an incomplete canonical projection"; return false;
            }
            try {
                if (!seen.insert(character->identity).second) {
                    error = "Current-Level CharacterList contains a duplicate Character identity";
                    return false;
                }
            } catch (...) {
                error = "Current-Level Character identity set allocation failed"; return false;
            }
            const auto* object = b.object_manager->find_by_identity(character->identity);
            if (!object || object->identity != character->identity) {
                error = "Current-Level Character is absent from its ObjectManager"; return false;
            }
            std::int32_t resolved_property_id = -1;
            std::int32_t resolved_template_id = -1;
            try {
                bool resolved=false;
                if (b.safe_get_bindings) {
                    SafeGetBindings safe{};
                    if (!b.safe_get_bindings(b.context,b.event_owner_identity,
                            b.level_identity,character->identity,safe,error)) {
                        if (error.empty()) error = "SafeGetCharPropsId bindings unavailable for a listed Character";
                        return false;
                    }
                    if (safe.character!=character->identity||!safe.property_cache||
                        (by_template&&!safe.template_cache)) {
                        error = by_template ? "SafeGet bindings do not borrow the listed Character template cache" :
                            "SafeGetCharPropsId bindings do not borrow the listed Character cache";
                        return false;
                    }
                    dh2::character_saved_class_v1::Runtime resolver(safe);
                    dh2::character_saved_class_v1::Result result{};
                    if (resolver.resolve(&result,error)!=dh2::character_saved_class_v1::Status::complete) {
                        if (error.empty()) error = "Character::SafeGetCharPropsId failed";
                        return false;
                    }
                    resolved_property_id=result.value;
                    if (safe.template_cache) resolved_template_id=*safe.template_cache;
                    resolved=true;
                } else {
                    resolved=b.safe_get_character_property_id(b.context,
                        b.event_owner_identity,b.level_identity,character->identity,
                        &resolved_property_id,error);
                }
                if (!resolved) {
                    if (error.empty()) error = "SafeGetCharPropsId failed for a listed Character";
                    return false;
                }
            } catch (...) {
                error = "SafeGetCharPropsId provider threw for a listed Character"; return false;
            }
            // SafeGetCharPropsId may fill the canonical cache and, for a
            // player, reach SG_Load(1). Revalidate before following a possibly
            // retired next link or publishing a mixed-generation snapshot.
            if (b.characters->mutation_generation != b.roster_generation ||
                b.characters->character_count != b.expected_character_count ||
                !list::validate(b.characters)) {
                error = "Current-Level Character roster changed during population query";
                return false;
            }
            if (!by_template && (resolved_property_id < -1 || resolved_property_id >= static_cast<std::int32_t>(b.character_table_size))) {
                error = "SafeGetCharPropsId returned an invalid CharacterTable row"; return false;
            }
            if (by_template) {
                if (resolved_template_id < -1 || resolved_template_id > 32767) {
                    error = "SafeGetCharPropsTemplateId returned an invalid source halfword"; return false;
                }
                if (resolved_template_id == selector) ++matches;
            } else if (resolved_property_id == selector) ++matches;
            ++visited;
            node = node->next;
        }
        if (visited != b.expected_character_count ||
            b.characters->mutation_generation != b.roster_generation ||
            b.characters->character_count != b.expected_character_count) {
            error = "Current-Level CharacterList changed during population query"; return false;
        }

        // Original HasEnemyOfTypeLoaded consults the Level OID cache only for
        // TestCharPropId. TestCharTemplate returns its loaded Character count.
        if (!by_template && matches == 0) {
            if (selector >= 0 &&
                static_cast<std::uint32_t>(selector) < b.character_table_size) {
                dh2::character_oid_cache_v1::Result cached{};
                const auto status = b.oid_cache->count(
                    static_cast<std::uint32_t>(selector), &cached);
                if (status != dh2::character_oid_cache_v1::Status::complete) {
                    error = "Current-Level Character OID cache query failed"; return false;
                }
                matches = cached.value;
            }
        }
        if (matches > std::numeric_limits<std::int32_t>::max()) {
            error = "Current-Level Character population exceeds source count range"; return false;
        }
        snapshot.event_owner_identity = b.event_owner_identity;
        snapshot.level_identity = b.level_identity;
        snapshot.level_id = b.level_id;
        snapshot.loaded_match_count = static_cast<std::int32_t>(matches);
        error.clear();
        return true;
    }

    static bool read_callback(void* context, std::uintptr_t expected_event_owner,
                              std::uintptr_t expected_level_identity,
                              std::int32_t property_id,
                              quests::ClearEnemiesPopulationSnapshotV1& snapshot,
                              std::string& error) {
        if (!context) { error = "Current-Level Character population provider is absent"; return false; }
        return static_cast<Provider*>(context)->query(expected_event_owner,
            expected_level_identity, property_id, false, snapshot, error);
    }
    static bool read_template_callback(void* context, std::uintptr_t expected_event_owner,
                              std::uintptr_t expected_level_identity,
                              std::int32_t template_id,
                              quests::ClearEnemiesPopulationSnapshotV1& snapshot,
                              std::string& error) {
        if (!context) { error = "Current-Level Character template provider is absent"; return false; }
        return static_cast<Provider*>(context)->query(expected_event_owner,
            expected_level_identity, template_id, true, snapshot, error);
    }

public:
    explicit Provider(Bindings bindings) noexcept : bindings_(bindings) {}
    Provider(const Provider&) = delete;
    Provider& operator=(const Provider&) = delete;

    quests::ClearEnemiesPopulationServicesV1 services() noexcept {
        return {this, &read_callback, bindings_.level_identity};
    }
    quests::KillEnemyTemplatePopulationServicesV1 kill_enemy_template_services() noexcept {
        return {this, &read_template_callback, bindings_.level_identity};
    }
    quests::ClearEnemyTemplatePopulationServicesV1 clear_enemy_template_services() noexcept {
        return {this, &read_template_callback, bindings_.level_identity};
    }
};

} // namespace dh2::native::current_level_character_population_v1
