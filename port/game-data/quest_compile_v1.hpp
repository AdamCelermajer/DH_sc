#pragma once
#include "quest_instance_v1.hpp"
#include "quest_objective_factory_v1.hpp"
#include "quest_reward_execution_v1.hpp"

namespace dh2::data::quest_compile_v1 {
using Objective=quest_objective_factory_v1::Record;
using Reward=quest_reward_factory_v1::Record;
using ActionRef=quest_runtime_fields_v1::ActionRef;
enum class Status:std::uint32_t {complete,invalid_argument,service_unavailable,service_failed,source_fault,unsafe_storage,reentrant,projection_changed};
enum class Operation:std::uint32_t {none,resolve_objective,invalidate_objective,objective_member,clear_reward_text,resolve_reward,invalidate_reward,compile_reward,read_priority,difficulty,mark_compiled,resolve_quest,compile_quest,complete};
struct Result {Status status=Status::complete;Operation last_operation=Operation::none;std::uint32_t service_calls=0,field_stores=0,visited=0;};
struct MemberCall {
    std::uint32_t function=0;std::int32_t adjustment=0;bool is_virtual=false;
    std::int32_t marker_priority=0,quest_state=0;
};
struct Services {
    void* context=nullptr;
    // Return the actual factory Records projected by these exact borrowed refs.
    Objective* (*resolve_objective)(void*,ActionRef*)=nullptr;
    Reward* (*resolve_reward)(void*,quest_reward_list_v1::RewardRef*)=nullptr;
    // Complete reached Objective leaves, including Compile, Register and marker
    // calls, are mandatory. Automatic Compile must reach real SetIsCompleted /
    // StartScript when required; combat/world queries/events cannot be omitted.
    std::int32_t (*objective_member)(void*,Objective&,const MemberCall&)=nullptr;
};
class Runtime {
    quest_instance_v1::Instance& instance_;
    quest_table_bindings_v1::View definitions_;
    Services services_;bool busy_=false;
public:
    Runtime(quest_instance_v1::Instance& instance,quest_table_bindings_v1::View definitions,Services services):instance_(instance),definitions_(std::move(definitions)),services_(services){}
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    quest_instance_v1::Instance& instance() noexcept{return instance_;}
    Status compile(Result*);
    Status invalidate_objectives(Result*);
    Status compile_objectives(Result*);
    Status register_objectives(Result*);
    Status install_markers(std::int32_t priority,std::int32_t state,Result*);
    Status invalidate_rewards(Result*);
    Status compile_rewards(Result*);
    // Whole source ARM member-pointer LoopOnAll. Encoded adjustment includes
    // the low virtual bit; provider receives the decoded signed adjustment.
    Status loop_objectives(std::uint32_t function,std::int32_t encoded_adjustment,Result*);
};
struct LogServices {
    void* context=nullptr;
    // Whole Character::SG_GetGameDifficulty: -1 without Character+14e8 Save,
    // otherwise current shared GameDifficulty, not Save unlocked difficulty.
    std::int32_t (*difficulty)(void*,std::uintptr_t character,std::int32_t*)=nullptr;
    // Retain the original Instance/Runtime; do not construct another Quest.
    Runtime* (*resolve_quest)(void*,quest_savegame_v1::QuestRef*)=nullptr;
};
class LogRuntime {
    quest_savegame_v1::QuestSavegame& log_;LogServices services_;bool busy_=false;
public:
    LogRuntime(quest_savegame_v1::QuestSavegame& log,LogServices services):log_(log),services_(services){}
    LogRuntime(const LogRuntime&)=delete;LogRuntime& operator=(const LogRuntime&)=delete;
    Status compile_quests(bool force,Result*);
};
// Existing Instance lists/factory Records and QuestSavegame vectors/byte28 are
// sole authorities. Invalidation preserves prior compiled definitions and
// counters. Calls retain every reached store and flag, even failed providers;
// no rollback or synthetic successful child is supplied. Views/refs/arenas must
// remain alive through callbacks, which may mutate live pointers/counts/fields.
// Source Compile has no ConditionList Compile call. Reward +0c Compile uses
// the already recovered RewardExecution body; its gameplay Give is unreached.
}
