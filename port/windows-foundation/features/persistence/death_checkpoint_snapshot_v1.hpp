#pragma once

#include "../../game_save.hpp"
#include "../../playable_actor_world.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dh::foundation::persistence::death_flow {

enum class SourceCharacterRestoreOpV1 : std::uint8_t {
    restore_serialized_dead_bit,
    set_limbus_if_not_already,
    set_source_dead_true,
    set_pre_spawn_if_not_already,
    set_hp_zero,
    set_ai_dead,
    can_respawn_gate,
    initialize_physical_if_missing,
    set_idle_zero,
    clear_target
};

struct SourceCharacterRestorePlanV1 {
    std::uint32_t saved_state = 0;
    bool saved_dead = false;
    bool requires_can_respawn = false;
    bool can_respawn_allowed = false;
    bool resulting_dead = false;
    std::vector<SourceCharacterRestoreOpV1> ordered_operations;
};

// Mirrors Character::Deserialize's CS-state branch without shifting by an
// unchecked source state. A required but unavailable CanRespawn answer fails
// closed; it is never inferred from HP or current action.
bool source_character_restore_plan_v1(
    std::uint32_t saved_state, bool saved_dead,
    bool already_limbus, bool already_pre_spawn,
    std::optional<bool> can_respawn,
    bool physical_body_present,
    SourceCharacterRestorePlanV1&, std::string& error);

struct DeathCheckpointActorV1 {
    ActorId id = invalid_actor_id;
    std::string definition_id;
    bool player = false;
    Transform transform;
    std::uint32_t source_cs_state = 0;
    bool source_dead = false;
    std::optional<bool> physical_present;
    bool npc_vitals_present = false;
    float health = 0.0f, max_health = 0.0f;
    float resource = 0.0f, max_resource = 0.0f;
};

struct DeathCheckpointSnapshotV1 {
    std::uint32_t version = 1;
    std::string level_uri;
    std::string checkpoint_name;
    ActorId controlled_actor_id = invalid_actor_id;
    std::array<float, 3> checkpoint_key_position{};
    std::array<float, 3> revive_position{};
    std::vector<DeathCheckpointActorV1> actors;
    std::vector<WorldObject> objects;
    bool qest_present = false;
    std::vector<std::uint8_t> qest;
};

struct DeathCheckpointCaptureServicesV1 {
    void* context = nullptr;
    bool (*actor_checkpoint_enabled)(void*, ActorId, bool&, std::string&) = nullptr;
    bool (*actor_source_state)(void*, ActorId, std::uint32_t& cs_state,
                               bool& source_dead, std::string&) = nullptr;
    bool (*object_checkpoint_enabled)(void*, ObjectId, bool&, std::string&) = nullptr;
};

bool capture_death_checkpoint_snapshot_v1(
    const GameSave&, const std::string& level_uri,
    const std::string& checkpoint_name,
    const std::array<float, 3>& checkpoint_key_position,
    const std::array<float, 3>& revive_position,
    const DeathCheckpointCaptureServicesV1&,
    DeathCheckpointSnapshotV1&, std::string& error);

bool validate_death_checkpoint_snapshot_v1(
    const DeathCheckpointSnapshotV1&, std::string& error);
bool encode_death_checkpoint_snapshot_v1(
    const DeathCheckpointSnapshotV1&, std::vector<std::uint8_t>&,
    std::string& error);
bool decode_death_checkpoint_snapshot_v1(
    const std::vector<std::uint8_t>&, DeathCheckpointSnapshotV1&,
    std::string& error);

struct DeathCheckpointRestoreServicesV1 {
    void* context = nullptr;
    bool (*current_source_states)(void*, ActorId, bool& already_limbus,
                                  bool& already_pre_spawn,
                                  std::optional<bool>& can_respawn,
                                  std::string&) = nullptr;
    // Applies only the source state operations represented by plan to staged
    // ActorState. It must not mutate the live owner or profile/inventory.
    bool (*apply_source_state)(void*, const SourceCharacterRestorePlanV1&,
                               ActorState&, std::string&) = nullptr;
};

struct DeathCheckpointRestoreReceiptV1 {
    std::size_t actors_restored = 0;
    std::size_t actors_missing_skipped = 0;
    std::size_t objects_restored = 0;
    std::size_t objects_missing_skipped = 0;
    bool qest_restored = false;
    bool complete = false;
};

// Prevalidates the complete matching prefix, restores only existing matching
// world identities, leaves absent/unmatched live entries in place, preserves
// RNG and all canonical profile fields except the QEST blob, and never creates
// or deletes an actor/object. Existing GameSave remains the portable input.
bool restore_death_checkpoint_snapshot_v1(
    const DeathCheckpointSnapshotV1&, const std::string& expected_level_uri,
    PlayableActorWorld&, CharacterState&,
    const DeathCheckpointRestoreServicesV1&,
    DeathCheckpointRestoreReceiptV1&, std::string& error);

} // namespace dh::foundation::persistence::death_flow
