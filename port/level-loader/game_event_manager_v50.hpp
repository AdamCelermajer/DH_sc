#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
namespace dh2::loader {
// Original Structs::v2QuestObjectiveStub44 / v2Event24 semantic rows. Offset
// names preserve opaque values; this is native storage, not an ARM buffer.
struct GameEventObjectiveRowV50 {std::int32_t type4{},field8{},fieldc{},field20{},field24{},field28{};std::string string14,string1c;};
struct GameEventRowV50 {std::int32_t field4{};std::string script_c;std::vector<GameEventObjectiveRowV50> objectives14;};
struct GameEventTablesBorrowV50 {
 std::shared_ptr<const void> actual_owner;
 const std::uint32_t* size{};
 const GameEventRowV50* const* members{};
 const char* const* const* names{};
 std::uint32_t members_bound{},names_bound{};
};
// New original-derived Arrays::v2Events producer: main has no such initialized
// owner yet. Adopt it as that ONE application table owner, or borrow an actual
// already initialized owner via the typed view; do not keep two global tables.
class GameEventTablesV50 final {
 struct Snapshot;std::shared_ptr<Snapshot> snapshot_;
public:
 bool initialize(const std::uint8_t*,std::size_t,const std::uint8_t*,std::size_t,std::string&);
 GameEventTablesBorrowV50 borrow()const noexcept;
};
struct GameEventObjectiveFieldsV50 {
 std::int32_t type4{-1};std::uint8_t byte8{};
 const GameEventObjectiveRowV50* data_c{};std::uintptr_t owner10{};std::uint8_t completed14{};
 // Constructor did NOT write original EventReceiver byte1c. Keep unknown
 // until its real body writes it; never manufacture registered=false here.
 std::optional<std::uint8_t> byte1c;
 std::array<std::optional<std::uint32_t>,4> words20_2c;
};
class GameEventObjectiveV50 final {
 GameEventObjectiveFieldsV50 fields_;std::uint32_t original_size_{};bool saved_quantity_{};
public:
 explicit GameEventObjectiveV50(const GameEventObjectiveRowV50&);
 void reset_data()noexcept;
 auto& fields()noexcept{return fields_;}const auto& fields()const noexcept{return fields_;}
 std::uint32_t original_size()const noexcept{return original_size_;}
};
struct GameEventFieldsV50 {
 std::int32_t state0{-1},id4{-1};const char* name8{};
 std::int32_t objective_count_c{};const GameEventObjectiveRowV50* objective_data14{};
 std::uint8_t registered18{};const GameEventRowV50* data1c{};
};
// GameEvent has no source vptr. Objective behaviors/Compile/Register/quest
// effects remain required main services over these SAME retained fields.
// Actual Objective derived D1/IEventReceiver teardown. The loader performs
// native D0 storage free only AFTER this functional body succeeds. Provider
// must remove SAME event/inventory observer aliases before port storage drops.
struct GameEventNativeDestructionV1 {
 std::shared_ptr<void> owner;
 // The leaf must not delete this borrowed port allocation itself: the
 // loader D0 continuation owns its operator-delete/slot-NULL operation.
 std::function<bool(GameEventObjectiveV50&,std::string&)> objective_d1;
};
class GameEventV50 final {
 friend class GameEventManagerV50;
 GameEventFieldsV50 fields_;std::vector<std::unique_ptr<GameEventObjectiveV50>> objectives_;
 bool destruction_attempted_v1_{},destruction_complete_v1_{};std::string destruction_failure_v1_;
public:
 const auto& fields()const noexcept{return fields_;}auto& fields()noexcept{return fields_;}
 const auto& objectives()const noexcept{return objectives_;}
 bool reinit_storage(std::string& error);
 bool destroy_native_storage_v1(const GameEventNativeDestructionV1&,std::string&);
 bool native_destruction_started_v1()const noexcept{return destruction_attempted_v1_;}
};
enum class GameEventLoadStatusV50 {pending,complete,failed};
enum class GameEventStorageAllocationV50 {manager_slots,event32,objective_slots,objective};
struct GameEventStoragePolicyV50 {
 std::shared_ptr<void> owner;
 // Optional modern allocation budget/diagnostic guard only, NOT an original
 // constructor or quest service. A false result preserves the reached prefix.
 std::function<bool(GameEventStorageAllocationV50,std::uint32_t,std::string&)> allow;
};
struct GameEventLoadDiagnosticsV50 {std::uint32_t slots{},published{},objective_constructors{},objective_resets{};bool storage_load_complete{};std::string error;};
// Distinct original GameEventManager12, not either EventManagerOwnerV12.
// This owns the sole event vector/receivers and pins their actual table rows.
// load_step is bounded: at most one objective construction/reset per call.
class GameEventManagerV50 final {
 enum class Phase {idle,event,objective,reset,publish,complete,failed};
 Phase phase_{Phase::idle};bool busy_{};
 GameEventTablesBorrowV50 tables_;GameEventStoragePolicyV50 policy_;
 std::vector<std::unique_ptr<GameEventV50>> events_;std::unique_ptr<GameEventV50> pending_;
 std::uint32_t cursor_{},objective_cursor_{};GameEventLoadDiagnosticsV50 diagnostics_;
 bool destruction_attempted_v1_{},destruction_complete_v1_{};std::string destruction_failure_v1_;
 bool budget(GameEventStorageAllocationV50,std::uint32_t,std::string&);
 GameEventLoadStatusV50 fail(const std::string&);
public:
 explicit GameEventManagerV50(GameEventStoragePolicyV50 p={}):policy_(std::move(p)){}
 GameEventManagerV50(const GameEventManagerV50&)=delete;
 GameEventLoadStatusV50 load_step(GameEventTablesBorrowV50);
 const auto& events()const noexcept{return events_;}const GameEventV50* pending_event()const noexcept{return pending_.get();}
 const GameEventLoadDiagnosticsV50& diagnostics()const noexcept{return diagnostics_;}
 GameEventV50* by_id(std::int32_t)const noexcept;
 bool destroy_native_storage_v1(const GameEventNativeDestructionV1&,std::string&);
 bool native_destruction_started_v1()const noexcept{return destruction_attempted_v1_;}
 // Deliberately absent functional Compile/Register/Update acceptance. Main
 // must attach those actual services to these owners before source stage31.
};
}
