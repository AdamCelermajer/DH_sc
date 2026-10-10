#pragma once

#include "character_state.hpp"
#include "world.hpp"
#include <optional>

namespace dh::foundation {

using ActorId = ObjectId;
inline constexpr ActorId invalid_actor_id = invalid_object_id;

struct ActorEquipmentReference {
    std::string slot;
    std::string definition_id;
    // NPC equipment can have a definition without a persistent inventory item.
    std::optional<std::string> item_instance_id;
};

// Shared live data for any character. Input controllers and AI operate on this
// same structure; neither ownership nor faction implies player-specific rules.
// Zero vitals represent an unbound actor, never recovered balance defaults.
struct ActorState {
    ActorId id = invalid_actor_id;
    std::string definition_id;
    std::string class_id;
    // Original faction tables use signed IDs. -1 denotes no bound faction.
    std::int32_t faction_id = -1;
    Transform transform;
    float health = 0.0f;
    float max_health = 0.0f;
    float resource = 0.0f;
    float max_resource = 0.0f;
    CharacterAction action = CharacterAction::idle;
    float action_elapsed_seconds = 0.0f;
    ActorId target_id = invalid_actor_id;
    std::vector<ActorEquipmentReference> equipment;
    std::vector<std::string> attack_ids;
    std::optional<std::string> persistent_character_id;
    // Source motion cells are populated by explicit constructor/focus effects.
    // They are transient and unknown until a source producer runs.
    std::optional<std::uint32_t> source_flags520;
    std::optional<std::uint32_t> source_movement_type;
    std::optional<std::uint32_t> source_validate_boundary452;
    // Own authored scene target node/cache; unrelated to combat target_id.
    // Node tokens are valid only for the currently bound visual hierarchy.
    std::optional<std::uintptr_t> source_target_node180;
    std::optional<std::array<float,3>> source_target_position184;
    // Actual host-owned physical assignment, independent of HP/action/render.
    // Unknown until body assembly publishes its result. Persist as a value;
    // physical receivers, pointers and pool ownership remain transient.
    std::optional<bool> source_physical_present;

    bool alive() const noexcept;
};

// Definition IDs and world IDs must be bound before validation succeeds.
// This checks structural consistency, not recovered faction/content existence.
bool validate_actor_state(const ActorState& actor, std::string& error);
void reset_actor_action(ActorState& actor, CharacterAction action) noexcept;
// Returns actual health removed. Invalid/nonpositive amounts are ignored.
// Resistance, attack eligibility and event timing belong to the combat system.
float apply_actor_damage(ActorState& actor, float amount) noexcept;
// Health prefix for a same-owner runtime that publishes the admitted death
// transition synchronously afterwards. Keeps outgoing action/target/elapsed
// available to Blur; zero health already makes the actor ineligible as a target.
float apply_actor_damage_health_prefix(ActorState& actor,float amount)noexcept;

} // namespace dh::foundation
