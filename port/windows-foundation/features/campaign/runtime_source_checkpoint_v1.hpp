#pragma once

#include "../../game_save.hpp"
#include <filesystem>

namespace dh::foundation::campaign {

// Source identity supplied by the existing LevelList/area-transition owner
// after it has admitted a real authored transition. destination_level_uri is
// the exact resolved definition currently owned by GameSave::level_uri.
// This descriptor is a caller proof, not additional save data.
struct SourceTransitionV1 {
    std::string source_module;
    std::string transition_object;
    std::string destination_member;
    std::string destination_level_uri;
    std::int32_t entrypoint_id = -1;
};

struct RuntimeSourceCheckpointReceiptV1 {
    // These source route fields describe the caller's accepted route. They are
    // not encoded by GameSave; only level_uri and its existing canonical data
    // are durable in this adapter.
    std::string source_module;
    std::string transition_object;
    std::string destination_member;
    std::string level_uri;
    std::string character_id;
    std::int32_t entrypoint_id = -1;
    bool persisted = false;
    bool source_entrypoint_persisted = false;
};

// Persist the existing canonical GameSave only after the actual transition's
// destination is the current checkpoint. No new fields or source Door state
// are encoded; SaveStore remains the sole file/checksum/atomic-replace owner.
bool save_runtime_source_checkpoint_v1(
    const std::filesystem::path&, const SourceTransitionV1&, const GameSave&,
    RuntimeSourceCheckpointReceiptV1&, std::string& error);

// Reload into a detached candidate, verify it still names the same admitted
// transition destination and publish only after all checks succeed.
bool load_runtime_source_checkpoint_v1(
    const std::filesystem::path&, const SourceTransitionV1&, GameSave&,
    RuntimeSourceCheckpointReceiptV1&, std::string& error);

} // namespace dh::foundation::campaign
