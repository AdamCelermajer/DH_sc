#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::character::template_factory {

// MGP source attributes are kept separate from the editor primitive type.
// `_templateName=MonsterCommonType1` is editor metadata; the runtime template
// key is `char_template=...` in `char_template_pydata=Charater_Templates`.
struct Source {
    std::string object_name;
    std::string editor_template_name;
    std::string template_data_class;
    std::string template_name;
    std::string explicit_property_name;
};

struct TemplateRecord {
    std::string name;
    // Preserve repeated CharInfo values: duplicates are authored selection
    // slots and therefore affect the source selection weighting.
    std::vector<std::int32_t> property_ids;
};

struct CharacterRecord {
    std::string name;
    std::int32_t class_id = -1;
};

struct Catalog {
    // `characters[id]` must be the CharacterTable row indexed by that ID.
    std::vector<TemplateRecord> templates;
    std::vector<CharacterRecord> characters;
    std::vector<std::string> class_names;
};

enum class Status : std::uint32_t {
    resolved,
    selection_required,
    invalid_source,
    conflicting_property_sources,
    unsupported_template_data_class,
    template_not_found,
    ambiguous_template,
    template_has_no_alternatives,
    alternative_out_of_range,
    property_not_found,
    property_id_out_of_range,
    ambiguous_property,
    invalid_class_id,
};

constexpr std::int32_t selection_required = -1;

struct Resolution {
    Status status = Status::invalid_source;
    bool explicit_property = false;
    bool class_formula_selected = false;
    std::int32_t alternative_index = selection_required;
    std::int32_t property_id = -1;
    std::int32_t class_id = -1;
    std::string property_name;
    std::string class_name;
    std::vector<std::int32_t> authored_alternatives;
};

// Resolves only authored links. For a template with several CharInfo entries,
// pass the index already selected by the game's RNG. `selection_required`
// returns the full weighted slot list and deliberately performs no random
// draw. Class formulas and the 224-property default/recalculation stage remain
// the responsibility of the existing game-data property pipeline.
Resolution resolve(const Source&, const Catalog&,
                   std::int32_t selected_alternative = selection_required);

const char* status_name(Status);

}  // namespace dh2::character::template_factory
