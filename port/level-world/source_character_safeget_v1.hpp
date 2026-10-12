#pragma once

#include "character_saved_class_v1.hpp"
#include "world.hpp"
#include "../game-data/data.hpp"
#include "character_template_factory.hpp"



#include <cstdint>
#include <string>
#include <utility>

namespace dh2::source_character_safeget_v1 {

// The result is a view of one retained MGP Character after the canonical
// Character::SafeGetCharPropsId owner has resolved its authored property
// route. The cache storage remains owned by the Character runtime Record;
// this value only carries provenance and the resolved source table identity.
struct Resolved {
    std::uint32_t module_index{};
    std::uint32_t source_record{};
    std::int32_t source_handle{-1};
    std::uintptr_t character_identity{};
    std::int16_t property_cache{-1};
    std::int16_t template_cache{-1};
    std::uint32_t random_draws{};
    std::string object_name;
    std::string character_table_name;
};

// Construct the canonical template-factory source from fields copied by the
// single source MGP census. This avoids reparsing Level/MGP bytes through a
// second world parser and keeps XML fields distinct from editor/table labels.
inline bool make_authored_source(const world::SourceCharacterCensusV1& row,
                                 character::template_factory::Source* output,
                                 std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || row.source_handle < 0 || row.name.empty()) {
        error = "Source Character property census row has an invalid source identity";
        return false;
    }
    output->object_name = row.name;
    output->editor_template_name = row.template_name;
    output->template_data_class = row.char_template_pydata;
    output->template_name = row.char_template;
    output->explicit_property_name = row.character;
    return true;
}
// Resolve a census row against the exact MGP property source and the existing
// canonical Character cache/service bindings. `owner_bindings` must borrow the
// Character's +0x13c8/+0x13ca halfwords and the shared RNG/Save/LoadOwner slots.
// The actual SafeGet kernel performs cache bypass, player handling, template
// lookup, weighted slot draw, and publication; this adapter never owns any of
// those resources. Conditions do not gate roster import and are deliberately
// not consulted here. Duplicate-name ObjectManager rows have no distinct live
// Character owner and are refused.
inline bool resolve(const world::SourceCharacterCensusV1& source_row,
                    const character::template_factory::Source& authored,
                    const data::CharacterTable& character_table,
                    character_saved_class_v1::Bindings owner_bindings,
                    Resolved* output, std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || source_row.source_handle < 0 || source_row.duplicate_name ||
        source_row.name.empty() || !owner_bindings.character ||
        !owner_bindings.property_cache || !owner_bindings.template_cache ||
        authored.object_name != source_row.name ||
        authored.editor_template_name != source_row.template_name ||
        (!source_row.character.empty() &&
         authored.explicit_property_name != source_row.character) ||
        character_table.names.empty() ||
        character_table.rows.size() != character_table.names.size() ||
        (owner_bindings.characters && owner_bindings.characters != &character_table)) {
        error = "Source Character SafeGet import bindings do not match the retained MGP row";
        return false;
    }
    if (owner_bindings.authored && owner_bindings.authored != &authored) {
        error = "Source Character SafeGet import must use the retained MGP property source";
        return false;
    }

    owner_bindings.authored = &authored;
    owner_bindings.characters = &character_table;
    character_saved_class_v1::Runtime runtime(owner_bindings);
    character_saved_class_v1::Result safeget{};
    if (runtime.resolve(&safeget, error) !=
        character_saved_class_v1::Status::complete) {
        if (error.empty()) error = "Character::SafeGetCharPropsId failed for a source MGP row";
        return false;
    }

    const auto property_id = safeget.value;
    if (property_id < 0 ||
        static_cast<std::size_t>(property_id) >= character_table.names.size() ||
        *owner_bindings.property_cache != property_id) {
        error = "Source Character SafeGet result does not resolve to a CharacterTable row (" +
            authored.template_name + " -> " + std::to_string(property_id) + ")";
        return false;
    }
    const auto template_id = owner_bindings.template_cache
        ? *owner_bindings.template_cache : static_cast<std::int16_t>(-1);
    if (template_id < -1) {
        error = "Source Character SafeGet template cache is outside its signed halfword domain";
        return false;
    }
    if (!authored.template_name.empty() && template_id >= 0) {
        if (!owner_bindings.templates ||
            static_cast<std::size_t>(template_id) >=
                owner_bindings.templates->templates.size() ||
            owner_bindings.templates->templates[
                static_cast<std::size_t>(template_id)].name != authored.template_name) {
            error = "Source Character SafeGet template cache does not match its authored template";
            return false;
        }
    }

    Resolved result{};
    result.module_index = source_row.module_index;
    result.source_record = source_row.source_record;
    result.source_handle = source_row.source_handle;
    result.character_identity = owner_bindings.character;
    result.property_cache = *owner_bindings.property_cache;
    result.template_cache = template_id;
    result.random_draws = safeget.random_draws;
    result.object_name = source_row.name;
    result.character_table_name = character_table.names[
        static_cast<std::size_t>(property_id)];
    *output = std::move(result);
    error.clear();
    return true;
}

// Production entry point: authored property fields already came through the
// same source census as ObjectManager provenance, so no secondary parser or
// parallel Character/cache/RNG/Save owner is involved.
inline bool resolve(const world::SourceCharacterCensusV1& source_row,
                    const data::CharacterTable& character_table,
                    character_saved_class_v1::Bindings owner_bindings,
                    Resolved* output, std::string& error) {
    character::template_factory::Source authored{};
    if (!make_authored_source(source_row, &authored, error)) {
        if (output) *output = {};
        return false;
    }
    return resolve(source_row, authored, character_table, owner_bindings,
                   output, error);
}
} // namespace dh2::source_character_safeget_v1
