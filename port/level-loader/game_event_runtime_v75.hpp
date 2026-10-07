#pragma once
#include "game_event_manager_v50.hpp"
#include "../level-world/event_manager_owner_v12.hpp"
#include <map>
namespace dh2::loader {
// Native typed views of the original QuestEvent fields. Projection must prove
// the actual payload family; an arbitrary EventBorrow.context is never cast.
struct GameEventQuestBorrowV75 {
 std::shared_ptr<void> receiver;
 std::uintptr_t character8{},object18{};
 std::int32_t id18{};
 std::uint8_t* pending_network10{};
 const std::uint8_t* from_network11{};
 std::int32_t* quantity14{};
 const std::uintptr_t* character8_cell{};
 const std::uintptr_t* object18_cell{};
 const std::int32_t* id18_cell{};
};
// Synchronous stack bridge over an ALREADY produced quest event. It owns no
// payload, clone, queued event or alternate field backing. Every pointer and
// identity comes from the caller's actual source family constructor/storage.
class ScopedGameQuestEventV75 final {
 std::uintptr_t identity_{};
 const std::int32_t* type4_{};
 GameEventQuestBorrowV75 fields_;
 static bool type(void*,std::int32_t&,std::string&);
 friend bool project_scoped_game_quest_event_v75(const events::EventBorrowV12&,GameEventQuestBorrowV75&,std::string&);
public:
 ScopedGameQuestEventV75(std::uintptr_t,const std::int32_t*,GameEventQuestBorrowV75);
 ScopedGameQuestEventV75(const ScopedGameQuestEventV75&)=delete;
 events::EventBorrowV12 borrow()noexcept{return {identity_,this,type,fields_.receiver};}
};
bool project_scoped_game_quest_event_v75(const events::EventBorrowV12&,GameEventQuestBorrowV75&,std::string&);
struct GameEventObjectBorrowV75 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
};
// Nonowning scalar view. Saved Quest quantity20 is signed native storage;
// GameEvent derived words retain C1 provenance as optional uint32 cells.
// memcpy preserves original word bits without aliasing either representation.
class ObjectiveWordBorrowV75 {
 std::int32_t* saved_{};std::optional<std::uint32_t>* constructed_{};
public:
 ObjectiveWordBorrowV75()=default;
 explicit ObjectiveWordBorrowV75(std::int32_t* saved):saved_(saved){}
 explicit ObjectiveWordBorrowV75(std::optional<std::uint32_t>* constructed):constructed_(constructed){}
 explicit operator bool()const noexcept{return saved_||(constructed_&&constructed_->has_value());}
 std::uint32_t operator*()const;
 ObjectiveWordBorrowV75& operator=(std::uint32_t);
};
struct ObjectiveBorrowV75 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 std::int32_t& type4;
 std::uint8_t& byte8;
 const GameEventObjectiveRowV50* data_c{};
 std::uintptr_t& owner10;
 std::uint8_t& completed14;
 std::optional<std::uint8_t>& byte1c;
 std::array<ObjectiveWordBorrowV75,4> words20_2c;
};
ObjectiveBorrowV75 borrow_game_event_objective_v75(const std::shared_ptr<GameEventManagerV50>&,GameEventObjectiveV50&);
struct GameEventRuntimeServicesV75 {
 std::shared_ptr<void> provider;
 // Borrow the actual current Level inherited EventManager and source row3c.
 // No parallel event dispatcher, Level194 replacement, or source clock.
 std::function<bool(std::shared_ptr<void>&,events::EventManagerOwnerV12*&,const std::int32_t*&,std::string&)> current_level;
 std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
 std::function<bool(std::int32_t&,std::string&)> common_script_count;
 std::function<bool(const char*,std::int32_t&,std::string&)> script_id;
 std::function<bool(std::int32_t,std::int32_t,bool,std::string&)> start_script;
 std::function<bool(bool,std::int32_t,std::int32_t&,std::string&)> enemies_loaded;
 std::function<bool(const char*,GameEventObjectBorrowV75&,std::string&)> zone_by_name;
 std::function<bool(std::int32_t,std::uint8_t,std::string&)> talk_flag;
 std::function<bool(std::uintptr_t,std::int32_t,bool&,std::int16_t&,std::string&)> inventory_quantity;
 std::function<bool(std::uintptr_t,std::int32_t,bool,std::string&)> inventory_register;
 std::function<bool(const events::EventBorrowV12&,GameEventQuestBorrowV75&,std::string&)> project_event;
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> character_props;
 std::function<bool(std::uintptr_t,bool&,std::string&)> character_is_player;
 std::function<bool(const GameEventQuestBorrowV75&,std::string&)> send_network;
 // Nonordinary assertion modes are owned by the process diagnostic owner.
 std::function<bool(std::int32_t,const char*,std::string&)> assertion;
};
// Whole GameEvent/ObjectiveList control flow over V50's SAME stored objects.
// All thirteen source table constructors share this runtime. Mechanic queries
// remain real typed services; unsupported positive transport fails at reach.
class GameEventRuntimeV75 final:public std::enable_shared_from_this<GameEventRuntimeV75> {
 struct Receiver;
 std::shared_ptr<GameEventManagerV50> manager_;
 GameEventRuntimeServicesV75 services_;
 std::map<std::uintptr_t,GameEventObjectBorrowV75> zones_;
 std::map<std::uintptr_t,std::shared_ptr<Receiver>> receivers_;
 bool busy_{},failed_{},compiled_{},native_retiring_v88_{};std::string failure_;
 bool fail(const std::string&,std::string&);
 bool current(std::shared_ptr<void>&,events::EventManagerOwnerV12*&,const std::int32_t*&,std::string&);
 bool on_event(ObjectiveBorrowV75,const events::EventBorrowV12&,std::int32_t&,std::string&);
 bool compile_event(GameEventV50&,std::string&);
 bool update_event(GameEventV50&,std::string&);
 bool set_state(GameEventV50&,std::int32_t,std::string&);
public:
 explicit GameEventRuntimeV75(GameEventRuntimeServicesV75);
 GameEventRuntimeV75(std::shared_ptr<GameEventManagerV50>,GameEventRuntimeServicesV75);
 bool attach_manager_v75(std::shared_ptr<GameEventManagerV50>,std::string&);
 // Shared kernels operate borrowed SAME saved fields and can be used by
 // Quest.Compile independently of manager-wide event Compile readiness.
 bool invalidate_objective(ObjectiveBorrowV75,std::string&);
 bool complete(ObjectiveBorrowV75,std::string&);
 bool compile_objective(ObjectiveBorrowV75,std::string&);
 bool register_objective(ObjectiveBorrowV75,bool,std::string&);
 bool compile(std::string&);
 bool update(std::string&);
 bool reinit(std::string&);
 bool unregister_objectives(std::string&);
 //Actual Objective D1 has only base-vptr stores and BXLR. This retires the
 //native dispatcher adapters under quiescence before loader frees storage;
 //it deliberately invokes no gameplay Unregister/talk/inventory/marker leaf.
 bool destroy_objective_native_v88(const std::shared_ptr<GameEventManagerV50>& actual_storage,GameEventObjectiveV50&,std::string&);
 //Same saved Quest fields, independent of Level194 storage. Native observer
 //detachment only: original Objective D1 has no gameplay marker/flag effects.
 bool destroy_objective_aliases_v108(ObjectiveBorrowV75,std::string&);
 const std::shared_ptr<GameEventManagerV50>& manager()const noexcept{return manager_;}
 bool compiled()const noexcept{return compiled_;}
 bool failed()const noexcept{return failed_;}
};
}
