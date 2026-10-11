#pragma once

#include "death_checkpoint_snapshot_v1.hpp"

namespace dh::foundation::persistence::death_flow {

struct DeathCheckpointZoneContactV1 {
    std::int32_t source_level_state = 0; // Level+304: source save gate is >33.
    bool contact_object_zonable = false;
    bool contact_character_is_player = false;
    bool contact_is_local_player_zero = false;
    bool local_character_present = false;
    bool local_character_is_dead = false; // Source Character::IsDead fact; never inferred from HP.
    bool level_savegame_present = false;
    std::string level_uri;
    std::string zone_name;
    std::array<float,3> zone_position{};
    std::array<float,3> current_checkpoint_key{};
    std::array<float,3> current_character_position{};
};

enum class DeathCheckpointZoneResultV1 : std::uint8_t {
    skipped_not_zonable,
    skipped_not_local_player,
    skipped_same_checkpoint_key,
    skipped_level_or_character_gate,
    saved,
    failed
};

struct DeathCheckpointZoneServicesV1 {
    void* context = nullptr;
    // LevelCheckpointSave first records key=zone position and revive position
    // = current Character position (CheckpointZone passes arg3=false).
    bool (*record_checkpoint_positions)(void*, const std::string& zone_name,
        const std::array<float,3>& checkpoint_key,
        const std::array<float,3>& revive_position, std::string&) = nullptr;
    // Character::SG_SaveCheckpoint runs before LevelSavegame::SaveCheckPoint.
    bool (*save_player_qest_checkpoint)(void*, std::string&) = nullptr;
    bool (*capture_current_game_save)(void*, GameSave&, std::string&) = nullptr;
    DeathCheckpointCaptureServicesV1 capture_services{};
    // Persist the existing per-level GameSave and the portable source subset;
    // restoration must later project the subset, never call full F9 restore.
    bool (*save_level_checkpoint)(void*, const GameSave&,
        const DeathCheckpointSnapshotV1&, std::string&) = nullptr;
};

struct DeathCheckpointZoneReceiptV1 {
    DeathCheckpointZoneResultV1 result = DeathCheckpointZoneResultV1::failed;
    DeathCheckpointSnapshotV1 snapshot;
    bool complete = false;
};

// Generic CheckpointZone source gate and call order for any loaded level.
bool run_death_checkpoint_zone_contact_v1(
    const DeathCheckpointZoneContactV1&,
    const DeathCheckpointZoneServicesV1&,
    DeathCheckpointZoneReceiptV1&, std::string& error);

} // namespace dh::foundation::persistence::death_flow
