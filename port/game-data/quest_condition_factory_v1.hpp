#pragma once
#include "quest_condition_list_v1.hpp"
#include <cstdint>

namespace dh2::data::quest_condition_factory_v1 {
using ConditionFields=quest_condition_list_v1::ConditionFields;
using ConditionRef=quest_condition_list_v1::ConditionRef;
enum class Dispatch:std::uint32_t {
    null_vtable,base,generic,quest_in_state,quest_state_lower,quest_state_higher,
    player_in_level,level_in_state,player_at_level,event_in_state,
    uninitialized=UINT32_MAX
};
// The real factory object's sole fields and stable list projection. Dispatch
// replaces the original ARM vtable address; comparator_8 is the actual source
// comparison selector for the three 12-byte quest-state condition types.
struct Record {
    ConditionFields fields;
    ConditionRef ref;
    Dispatch dispatch_0=Dispatch::uninitialized;
    std::int32_t comparator_8=0;
    explicit Record(std::uintptr_t identity):ref{identity,&fields}{}
    Record(const Record&)=delete;Record& operator=(const Record&)=delete;
};
enum class Operation:std::uint32_t {none,allocate,base_construct,construct,destruct,deallocate,complete};
enum class Status:std::uint32_t {complete,invalid_argument,service_unavailable,service_failed,source_fault,reentrant,projection_changed};
struct Result {
    Status status=Status::complete;Operation last_operation=Operation::none;
    Record* record=nullptr;
    std::uint32_t service_calls=0,field_stores=0,logical_bytes=0;
};
struct Services {
    void* context=nullptr;
    // Source logical sizes are 12 for types0..2 and8 for types3..6, tag0.
    // Native projection storage may be larger. The actual allocator/factory
    // arena owns the stable Record and its one ConditionFields, including any
    // allocation retained after an incomplete source call.
    std::int32_t (*allocate)(void*,std::uint32_t logical_bytes,std::uint32_t tag,Record**)=nullptr;
    std::int32_t (*deallocate)(void*,Record*)=nullptr;
};
class Runtime {
    Services services_;bool busy_=false;
public:
    explicit Runtime(Services services):services_(services){}
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    Status create(std::int32_t source_type,Result*);
    Status construct_base(Record&,Result*);
    // Base D1/D2 are empty. Derived D1 stores the real source parent dispatch;
    // deleting D0 then reaches mandatory actual deallocation. No definition,
    // comparator or evaluation state is cleared by destruction.
    Status destroy(Record&,bool deleting,Result*);
};
// All seven GetConditionImplInstance templates and the base/derived constructor
// and destructor leaves. Base Compile is source-empty, but this factory does
// not expose Compile or Eval substitutes. Actual evaluation remains a mandatory
// ConditionList provider until its game-service dependencies are reconstructed.
}
