#pragma once
#include "player_savegame_v1.hpp"
#include <string>

namespace dh2::data {struct LevelTables;struct WorldMapTables;}
namespace dh2::data::player_saved_level_states_v1 {
// A captured source name-array borrow. The callback reads a row from this
// retained backing, not a second authoritative table or a copied name list.
struct NameArray {
 const void* backing=nullptr;
 const char* (*name)(const void*,std::uint32_t)=nullptr;
};
enum class Assertion : std::uint32_t {negative_id,large_id,negative_state,large_state};
struct AssertRequest {
 SavedStateTableV1 table{};Assertion condition{};
 std::int32_t id=0,state=0,difficulty=0;
 std::uint32_t source_caller=0,source_line=0;
};
struct Services {
 void* context=nullptr;
 bool (*count)(void*,SavedStateTableV1,std::uint32_t*,std::string&)=nullptr;
 bool (*names)(void*,SavedStateTableV1,NameArray*,std::string&)=nullptr;
 // The original rereads its assertion-mode global only after a failed test.
 bool (*assert_mode)(void*,std::int32_t*,std::string&)=nullptr;
 bool (*log_assert)(void*,const AssertRequest&,std::string&)=nullptr;
};
struct Bindings {PlayerSavegameV1* save=nullptr;Services services{};};
// Borrow the actual selected table owners and source assertion global. This
// adapter stores no names/counts; source lookups capture each table's real rows.
struct TableBindings {
 const LevelTables* levels=nullptr;const WorldMapTables* world_map=nullptr;
 const std::int32_t* assertion_mode=nullptr;
 void* logger_context=nullptr;
 bool (*log_assert)(void*,const AssertRequest&,std::string&)=nullptr;
};
Services table_services(TableBindings&) noexcept;
enum class Stage : std::uint32_t {not_started,count,name,resolve,value,set_state,complete};
struct Result {
 Stage stage=Stage::not_started;
 std::uint32_t source_caller=0,consumed=0,read_calls=0,string_reads=0,
 table_counts=0,name_arrays=0,name_comparisons=0,assert_mode_reads=0,
 assert_logs=0,stores=0,completed_entries=0,table=0,difficulty=0;
 std::int32_t declared_entries=0,resolved_id=-1,last_state=0;
};
enum class Status {complete,invalid_argument,busy,failed};
class Runtime {
 Bindings bindings_;bool busy_=false;
public:
 explicit Runtime(Bindings);
 Status load(Bytes,Result*,std::string&);
 // Whole SG_SetLevelState/SG_SetMapLocState callers over the same six Save
 // arrays. Explicit calls preserve source assertion/log/store ordering.
 Status set_state(SavedStateTableV1,std::int32_t id,std::int32_t state,
                  std::int32_t difficulty,Result*,std::string&);
};
// Whole __LoadLevelStates648B@46a9cc and both 504B setter bodies. Reads three
// LevelList groups followed by three WorldMap groups, consuming values even
// for unknown names. Name/count globals and assertion logging remain mandatory
// borrowed services when reached. Setters use existing Save arrays only.
// No Save, profile, level/map table, property, inventory, VM or timer owner.
// Malformed streams and source unsafe pointers/stores fail at their reached
// prefix; delivered stores remain. Native bounded spans, callback exceptions
// and reentry are additional policies. Array/control/input lifetimes remain
// stable on one owning thread; callbacks must not destroy or rebind this Runtime.
}
