#pragma once
#include "quest_savegame_v1.hpp"
#include <cstdint>

namespace dh2::data::player_saved_quests_v1 {struct StreamRef;}
namespace dh2::data::quest_runtime_fields_v1 {
// Stable borrows of actual objective/definition owners. Fields are never copied
// into a second objective or PyData store. Their backing outlives each call.
struct ActionRef {std::uintptr_t identity=0;std::uintptr_t* character_10=nullptr;};
struct PyDataRef {std::uintptr_t identity=0;};

// The factory's full Quest scalar owner. The existing save caller publishes
// ref, and writes fields directly; there is only one row/name/Character store.
// Embedded ConditionList/ObjectiveList/RewardList bodies belong to the real
// factory backing and are reached through mandatory services below.
struct Record {
 quest_savegame_v1::QuestFields fields;
 quest_savegame_v1::QuestRef ref;
 std::int32_t state_0=0,word_4=0,word_c=0,difficulty_10=0;
 ActionRef* action_18=nullptr;ActionRef* action_1c=nullptr;
 std::uint8_t byte_5c=0,byte_5d=0,byte_64=0;
 const PyDataRef* py_data_68=nullptr;
 explicit Record(std::uintptr_t identity):ref{identity,&fields}{}
 Record(const Record&)=delete;Record& operator=(const Record&)=delete;
};
enum class Operation : std::uint32_t {
 none,construct_conditions,construct_objectives,construct_rewards,
 objective_owners,reward_owners,assign_conditions,assign_objectives,
 assign_rewards,create_objective,remove_markers,unregister_objectives,
 action_virtual,read_py_word,complete,read_stream_word,load_objectives,
 destroy_rewards,destroy_objectives,destroy_conditions
};
struct Request {
 Operation operation=Operation::none;Record* quest=nullptr;
 std::uintptr_t target=0,value=0;std::int32_t count=0;
 const PyDataRef* row=nullptr;std::uint32_t offset=0;
 player_saved_quests_v1::StreamRef* stream=nullptr;
 // The actual signed reader writes to this borrowed live destination, even
 // when it subsequently reports a partial-read failure. Never retain it.
 void* destination=nullptr;
};
struct Response {std::uintptr_t value=0;ActionRef* action=nullptr;};
struct Services {
 void* context=nullptr;
 std::int32_t (*invoke)(void*,const Request&,Response*)=nullptr;
};
enum class Status : std::uint32_t {
 complete,invalid_argument,service_unavailable,service_failed,
 source_fault,reentrant,projection_changed
};
struct Result {
 Status status=Status::complete;Operation last_operation=Operation::none;
 std::uint32_t service_calls=0,owner_stores=0;
};
class Runtime {
 Record& record_;Services services_;bool busy_=false;
public:
 Runtime(Record& record,Services services):record_(record),services_(services){}
 Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
 Status construct(std::int32_t difficulty,Result*);
 Status owner_children(Result*);
 Status assign_pydata(const PyDataRef&,Result*);
 Status reinit(Result*);
 Status load_quest_data(player_saved_quests_v1::StreamRef&,std::uint32_t flag,Result*);
 Status destroy(Result*);
};
bool is_volatile_state(std::int32_t state) noexcept;
// Whole Quest C1(int), SetOwnerToChildren, AssignPyData and ReInit callers.
// Services must perform the actual reached list constructor/assignment/owner
// propagation, Objective factory and virtual cleanup. read_py_word borrows the
// same live definition row at its original field offset (pointers may be native
// width; scalar words retain their low 32 bits). No invented empty child succeeds.
// Calls are synchronous on one owning thread. Providers may change live scalar
// fields/definition backing, but cannot destroy/rebind the Record, projection,
// Runtime or borrowed ActionRef/row controls during delivery. Failed deliveries
// retain source stores already reached. C1 deliberately leaves Character +60
// and the unaddressed padding bytes untouched. Whole D1/D2 destroy action18,
// clear it after return, read/destroy action1c, then rewards/objectives/conditions.
// Actual deleting virtual4 and list destructors remain mandatory providers.
// _loadQuestData also calls mandatory action virtual+28 and objective-list
// payload readers on the same borrowed stream. Its bool flag is source-unused.
}
