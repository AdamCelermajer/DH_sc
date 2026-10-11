#pragma once
// P17 TRANSITIONS2: what a level transition keeps on disk.
//
// Entry point (original: Character::SG_SetLevelEntryPoint from Level::_LoadPlayer, stored with the character):
//   The character carries the level URI and entrypointID of the last transition arrival, as a named component
//   of the schema v4 tail (CharacterState::components). No schema bump: older saves simply have no component.
// Per-level world state (original: LevelSavegame / Level::QuickSave, kept per level and restored on return):
//   One GameSave snapshot per level, next to the character save: <save stem>.world-<level stem>.gsave.
//   It is written when the player leaves a level (transition) and at checkpoints, and read on arrival.
//
// Nothing here names a level or a position. Paths and ids come from the save and the level being run.

#include "../../character_state.hpp"

#include <cstdint>
#include <filesystem>
#include <string>

namespace dh::foundation::transitions {

inline constexpr const char* entry_component_id = "dh2.transition.entry.v1";

// Records the arrival entry for the level URI (replaces any previous record).
void set_entry_component(CharacterState& state, const std::string& level_uri, int entry_id);
// Reads the record. False when absent or malformed; the outputs are then untouched.
bool entry_component(const CharacterState& state, std::string& level_uri, int& entry_id);
void clear_entry_component(CharacterState& state);

// <dir>/<save stem>.world-<level file stem>.gsave for the level URI (e.g. data/scene/003_darkwood.mlx).
std::filesystem::path level_world_path(const std::filesystem::path& save, const std::string& level_uri);

} // namespace dh::foundation::transitions
