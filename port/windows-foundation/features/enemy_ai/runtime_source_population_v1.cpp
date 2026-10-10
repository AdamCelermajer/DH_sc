#include "runtime_source_population_v1.hpp"

#include <charconv>
#include <limits>
#include <unordered_map>

namespace dh::foundation::enemy_ai {
namespace {
constexpr std::size_t kMaximumSourceDeclarations = 100000;

bool authored_probability(const ActorDefinition& definition,
                         std::int32_t source_probability,
                         std::string& error) {
    const auto found = definition.properties.find("spawn_prob");
    if (found == definition.properties.end()) return true; // May be inherited by the source object.
    std::int32_t parsed = 0;
    const auto* begin = found->second.data();
    const auto* end = begin + found->second.size();
    const auto result = std::from_chars(begin, end, parsed);
    if (result.ec != std::errc{} || result.ptr != end) {
        error = "Authored spawn_prob is not a signed integer for " + definition.sourceId;
        return false;
    }
    if (parsed != source_probability) {
        error = "Borrowed source probability disagrees with authored spawn_prob for " + definition.sourceId;
        return false;
    }
    return true;
}

SourcePopulationPlanStatusV1 status_for(PopulationDecision decision) {
    switch (decision) {
    case PopulationDecision::include: return SourcePopulationPlanStatusV1::include;
    case PopulationDecision::exclude: return SourcePopulationPlanStatusV1::exclude;
    case PopulationDecision::deferred: return SourcePopulationPlanStatusV1::deferred;
    case PopulationDecision::unknown: return SourcePopulationPlanStatusV1::unknown;
    }
    return SourcePopulationPlanStatusV1::unknown;
}
}

bool plan_source_population_v1(const SourcePopulationPlannerInputV1& input,
                               std::vector<SourcePopulationPlanRowV1>& output,
                               std::string& error) {
    if (!input.definitions || !input.selected_actors || !input.activation_decision ||
        !input.bind_spawn_context) {
        error = "Source population planning requires declarations, selected rows, source gates, and a spawn-context binder";
        return false;
    }
    if (input.definitions->size() > kMaximumSourceDeclarations ||
        input.selected_actors->size() > kMaximumSourceDeclarations) {
        error = "Source population input exceeds the 100000 declaration safety bound";
        return false;
    }

    std::unordered_map<std::uint64_t, std::size_t> selected_by_id;
    selected_by_id.reserve(input.selected_actors->size());
    for (std::size_t i = 0; i < input.selected_actors->size(); ++i) {
        const auto& selected = (*input.selected_actors)[i];
        if (!selected_by_id.emplace(selected.definition.stableId, i).second) {
            error = "Selected population contains duplicate stable IDs";
            return false;
        }
    }

    std::vector<SourcePopulationPlanRowV1> planned;
    planned.reserve(input.definitions->size());
    for (std::size_t i = 0; i < input.definitions->size(); ++i) {
        const auto& definition = (*input.definitions)[i];
        SourcePopulationPlanRowV1 row;
        row.definition_index = i;
        row.stable_id = definition.stableId;
        row.source_id = definition.sourceId;
        row.authored_name = definition.name;
        row.authored_placement = definition.placement;

        const auto decision = input.activation_decision(definition);
        row.status = status_for(decision);
        if (decision != PopulationDecision::include) {
            row.reason = decision == PopulationDecision::exclude ? "caller excluded source activation" :
                         decision == PopulationDecision::deferred ? "source activation is deferred" :
                         "source activation/condition is unknown";
            planned.push_back(std::move(row));
            continue;
        }

        const auto selected = selected_by_id.find(definition.stableId);
        if (selected == selected_by_id.end()) {
            row.status = SourcePopulationPlanStatusV1::no_selected_population_actor;
            row.reason = "caller admitted declaration, but ActorPopulation has no selected row for this stable ID";
            planned.push_back(std::move(row));
            continue;
        }
        const auto& population_actor = (*input.selected_actors)[selected->second];
        if (population_actor.definition.sourceId != definition.sourceId) {
            error = "Stable ID matched a different authored source declaration";
            return false;
        }
        row.selected_population_index = selected->second;
        row.selected_profile_id = population_actor.profileId;
        row.source_character_cache = population_actor.source_character_cache;
        row.source_template_cache = population_actor.source_template_cache;

        SourcePopulationSpawnContextV1 context;
        if (!input.bind_spawn_context(definition, context, error)) {
            if (error.empty()) error = "Could not borrow source spawn context for " + definition.sourceId;
            else error = definition.sourceId + ": " + error;
            return false;
        }
        if (!authored_probability(definition, context.probability, error)) return false;

        auto cached_roll = context.cached_roll;
        auto probability = context.probability;
        auto network_id = context.network_id;
        auto online_owner = context.online_owner;
        auto byte82 = context.byte82;
        bool visibility_requested = false;
        bool delete_requested = false;
        bool mark_requested = false;

        dh2::world::GameObjectSpawnProbabilityBorrowV1 borrow;
        borrow.owner = std::move(context.owner);
        borrow.cached_roll270 = &cached_roll;
        borrow.probability274 = &probability;
        borrow.network_id108 = &network_id;
        borrow.online_owner_fc = &online_owner;
        borrow.byte82 = &byte82;
        borrow.random0 = context.random0;
        borrow.random1 = context.random1;
        borrow.handle_as_player_character = [&context](bool& value, std::string&) {
            value = context.is_player;
            return true;
        };
        borrow.online_byte5 = [&context](bool& value, std::string&) {
            value = context.online;
            return true;
        };
        // CheckSpawnProbability's failure path normally mutates source visibility,
        // deletion and manager state. A planner reports those requested effects;
        // actual execution remains with the owner of the source object lifecycle.
        borrow.set_visible_false = [&visibility_requested](std::string&) {
            visibility_requested = true;
            return true;
        };
        borrow.object_base_delete = [&delete_requested](std::string&) {
            delete_requested = true;
            return true;
        };
        borrow.mark_for_deletion = [&mark_requested](std::string&) {
            mark_requested = true;
            return true;
        };

        const bool alternate = context.online && network_id != -1 && online_owner != 0;
        auto* used_rng = alternate ? context.random1 : context.random0;
        row.roll_cache_before = context.cached_roll;
        row.probability = probability;
        row.rng_calls_before = used_rng ? used_rng->calls : 0;
        std::int32_t roll = 0;
        if (!dh2::world::game_object_check_spawn_probability_v1(borrow, roll, probability, error)) {
            if (error.empty()) error = "Canonical source spawn probability failed for " + definition.sourceId;
            else error = definition.sourceId + ": " + error;
            return false;
        }

        row.decision_roll = roll;
        row.roll_cache_after = cached_roll;
        row.rng_calls_after = used_rng ? used_rng->calls : 0;
        row.failed_spawn_visibility_requested = visibility_requested;
        row.failed_spawn_delete_requested = delete_requested;
        row.failed_spawn_mark_requested = mark_requested;
        const bool passed = roll == -2 || roll < probability;
        row.status = passed ? SourcePopulationPlanStatusV1::include
                            : SourcePopulationPlanStatusV1::exclude;
        row.reason = passed ? "source caller admission passed: roll < signed spawn probability (or -2 sentinel)"
                            : "source caller admission failed: roll >= signed spawn probability; lifecycle effects recorded only";
        planned.push_back(std::move(row));
    }
    output.swap(planned);
    error.clear();
    return true;
}

} // namespace dh::foundation::enemy_ai
