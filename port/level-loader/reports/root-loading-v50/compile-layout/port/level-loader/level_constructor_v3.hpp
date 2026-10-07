#pragma once
#include "../game-data/level_tables.hpp"
#include "../level-world/event_manager_owner_v12.hpp"
#include <functional>
#include <memory>
#include <string>

namespace dh2::loader {
// Native semantic fields written by LevelC1, beyond the existing context's
// config/sound/gate/module fields. Offset suffixes identify source stores;
// this is not an ARM object layout or emulated memory buffer.
struct LevelConstructorFieldsV3 {
 std::unique_ptr<events::EventManagerOwnerV12> events;
 std::shared_ptr<void> script44,save_ec;
 std::uint32_t phase30{},party_dc{},word_e0{},phase_e4{};
 std::int32_t row3c{-1},difficulty40{-1};
 std::string name_f8;
 std::uint8_t byte_e8{},byte_f0{},byte_f1{},byte_f2{},byte_f3{},byte_f4{},byte_f5{};
 std::int32_t level110{},mode118{};std::uint32_t seed114{};
 std::uintptr_t field128{},field12c{};
 std::uint32_t field130{}; // source Level::_LoadProcess state, not a pointer
 std::uint32_t field134{},field138{}; // source scalar counters; signed comparison in progress tail
 std::uint32_t field13c{}; // source file-count/index scalar, ADD32 in state7
 std::uintptr_t field140{}; // actual LoadFileData stream-state pointer
 std::uint8_t byte144{},byte145{};std::int32_t field148{-1};
 std::uintptr_t field14c{},field154{},field158{},field15c{},field194{},field19c{},field1a0{},field1a4{};
 std::uint8_t byte16c{},byte198{},byte1a8{};
};
struct LevelConstructorArgumentsV3 {
 // Retains the same selected CString through constructor helpers.
 const char* name{};std::int32_t level{};std::uint32_t seed{},party{},word_e0{};
 std::uint8_t byte_f1{},byte_f2{};std::int32_t requested_difficulty{},mode{};
};
struct LevelConstructorBorrowV3 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 LevelConstructorFieldsV3* fields{};
 // SAME existing CanonicalLevelContext writable cells; no second gate150.
 std::uintptr_t* config38{};
 std::int32_t* music11c{};std::int32_t* safezone120{};std::int32_t* ambient124{};
 std::uint32_t* gate150{};
 float* module_offset160{};std::int32_t* module_id18c{};
};
struct LevelSaveConstructionV3 {
 std::uintptr_t level_identity{};
 std::uint32_t seed{};std::int32_t difficulty{},row{},mode{};
 bool source_flag{};
};
struct LevelConstructorServicesV3 {
 std::shared_ptr<void> application;
 // Source static debug counter and SAME Module::s_moduleId field. Constructor
 // increments the first and resets the second only at their reached stores.
 std::uint32_t* debug_level_load_count{};
 std::uint32_t* module_id_global{};
 // Exact immutable Arrays::LevelList owner used by this application.
 const data::LevelTables* levels{};
 std::function<bool(std::uintptr_t,bool,std::shared_ptr<void>&,std::string&)> construct_script;
 // Source Level+ac is embedded LuaScript44's path, not another Level string.
 // Assign through that SAME retained script owner, which owns the sole path.
 std::function<bool(const std::shared_ptr<void>&,const char*,std::size_t,std::string&)> script_assign_path;
 std::function<bool(const std::shared_ptr<void>&,const char*,std::string&)> script_load;
 std::function<bool(std::uint8_t&,std::string&)> online_byte5;
 std::function<bool(bool&,std::string&)> local_player_hosting;
 std::function<bool(std::uint8_t&,std::string&)> player_manager_byte719;
 std::function<bool(std::int32_t&,std::string&)> online_state34;
 std::function<bool(bool&,std::string&)> matching_is_host;
 // Allocation precedes field reads in the original. Keep these separate so
 // allocation side effects cannot construct a save from stale Level fields.
 // The returned lease is the actual unconstructed native Save owner/storage.
 std::function<bool(std::shared_ptr<void>&,std::string&)> allocate_save;
 // C1 on that SAME allocation and Level; publication into save_ec occurs only
 // after completion. A failed constructor never publishes a dummy receiver.
 std::function<bool(const std::shared_ptr<void>&,const LevelSaveConstructionV3&,std::string&)> construct_save;
};
enum class LevelConstructorPhaseV3 {
 idle,event_manager,script_constructor,script_path,first_script,second_script,
 module_reset,level_lookup,online,save_allocation,save_constructor,complete,failed
};
class LevelConstructorV3 {
 LevelConstructorBorrowV3 borrow_;LevelConstructorServicesV3 services_;
 LevelConstructorPhaseV3 phase_{LevelConstructorPhaseV3::idle},failed_at_{LevelConstructorPhaseV3::idle};
 bool attempted_{},busy_{};std::string error_;
 bool fail(const char*);
public:
 LevelConstructorV3(LevelConstructorBorrowV3 b,LevelConstructorServicesV3 s):borrow_(std::move(b)),services_(std::move(s)){}
 LevelConstructorV3(const LevelConstructorV3&)=delete;
 LevelConstructorV3& operator=(const LevelConstructorV3&)=delete;
 bool construct(LevelConstructorArgumentsV3);
 LevelConstructorPhaseV3 phase()const noexcept{return phase_;}
 LevelConstructorPhaseV3 failed_at()const noexcept{return failed_at_;}
 const std::string& error()const noexcept{return error_;}
};
}
