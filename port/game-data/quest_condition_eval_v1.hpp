#pragma once
#include "quest_condition_factory_v1.hpp"
#include <cstdint>

namespace dh2::data::quest_condition_eval_v1 {
using Record=quest_condition_factory_v1::Record;
constexpr bool compare_quest_state(std::int32_t operation,std::int32_t current,
                                   std::int32_t required)noexcept{
    return operation==0?current==required:operation==1?current<required:
           operation==2?current>required:false;
}
// Borrow actual canonical fields; none of these controls owns/copies player,
// quest or Level state. Providers keep controls/backing alive through delivery.
struct PlayerRef {std::uintptr_t identity=0;std::uintptr_t* character_660=nullptr;};
struct QuestStateRef {std::uintptr_t identity=0;std::int32_t* state_0=nullptr;};
struct LevelRef {std::uintptr_t identity=0;std::int32_t* level_list_index_3c=nullptr;};
struct Services {
    void* context=nullptr;
    std::int32_t (*application)(void*,std::uintptr_t*)=nullptr;
    std::int32_t (*player_manager)(void*,std::uintptr_t application,std::uintptr_t*)=nullptr;
    std::int32_t (*local_player)(void*,std::uintptr_t manager,std::int32_t ordinal,
                                  std::uint32_t require_character,PlayerRef**)=nullptr;
    // Mandatory actual Character::SG_GetQuestByID path, including its Save
    // selection and reached QuestSavegame::CompileQuests(false) continuation.
    // A direct Save vector lookup does not implement this service. Null means
    // the real source quest lookup missed; prior compile effects are retained.
    std::int32_t (*quest_by_id)(void*,std::uintptr_t character,std::int32_t quest_id,
                                 std::int32_t difficulty,QuestStateRef**)=nullptr;
    std::int32_t (*current_level)(void*,std::uintptr_t application,LevelRef**)=nullptr;
};
enum class Operation:std::uint32_t {none,application,player_manager,local_player,character_660,definition_id,quest_by_id,quest_state,definition_state,current_level,level_index,complete};
enum class Status:std::uint32_t {complete,invalid_argument,service_unavailable,service_failed,source_fault,reentrant,projection_changed,outside_domain};
struct Result {Status status=Status::complete;Operation last_operation=Operation::none;std::uint32_t service_calls=0,value=0;};
class Runtime {
    Record& record_;Services services_;bool busy_=false;
public:
    Runtime(Record& record,Services services):record_(record),services_(services){}
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    // Whole shared Generic Eval for source types0..2 and PlayerInLevel Eval
    // for type3. Comparator is freshly read after the actual quest provider.
    // Captured definition borrows stay stable across provider mutation/reload.
    Status evaluate(Result*);
};
}
