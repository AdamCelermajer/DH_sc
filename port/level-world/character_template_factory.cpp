#include "character_template_factory.hpp"

#include <utility>

namespace dh2::character::template_factory {
namespace {

constexpr const char* kCharacterTemplateDataClass = "Charater_Templates";

Resolution invalid(Status status, bool explicit_property = false) {
    Resolution result;
    result.status = status;
    result.explicit_property = explicit_property;
    return result;
}

Resolution resolve_property(const Catalog& catalog, std::int32_t property_id,
                            bool explicit_property, std::int32_t alternative,
                            std::vector<std::int32_t> alternatives) {
    if (property_id < 0 ||
        static_cast<std::uint32_t>(property_id) >= catalog.characters.size())
        return invalid(Status::property_id_out_of_range, explicit_property);

    const auto& character = catalog.characters[static_cast<std::size_t>(property_id)];
    if (character.name.empty())
        return invalid(Status::property_id_out_of_range, explicit_property);

    Resolution result;
    result.status = Status::resolved;
    result.explicit_property = explicit_property;
    result.alternative_index = alternative;
    result.property_id = property_id;
    result.property_name = character.name;
    result.authored_alternatives = std::move(alternatives);

    if (character.class_id < -1 ||
        (character.class_id >= 0 &&
         static_cast<std::uint32_t>(character.class_id) >= catalog.class_names.size()))
        return invalid(Status::invalid_class_id, explicit_property);

    result.class_id = character.class_id;
    if (character.class_id >= 0) {
        result.class_formula_selected = true;
        result.class_name = catalog.class_names[static_cast<std::size_t>(character.class_id)];
        if (result.class_name.empty()) return invalid(Status::invalid_class_id, explicit_property);
    }
    return result;
}

}  // namespace

Resolution resolve(const Source& source, const Catalog& catalog,
                   std::int32_t selected_alternative) {
    if (selected_alternative < selection_required) return invalid(Status::invalid_source);

    const bool has_property = !source.explicit_property_name.empty();
    const bool has_template = !source.template_name.empty() ||
                              !source.template_data_class.empty();
    if (has_property && has_template)
        return invalid(Status::conflicting_property_sources, true);

    if (has_property) {
        if (selected_alternative != selection_required)
            return invalid(Status::invalid_source, true);

        std::int32_t property_id = -1;
        std::uint32_t matches = 0;
        for (std::uint32_t i = 0; i < catalog.characters.size(); ++i) {
            if (catalog.characters[i].name == source.explicit_property_name) {
                property_id = static_cast<std::int32_t>(i);
                ++matches;
            }
        }
        if (matches > 1) return invalid(Status::ambiguous_property, true);
        if (matches != 1) return invalid(Status::property_not_found, true);
        return resolve_property(catalog, property_id, true, selection_required, {});
    }

    if (source.template_name.empty() || source.template_data_class.empty())
        return invalid(Status::invalid_source);
    if (source.template_data_class != kCharacterTemplateDataClass)
        return invalid(Status::unsupported_template_data_class);

    const TemplateRecord* record = nullptr;
    std::uint32_t matches = 0;
    for (const auto& candidate : catalog.templates) {
        if (candidate.name == source.template_name) {
            record = &candidate;
            ++matches;
        }
    }
    if (matches == 0) return invalid(Status::template_not_found);
    if (matches != 1) return invalid(Status::ambiguous_template);
    if (record->property_ids.empty()) return invalid(Status::template_has_no_alternatives);

    if (selected_alternative == selection_required) {
        Resolution result;
        result.status = Status::selection_required;
        result.authored_alternatives = record->property_ids;
        return result;
    }

    if (static_cast<std::uint32_t>(selected_alternative) >= record->property_ids.size())
        return invalid(Status::alternative_out_of_range);

    return resolve_property(catalog,
        record->property_ids[static_cast<std::size_t>(selected_alternative)], false,
        selected_alternative, record->property_ids);
}

const char* status_name(Status status) {
    switch (status) {
    case Status::resolved: return "resolved";
    case Status::selection_required: return "selection_required";
    case Status::invalid_source: return "invalid_source";
    case Status::conflicting_property_sources: return "conflicting_property_sources";
    case Status::unsupported_template_data_class: return "unsupported_template_data_class";
    case Status::template_not_found: return "template_not_found";
    case Status::ambiguous_template: return "ambiguous_template";
    case Status::template_has_no_alternatives: return "template_has_no_alternatives";
    case Status::alternative_out_of_range: return "alternative_out_of_range";
    case Status::property_not_found: return "property_not_found";
    case Status::property_id_out_of_range: return "property_id_out_of_range";
    case Status::ambiguous_property: return "ambiguous_property";
    case Status::invalid_class_id: return "invalid_class_id";
    }
    return "unknown";
}

}  // namespace dh2::character::template_factory
