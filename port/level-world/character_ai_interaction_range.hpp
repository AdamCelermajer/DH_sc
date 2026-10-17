#pragma once
#include "character_ai_sight.hpp"
#include "character_enemy_retention.hpp"
#include <cstdint>

namespace dh2::character_ai_interaction_range {
using State=character_ai_sight::State;
using Point=character_ai_sight::Point;
using AiRow=character_enemy_retention::AiRow;
struct ObjectFacts { std::uintptr_t identity, interaction_node_2e8; };
enum class Operation : std::uint32_t {
    target_position, interaction_spot, melee_radius, interaction_radius,
    char_ai, interaction_type,
};
struct Request { Operation operation; std::uintptr_t subject, other; };
struct Response {
    const void* view;
    Point point;
    std::uint32_t word;
};
struct Services {
    void* context;
    // Zero success. target_position returns borrowed Point*. interaction_spot
    // implements the genuine GetInteractionSpot call (including lazy byte2ec/
    // node2e8 updates), returning a COPIED point and borrowed ObjectFacts*.
    // char_ai returns borrowed AiRow* from genuine GetCharAI. Radius queries
    // return raw binary32 words; interaction_radius/type retain virtual94/90.
    // melee_radius subject is the original AI, never a refreshed owner's AI.
    std::int32_t (*invoke)(void*,State*,const Request*,Response*);
};
struct Result {
    std::uint32_t value,calls,distance_squared_word,distance_word,
        owner_radius_word,target_radius_word,remaining_word,threshold_word,
        node_present,interaction_type;
    std::uintptr_t candidate;
};
enum class Status : std::int32_t {
    complete,invalid_argument,service_unavailable,service_failed,invalid_source_fact,
};
Status evaluate_object(State*,std::uintptr_t candidate,const Services*,Result*);

// One owning thread retains original AI, candidate, all fresh owners, returned
// points/ObjectFacts/rows and retired backing through return. Stable identity
// keys cannot change. Providers may change live bindings/backing; the copied
// interaction spot remains fixed. Reentry into this caller for the same State,
// output/services overwrite
// and destruction of borrowed storage are forbidden; independent calls may nest.
// Errors preserve completed effects. No handle, Character conversion, alive or
// visibility predicate is added to this caller. External soft-float imports are
// modeled binary32; NaN classification, not payload propagation, is claimed.
} // namespace dh2::character_ai_interaction_range
