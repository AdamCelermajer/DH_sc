#pragma once
// Per-slot main-menu metadata (CharacterState schema v4): stamping at the save
// points and projection of the slot panel strings. Evidence:
//   NativeGetSaveSlotDetails 0x44aa28 (panel fields), Level::SG_SavePlayer
//   0x3efa54 (SG_SetSaveDate), MenuBase::FS_StartGame 0x4220a0 (fresh profile:
//   save date, levels[CurrentDifficulty]=LevelList row 41, act 1),
//   Level::_LoadProcess case 0x22 (SG_SetLevelId on level load).
#include "../../asset_catalog.hpp"
#include "../../character_state.hpp"
#include "../../../game-data/data.hpp"
#include "../../../game-data/level_tables.hpp"
#include "../character_menu/menu_text.hpp"
#include "../frontend/creation/dynamic_text_bindings.hpp"
#include <string>
#include <string_view>

namespace dh::foundation::menu_metadata {

// LevelList row written by FS_StartGame for a new profile (001_swamp.mlx,
// "The Boglands"); verified against levels_pyarray.bin by the schema tests.
inline constexpr std::int32_t fresh_profile_level_row = 41;

// Decodes the original LevelList from the package assets (data/levels_pyarray.bin,
// levels_pyarraynames.bin, levels_pystructnames.bin).
bool load_level_tables(const AssetCatalog&, dh2::data::LevelTables&, std::string& error);

// Row of the LevelList whose file names level_uri (path with or without the
// "data/scene/" prefix, case-insensitive on the file name). -1 when absent.
std::int32_t find_level_row(const dh2::data::LevelTables&, std::string_view level_uri);

// New profile (FS_StartGame): known=true, save date=now, row 41 and act 1 for
// every difficulty, current/unlocked difficulty Normal.
void initialize_fresh_menu_metadata(CharacterState&, std::uint32_t now);

// Save point (SG_SavePlayer): known=true, save date=now. level_row >= 0 sets
// levels[current_difficulty] (SG_SetLevelId); act > 0 sets the current act of
// the current difficulty (SG_SetCurrentAct). Unknown values are never invented.
void stamp_menu_metadata(CharacterState&, std::uint32_t now, std::int32_t level_row = -1,
                         std::int32_t act = 0);

// Convenience for call sites that know the current level file: resolves the row
// through the LevelList (keeping the stored row when the level is absent there).
void stamp_menu_metadata_for_level(CharacterState&, std::uint32_t now,
                                   const dh2::data::LevelTables*, std::string_view level_uri);

// Sets one visited-module byte keeping visited_modules sorted/unique.
void set_visited_module(CharacterState&, const std::string& level_uri,
                        std::uint32_t module_id, bool visited);

// Fills the metadata part of the slot panel projection from the saved profile
// through engine-ui/menu_save_slot_projection_v1. character_row is the saved
// class' CharacterTable row. Legacy (known=false) saves only get the level text;
// act/location/difficulty/date stay blank. Returns false with an error on any
// invalid row or missing localization.
bool project_slot_presentation(const CharacterState&, std::int32_t slot,
    std::int32_t character_row, const dh2::data::CharacterTable&, const dh2::data::LevelTables&,
    character_menu::MenuLocalization&, std::uint32_t language,
    frontend::creation::SavedProfilePresentation& out, std::string& error);

} // namespace dh::foundation::menu_metadata
