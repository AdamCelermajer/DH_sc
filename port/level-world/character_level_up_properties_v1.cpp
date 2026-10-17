#include "character_level_up_properties_v1.hpp"

#include <vector>

namespace dh2::character_level_up_properties_v1 {

Status apply(const Owner* owner, Result* result) {
    if (!owner || !result || !owner->character || !owner->properties ||
        !owner->save || owner->save->character() != owner->character ||
        dh2_property_validate(owner->properties)) {
        return Status::invalid_argument;
    }

    *result = {};
    ++result->calls;
    result->level_add_reached = 1;
    if (dh2_property_add(owner->properties, 19, 1 << 8)) {
        return Status::property_failed;
    }

    ++result->calls;
    result->xp_reset_reached = 1;
    if (dh2_property_set_int(owner->properties, 33, 0)) {
        return Status::property_failed;
    }
    return Status::complete;
}

Status update_base_properties(const Owner* owner,
                              const data::PropertyRules* rules,
                              const data::CharacterTable* characters,
                              const data::ClassTables* classes,
                              std::int32_t class_row, Result* result) {
    if (!owner || !result || !rules || !characters || !classes ||
        !owner->character || !owner->properties || !owner->save ||
        owner->save->character() != owner->character ||
        dh2_property_validate(owner->properties) ||
        !owner->mutable_base || owner->properties->base != owner->mutable_base ||
        class_row < 0 ||
        std::size_t(class_row) >= characters->rows.size() ||
        characters->names.size() != characters->rows.size() ||
        classes->rows.empty() || classes->rows.size() > 10000) {
        return Status::invalid_argument;
    }

    std::vector<data::ClassRow> rows;
    try {
        rows.reserve(classes->rows.size());
        for (const auto& formulas : classes->rows) {
            if (formulas.size() > 10000) return Status::invalid_argument;
            rows.push_back({formulas.empty() ? nullptr : formulas.data(),
                            std::uint32_t(formulas.size())});
        }
    } catch (...) {
        return Status::property_failed;
    }

    *result = {};
    ++result->calls;
    result->base_reset_reached = 1;
    for (std::size_t property = 0; property < rules->defaults.size(); ++property) {
        owner->mutable_base[property] = rules->defaults[property];
    }

    ++result->calls;
    result->base_row_loaded = 1;
    for (std::size_t property = 0; property < characters->rows[std::size_t(class_row)].size(); ++property) {
        owner->mutable_base[property] = characters->rows[std::size_t(class_row)][property];
    }

    ++result->calls;
    result->class_recalc_reached = 1;
    result->class_result = dh2_class_recalc_base(
        rows.data(), std::uint32_t(rows.size()), owner->mutable_base,
        owner->properties);
    return result->class_result ? Status::property_failed : Status::complete;
}

} // namespace dh2::character_level_up_properties_v1
