#include "character_template_runtime_v1.hpp"

#include "character_template_random.hpp"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <limits>

namespace dh2::character_template_runtime_v1 {
namespace {

constexpr const char* kTemplateDataClass = "Charater_Templates";

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) noexcept {
    if (!left || !right || !left_size || !right_size) return false;
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    if (a > UINTPTR_MAX - left_size || b > UINTPTR_MAX - right_size) return true;
    return a < b + right_size && b < a + left_size;
}

bool disjoint(const Bindings& bindings, const Result* result,
              const std::string* error) noexcept {
    const auto no_alias = [&](const void* p, std::size_t n) {
        if (overlaps(p, n, bindings.property_cache, sizeof(std::int16_t)) ||
            overlaps(p, n, bindings.template_cache, sizeof(std::int16_t)) ||
            overlaps(p, n, bindings.random, sizeof(dh2_random_state)) ||
            (result && overlaps(p, n, result, sizeof(*result))) ||
            (error && overlaps(p, n, error, sizeof(*error))) ||
            overlaps(p, n, bindings.catalog, sizeof(*bindings.catalog)) ||
            overlaps(p, n, bindings.characters, sizeof(*bindings.characters)) ||
            overlaps(p, n, bindings.models, sizeof(*bindings.models)))
            return false;
        return true;
    };
    if (!bindings.property_cache || !bindings.template_cache ||
        !bindings.catalog || !bindings.characters || !bindings.models ||
        !no_alias(bindings.template_data_class,
                  bindings.template_data_class ? std::strlen(bindings.template_data_class) + 1 : 0) ||
        !no_alias(bindings.template_name,
                  bindings.template_name ? std::strlen(bindings.template_name) + 1 : 0))
        return false;
    return true;
}

bool field_index(const data::CharacterTable& table, const char* name,
                 std::size_t& output) noexcept {
    const auto found = std::find(table.fields.begin(), table.fields.end(), name);
    if (found == table.fields.end() ||
        std::find(std::next(found), table.fields.end(), name) != table.fields.end())
        return false;
    output = static_cast<std::size_t>(found - table.fields.begin());
    return output < 224;
}

Status fail(Status status, const char* message, std::string& error) noexcept {
    try {
        error = message;
    } catch (...) {
    }
    return status;
}

} // namespace

Status resolve(const Bindings& bindings, Result* output,
               std::string& error) noexcept {
    try {
    error.clear();
    if (!output || !bindings.property_cache || !bindings.template_cache ||
        reinterpret_cast<std::uintptr_t>(bindings.property_cache) % alignof(std::int16_t) ||
        reinterpret_cast<std::uintptr_t>(bindings.template_cache) % alignof(std::int16_t) ||
        overlaps(bindings.property_cache, sizeof(*bindings.property_cache),
                 bindings.template_cache, sizeof(*bindings.template_cache)) ||
        overlaps(output, sizeof(*output), bindings.property_cache,
                 sizeof(*bindings.property_cache)) ||
        overlaps(output, sizeof(*output), bindings.template_cache,
                 sizeof(*bindings.template_cache)) ||
        overlaps(output, sizeof(*output), bindings.random,
                 bindings.random ? sizeof(*bindings.random) : 0) ||
        overlaps(output, sizeof(*output), &error, sizeof(error)) ||
        overlaps(output, sizeof(*output), &bindings, sizeof(bindings)) ||
        overlaps(&error, sizeof(error), &bindings, sizeof(bindings)) ||
        !disjoint(bindings, output, &error))
        return fail(Status::invalid_argument, "Template runtime bindings alias or are incomplete", error);

    *output = {};
    if (*bindings.property_cache != -1) {
        output->property_id = *bindings.property_cache;
        output->template_id = *bindings.template_cache;
        return Status::cached;
    }
    if (!bindings.template_data_class ||
        std::strcmp(bindings.template_data_class, kTemplateDataClass) != 0 ||
        !bindings.template_name || !*bindings.template_name ||
        !bindings.catalog->templates.size() ||
        bindings.characters->names.empty() ||
        bindings.characters->rows.size() != bindings.characters->names.size() ||
        bindings.catalog->characters.size() != bindings.characters->names.size() ||
        bindings.models->values.empty())
        return fail(Status::invalid_source,
                    "Runtime template source or loaded tables are incomplete", error);

    const character::template_factory::Source source{
        "", "", bindings.template_data_class, bindings.template_name, ""};
    const auto pending = character::template_factory::resolve(
        source, *bindings.catalog, character::template_factory::selection_required);
    if (pending.status == character::template_factory::Status::template_not_found)
        return fail(Status::template_not_found, "Character template name is absent", error);
    if (pending.status == character::template_factory::Status::ambiguous_template)
        return fail(Status::ambiguous_template, "Character template name is ambiguous", error);
    std::size_t template_id = 0;
    for (; template_id < bindings.catalog->templates.size(); ++template_id) {
        if (bindings.catalog->templates[template_id].name == bindings.template_name) break;
    }
    if (template_id >= bindings.catalog->templates.size() ||
        template_id > static_cast<std::size_t>(std::numeric_limits<std::int16_t>::max()))
        return fail(Status::invalid_source, "Character template ID cannot fit its source halfword cache", error);
    *bindings.template_cache = static_cast<std::int16_t>(template_id);
    output->template_id = static_cast<std::int32_t>(template_id);
    if (pending.status == character::template_factory::Status::template_has_no_alternatives)
        return Status::no_alternatives;
    if (pending.status != character::template_factory::Status::selection_required)
        return fail(Status::invalid_source, "Character template route is invalid", error);

    std::size_t model_field = 0, animation_field = 0;
    if (!field_index(*bindings.characters, "ModelFile", model_field) ||
        !field_index(*bindings.characters, "AnimTable", animation_field))
        return fail(Status::incompatible_alternatives,
                    "CharacterTable lacks unique ModelFile/AnimTable selectors", error);

    const auto& alternatives = pending.authored_alternatives;
    if (!bindings.random)
        return fail(Status::random_unavailable, "Shared ordinary Random state is required", error);
    character::template_random::Selection selection{};
    if (alternatives.size() > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()) ||
        character::template_random::select_uncached_slot(
            bindings.random, static_cast<std::int32_t>(alternatives.size()), &selection).status !=
                character::template_random::Status::selected ||
        selection.slot_index < 0 ||
        static_cast<std::size_t>(selection.slot_index) >= alternatives.size())
        return fail(Status::selection_failed, "Shared ordinary Random template draw failed", error);

    const auto selected_id = alternatives[static_cast<std::size_t>(selection.slot_index)];
    if (selected_id < 0 || selected_id > std::numeric_limits<std::int16_t>::max() ||
        static_cast<std::size_t>(selected_id) >= bindings.characters->rows.size() ||
        static_cast<std::size_t>(selected_id) >= bindings.catalog->characters.size() ||
        bindings.catalog->characters[static_cast<std::size_t>(selected_id)].name !=
            bindings.characters->names[static_cast<std::size_t>(selected_id)])
        return fail(Status::invalid_source,
                    "Selected Character template alternative does not map to the loaded CharacterTable", error);

    // SafeGetCharPropsId caches the selected CharacterTable ID after its draw.
    // Resource lookup belongs to the selected Character only: the actor factory
    // loads that row's model and animation bank, so alternatives in one authored
    // template do not need a shared preload identity.
    *bindings.property_cache = static_cast<std::int16_t>(selected_id);
    output->property_id = selected_id;
    output->selected_slot = selection.slot_index;
    output->random_draws = 1;

    const auto& row = bindings.characters->rows[static_cast<std::size_t>(selected_id)];
    const auto model_id = row[model_field];
    const auto animation_id = row[animation_field];
    if (model_id < 0 || static_cast<std::size_t>(model_id) >= bindings.models->values.size() ||
        bindings.models->values[static_cast<std::size_t>(model_id)].empty() || animation_id < 0)
        return fail(Status::incompatible_alternatives,
                    "Selected template Character has no usable model or animation selector", error);
    return Status::selected;
    } catch (...) {
        if (output && !overlaps(output, sizeof(*output), &error, sizeof(error)))
            *output = {};
        return fail(Status::selection_failed,
                    "Character template selection failed unexpectedly", error);
    }
}

const char* status_name(Status status) noexcept {
    switch (status) {
    case Status::selected: return "selected";
    case Status::cached: return "cached";
    case Status::no_alternatives: return "no_alternatives";
    case Status::invalid_argument: return "invalid_argument";
    case Status::invalid_source: return "invalid_source";
    case Status::template_not_found: return "template_not_found";
    case Status::ambiguous_template: return "ambiguous_template";
    case Status::incompatible_alternatives: return "incompatible_alternatives";
    case Status::random_unavailable: return "random_unavailable";
    case Status::selection_failed: return "selection_failed";
    }
    return "unknown";
}

} // namespace dh2::character_template_runtime_v1
