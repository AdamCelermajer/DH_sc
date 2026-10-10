#pragma once

#include "../../actor_population.hpp"
#include "../../../level-world/game_object_spawn_probability_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::enemy_ai {

// Facts borrowed from the same source GameObject and application RNG that
// owns CheckSpawnProbability. The planner snapshots the mutable roll/byte
// fields so a plan can be inspected without changing an actor or manager.
struct SourcePopulationSpawnContextV1 {
    std::shared_ptr<void> owner;
    std::int32_t cached_roll = -1;
    std::int32_t probability = 0;
    std::int32_t network_id = -1;
    std::int32_t online_owner = 0;
    std::uint8_t byte82 = 0;
    bool is_player = false;
    bool online = false;
    dh2::data::LootRandom8V2* random0 = nullptr;
    dh2::data::LootRandom8V2* random1 = nullptr;
};

enum class SourcePopulationPlanStatusV1 {
    include,
    exclude,
    unknown,
    deferred,
    no_selected_population_actor
};

struct SourcePopulationPlanRowV1 {
    std::size_t definition_index = 0;
    std::size_t selected_population_index = static_cast<std::size_t>(-1);
    std::uint64_t stable_id = 0;
    std::string source_id;
    std::string authored_name;
    Mat4 authored_placement{};
    std::string selected_profile_id;
    std::int16_t source_character_cache = -1;
    std::int16_t source_template_cache = -1;
    SourcePopulationPlanStatusV1 status = SourcePopulationPlanStatusV1::unknown;
    std::string reason;
    std::int32_t probability = 0;
    std::int32_t roll_cache_before = -1;
    std::int32_t decision_roll = -1;
    std::int32_t roll_cache_after = -1;
    std::uint32_t rng_calls_before = 0;
    std::uint32_t rng_calls_after = 0;
    bool failed_spawn_visibility_requested = false;
    bool failed_spawn_delete_requested = false;
    bool failed_spawn_mark_requested = false;
};

struct SourcePopulationPlannerInputV1 {
    const std::vector<ActorDefinition>* definitions = nullptr;
    const std::vector<PopulationActor>* selected_actors = nullptr;
    // Must report the actual source activation/condition result. Unknown and
    // deferred are kept out of the plan; no conditions are guessed here.
    std::function<PopulationDecision(const ActorDefinition&)> activation_decision;
    // Supplies borrowed fields and the shared application RNG for this exact
    // source object. RNG pointers are never created or seeded by the planner.
    std::function<bool(const ActorDefinition&, SourcePopulationSpawnContextV1&,
                       std::string&)> bind_spawn_context;
};

// Builds a source-preserving plan only for caller-admitted, already selected
// population rows. It invokes the existing canonical GameObject decision
// kernel, but diverts its failed-spawn lifecycle writes into diagnostics.
// The source RNG draw is real and intentionally consumes the caller's stream;
// roll cache/byte82 changes are returned as plan data for the actual owner to
// commit when it executes the corresponding source lifecycle.
bool plan_source_population_v1(const SourcePopulationPlannerInputV1&,
                               std::vector<SourcePopulationPlanRowV1>& output,
                               std::string& error);

} // namespace dh::foundation::enemy_ai
