#pragma once
#include "level_savegame_cache_v1.hpp"
#include "level_savegame_objects_v2.hpp"
#include "savegame_jobs_owner_v2.hpp"
namespace dh2::level {
struct SavegameSectionWriterV2 {std::array<std::uint8_t,4>tag{};void* context{};bool(*write)(void*,SavegameStreamV2&,std::string&){};};
bool savegame_build_frame_v2(const data::PlayerProfileIndexV1::Borrow&,
 const std::vector<SavegameSectionWriterV2>&,std::unique_ptr<SavegameStreamV2>&,std::string&);
// Whole reached non-raw Savegame.saveAll over registered INFO/OBJS and cached
// unknown sections. The one Application-global jobs owner must be supplied.
bool level_savegame_save_all_v2(LevelSavegameCacheV1&,LevelSavegameFieldsV1&,
 const LevelSaveObjectsServicesV2&,SavegameJobsOwnerV2&,std::string&);
}
