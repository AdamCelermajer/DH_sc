#pragma once
#include "level_preparation_v1.hpp"
#include "gslevel_lifecycle_v2.hpp"
#include "campaign_profile_files_v1.hpp"
#include "player_savegame_v1.hpp"
#include "level_tables.hpp"
#include <functional>
#include <optional>

namespace dh2::loader {
// Real Menu confirmation, NOT TriggerZoneExitLevel::Update's HUD prompt.
// These arguments are supplied by the actual confirmation call site; none of
// the native flags/mode/seeds are inferred from the authored Exit or filename.
enum class AreaTransitionDestinationDomainV114 {raw_filename,level_member_name};
struct AreaTransitionConfirmationV114 {
 std::string destination; // unmodified raw filename OR exact member name
 std::optional<AreaTransitionDestinationDomainV114> destination_domain;
 std::optional<std::int32_t> profile_slot; // original explicit Application argument; literal0 is0
 std::int32_t entry{};
 bool flag_f1{},use_spawn_point{};
 std::int32_t mode{};
 bool bypass_saved_seed{};
 std::uint32_t explicit_seed24{},explicit_seed28{};
};
struct ApplicationLoadLevelArgumentsV114 {
 std::string filename;
 std::int32_t entry{},profile_slot{-1};
 bool flag_f1{},use_spawn_point{};
 std::int32_t mode{};
 bool bypass_saved_seed{};
 std::uint32_t seed24{},seed28{};
};
// Main supplies genuine current-profile serialized bytes, after its real Save
// operation. The independent file owner must retain NO old World/Character,
// live profile/index Borrow, UI session, or containing Level. Keeping a live
// index Borrow would prevent the original unload SaveAllPlayers index reload.
struct AreaTransitionRestoreReceiptV114 {
 std::shared_ptr<const data::CampaignProfileFileV1> file;
 std::string private_directory;
 std::int32_t profile_slot{-1};
 std::uintptr_t selected_character{}; // receipt identity, not retained actor
};
struct AreaTransitionRequestV114 {
 ApplicationLoadLevelArgumentsV114 application;
 LevelSourceRequestV1 source;
 GSLevelArgumentsV2 gs;
 std::int32_t destination_row{-1},source_difficulty{-1};
 AreaTransitionRestoreReceiptV114 restore;
 // Source scalar receipt only. No strong/weak old World needed after projection.
 std::uintptr_t source_character{};
};
struct AreaTransitionProjectionServicesV114 {
 // Only valid during synchronous request formation, before retirement starts.
 // Main's actual current journal/World/Level/player/Save guard, never readiness.
 std::function<bool(std::string&)> current;
 std::function<bool(std::string&)> debug_load;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(std::uint32_t&,std::string&)> real_time;
};
// Synchronous same-owner loans. No Save store, C1, IO, profile read or restore.
// old_level_difficulty is actual current Level40 (Application32bf1c), -1 only
// when the genuine Application caller has no current Level.
bool project_area_transition_request_v114(const AreaTransitionConfirmationV114&,
 const data::LevelTables&,const data::PlayerSavegameV1&,std::uintptr_t actual_character,
 std::int32_t actual_difficulty,std::int32_t old_level_difficulty,
 AreaTransitionRestoreReceiptV114,AreaTransitionProjectionServicesV114,
 std::shared_ptr<const AreaTransitionRequestV114>&,std::string&);
// Exact row+24 description, retained by the caller's actual immutable Tables loan.
// A miss preserves the native -1 result and does not invent a description.
// Exact existing LevelC1 first substring selection: lower row.file only,
// inspect the original caller CString unchanged, stop at first match.
bool borrow_area_transition_filename_level_v114(const data::LevelTables&,const char* filename,
 std::int32_t& row,const data::LevelRecord*& record,std::string&);
bool borrow_area_transition_level_v114(const data::LevelTables&,const char* name,
 std::int32_t& row,const data::LevelRecord*& record,std::string&);
}

