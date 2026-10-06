#pragma once
#include "character_level.hpp"
namespace dh2::character {
// Projections of PlayerInfo+330 and Level+3c/+118. Their genuine manager,
// Application and session owners are supplied; no default player is invented.
struct HostPlayer8 {std::int32_t cached_level;std::uint32_t reserved;};
struct HostLevel8 {std::int32_t row_index,difficulty;};
// Original 72-byte Level row+30..47. Values are signed integer level words.
struct LevelRangeRow24 {std::int32_t maximum[3],minimum[3];};
enum HostContextOperation : std::uint32_t {host_player_level=0,host_player_difficulty=1,host_level_range=2};
enum HostContextService : std::uint32_t {host_get_player=0,host_get_current_level=1,host_get_range_rows=2,host_push_integer=3};
struct HostContextRequest16 {std::uint32_t service,operation;std::int32_t value;std::uint32_t reserved;};
struct HostContextResponse16 {const void* data;std::uint32_t count,reserved;};
struct HostContextServices16 {
 void* context;
 // Delivery0; nonzero is native/provider failure, never a source Lua value.
 // Player/current-level requests return borrowed projections; rows returns
 // the current array/count. Push receives the signed integer in request.value.
 // Rows are queried again after the first push. Receivers/backing survive all
 // synchronous calls. No same-VM reentry or exceptions through this service.
 int(*invoke)(void*,const HostContextRequest16*,HostContextResponse16*);
};
struct HostContextBindings16 {HostContextServices16 services;};
static_assert(sizeof(HostPlayer8)==8&&sizeof(HostLevel8)==8&&sizeof(LevelRangeRow24)==24);
static_assert(sizeof(HostContextRequest16)==16&&sizeof(HostContextResponse16)==16&&sizeof(HostContextServices16)==16&&sizeof(HostContextBindings16)==16);
}
// Source wrappers. 1 completed (possibly zero pushes), -1 malformed before
// effects, -2 missing/provider/data failure after delivered prefixes. Range
// row=-1 pushes(-1,-1); a missing Level is failure, matching no source null gate.
// Only first NUMBER chooses tier via original signed f2iz; all extras ignored.
extern "C" int dh2_character_host_context_query(std::uint32_t,
 const dh2_script_value*,std::uint32_t,const dh2::character::HostContextServices16*);
// Cached-level write projection for the live _ManageCharacters producer. Caller
// selects when to sync; its comparison/repeated reads and netInteger publishing
// remain outside this helper. Does not replace cache reads in the global.
// 0 success,1 malformed; failed GetLevel preserves the cache.
extern "C" int dh2_character_host_context_sync_level(dh2::character::HostPlayer8*,const dh2::data::PropertyView*);
extern "C" int dh2_character_host_context_bind(dh2_script_vm*,const dh2::character::HostContextBindings16*);
