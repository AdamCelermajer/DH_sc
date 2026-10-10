#include "actor_population.hpp"

#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <utility>

namespace dh::foundation {
namespace {
std::string property(const ActorDefinition& actor, const char* name) {
    const auto found = actor.properties.find(name);
    return found == actor.properties.end() ? "" : found->second;
}
bool profile_transform(const ActorProfile& profile, const Mat4& placement,
                       Mat4& output, std::string& error) {
    Mat4 transform = placement;
    const char* keys[]{"Scale_X", "Scale_Y", "Scale_Z"};
    for (unsigned axis = 0; axis < 3; ++axis) {
        const auto found = profile.raw_properties.find(keys[axis]);
        if (found == profile.raw_properties.end()) continue;
        const auto& scale = found->second;
        float factor = 0;
        if (scale.encoding == "original-character-scale") {
            if (scale.raw < std::numeric_limits<std::int32_t>::min() ||
                scale.raw > std::numeric_limits<std::int32_t>::max()) {
                error = std::string("Original scale outside int32 range for ") + keys[axis];
                return false;
            }
            // Exact reconstructed dh2_character_visual_scale coefficients;
            // horizontal axes differ from Z and must not use uniform /100.
            const std::uint32_t bits = axis == 2 ? 0x3c23d70au : 0x3c1374bcu;
            float coefficient;
            std::memcpy(&coefficient, &bits, sizeof(coefficient));
            factor = static_cast<float>(scale.raw) * coefficient;
        } else if (scale.encoding == "percent" || scale.encoding == "original-percent") {
            factor = static_cast<float>(static_cast<double>(scale.raw) / 100.0);
        } else {
            error = std::string("Unsupported scale encoding for ") + keys[axis] + ": " + scale.encoding;
            return false;
        }
        if (!std::isfinite(factor)) {
            error = std::string("Invalid original scale for ") + keys[axis];
            return false;
        }
        for (unsigned row = 0; row < 4; ++row) {
            transform[axis*4+row] *= factor;
            if (!std::isfinite(transform[axis*4+row])) { error = "Actor profile scale overflow"; return false; }
        }
    }
    output = transform;
    return true;
}

bool select_template_profile(const ActorDefinition& definition,
                             const PopulationTemplateSelectionV1& services,
                             std::string& profile_name,
                             std::int16_t& character_cache,
                             std::int16_t& template_cache,
                             std::string& error) {
    const auto template_name = property(definition, "char_template");
    const auto table_name = property(definition, "char_template_pydata");
    if (template_name.empty()) { error = "Symbolic actor has no authored char_template"; return false; }
    if (table_name != "Charater_Templates") {
        error = "Unsupported or missing char_template_pydata table: " + table_name;
        return false;
    }
    if (!services.characters || !services.templates) {
        error = "Required actual CharacterTable and CharacterTemplateTableV78";
        return false;
    }

    // Match SafeGetCharPropsTemplateId's signed16 cache and actual template
    // name lookup before requesting the authored member array.
    std::int32_t template_id = -1;
    if (!services.templates->safe_template_id(template_name, template_cache,
                                               template_id, error)) return false;
    if (template_id < 0) {
        error = "Authored Charater_Templates row is unavailable: " + template_name;
        return false;
    }
    const std::int16_t* members = nullptr;
    std::uint32_t count = 0;
    if (!services.templates->preset(template_name, members, count, error)) return false;
    if (!count) {
        error = "Authored CharacterTemplate member array is empty: " + template_name;
        return false;
    }
    if (!members) { error = "CharacterTemplate member array has no retained storage"; return false; }
    if (!services.random_index) {
        error = "Required external shared source RNG callback for CharacterTemplate selection";
        return false;
    }
    std::int32_t selected = -1;
    if (!services.random_index(static_cast<std::int32_t>(count), selected, error)) {
        if (error.empty()) error = "External shared source RNG rejected CharacterTemplate selection";
        return false;
    }
    if (selected < 0 || static_cast<std::uint32_t>(selected) >= count) {
        error = "External source RNG returned an out-of-range CharacterTemplate member index";
        return false;
    }

    // `selected_ids` is the source's signed16 projection of CharacterTable
    // row IDs. Preserve duplicates in the member array: they are weights.
    character_cache = members[selected];
    if (character_cache < 0 || static_cast<std::size_t>(character_cache) >= services.characters->rows.size() ||
        static_cast<std::size_t>(character_cache) >= services.characters->names.size()) {
        error = "Selected CharacterTemplate member is outside the actual CharacterTable";
        return false;
    }
    profile_name = services.characters->names[static_cast<std::size_t>(character_cache)];
    if (profile_name.empty()) { error = "Selected CharacterTable row has no source name"; return false; }
    error.clear();
    return true;
}
}

bool ActorPopulation::load(const AssetCatalog& assets, const std::string& levelURI,
                           const ActorProfileLibrary& profiles, const PopulationPolicy& policy,
                           const PopulationCustomization& customization, std::string& error,
                           const PopulationTemplateSelectionV1* template_selection) {
    try {
        if (!policy || !customization) throw std::runtime_error("Population requires explicit condition and customization callbacks");
        std::vector<ActorDefinition> definitions;
        if (!load_actor_definitions(assets, levelURI, definitions, error)) return false;
        auto retainedDefinitions = definitions;
        std::vector<PopulationActor> actors;
        std::vector<PopulationNotice> notices;
        std::size_t authored = 0, excluded = 0;
        for (auto& definition : definitions) {
            if (definition.gametype != "Character" && definition.gametype != "SpawnPoint") continue;
            ++authored;
            const auto decision = policy(definition);
            if (decision == PopulationDecision::exclude) { ++excluded; continue; }
            if (decision != PopulationDecision::include && decision != PopulationDecision::deferred) {
                notices.push_back({definition.sourceId, "Condition policy returned unknown; actor not instantiated"});
                continue;
            }
            if (definition.gametype == "SpawnPoint") {
                notices.push_back({definition.sourceId, "Spawn point requires caller-owned spawning policy; no actor selected"});
                continue;
            }
            auto binding = property(definition, "charpropsname");
            std::int16_t source_character_cache = -1, source_template_cache = -1;
            if (binding.empty()) {
                const auto symbolic = property(definition, "char_template");
                if (symbolic.empty()) {
                    notices.push_back({definition.sourceId, "Character lacks explicit charpropsname binding"});
                    continue;
                }
                if (!template_selection) {
                    notices.push_back({definition.sourceId, "Symbolic character template requires caller-owned selection: " + symbolic});
                    continue;
                }
                std::string selection_error;
                if (!select_template_profile(definition, *template_selection, binding,
                                             source_character_cache, source_template_cache,
                                             selection_error)) {
                    notices.push_back({definition.sourceId, std::move(selection_error)});
                    continue;
                }
            }
            const auto* profile = profiles.find(binding);
            if (!profile) {
                notices.push_back({definition.sourceId, "Explicit character profile unavailable: " + binding});
                continue;
            }
            PopulationActor actor;
            actor.initial_decision = decision;
            actor.enabled = actor.initially_enabled = decision == PopulationDecision::include;
            actor.profileId = profile->id;
            actor.source_character_cache = source_character_cache;
            actor.source_template_cache = source_template_cache;
            std::string gap;
            if (!profile_transform(*profile, definition.placement, actor.transform, gap)) {
                notices.push_back({definition.sourceId, std::move(gap)});
                continue;
            }
            CharacterVisualConfig config;
            if (!make_visual_config(assets, *profile, customization(definition, *profile), config, gap) ||
                !actor.visual.load(assets, config, gap)) {
                notices.push_back({definition.sourceId, "Original visual unavailable for " + binding + ": " + gap});
                continue;
            }
            actor.definition = std::move(definition);
            actors.push_back(std::move(actor));
        }
        actors_.swap(actors);
        notices_.swap(notices);
        definitions_.swap(retainedDefinitions);
        authored_count_ = authored;
        excluded_count_ = excluded;
        error.clear();
        return true;
    } catch (const std::exception& exception) { error = exception.what(); return false; }
}

bool ActorPopulation::set_enabled(std::uint64_t stableId, bool enabled, std::string& error) {
    PopulationActor* selected = nullptr;
    for (auto& actor : actors_) {
        if (actor.definition.stableId != stableId) continue;
        if (selected) { error = "Population stable ID is ambiguous"; return false; }
        selected = &actor;
    }
    if (!selected) { error = "Population actor stable ID is unavailable"; return false; }
    selected->enabled = enabled;
    error.clear();
    return true;
}

std::size_t ActorPopulation::enabled_count() const noexcept {
    std::size_t count = 0;
    for (const auto& actor : actors_) if (actor.enabled) ++count;
    return count;
}

std::size_t ActorPopulation::initial_deferred_count() const noexcept {
    std::size_t count = 0;
    for (const auto& actor : actors_) if (actor.initial_decision == PopulationDecision::deferred) ++count;
    return count;
}

} // namespace dh::foundation
