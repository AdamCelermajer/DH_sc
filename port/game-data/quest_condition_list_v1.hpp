#pragma once
#include "quest_table_bindings_v1.hpp"
#include <cstdint>
#include <vector>

namespace dh2::data::quest_condition_list_v1 {
// One source v2ConditionStub pointer as a retained semantic borrow. The index
// advances by the original 0x10 stride; packed bytes never become source pointers.
struct Definition {
    quest_table_bindings_v1::View view;
    const quest_table_bindings_v1::ListRef* list=nullptr;
    std::uint32_t index=0;
};
struct ConditionFields {Definition py_data_4;};
struct ConditionRef {std::uintptr_t identity=0;ConditionFields* fields=nullptr;};
// Actual allocator-owned pointer array. Its one vector is the native pointer
// storage. Allocation/retirement remains with the real owning factory/context.
struct Array {std::uintptr_t identity=0;std::vector<ConditionRef*> slots;};
struct List {
    std::int32_t count_0=0;
    Array* children_4=nullptr;
    Definition py_data_8;
    List()=default;List(const List&)=delete;List& operator=(const List&)=delete;
};
enum class Operation:std::uint32_t {none,allocate,read_definition,factory,publish,bind_definition,delete_virtual4,deallocate,eval_virtual8,complete};
enum class Status:std::uint32_t {complete,invalid_argument,service_unavailable,service_failed,source_fault,reentrant,unsafe_storage};
struct Result {
    Status status=Status::complete;Operation last_operation=Operation::none;
    std::uint32_t service_calls=0,published=0,deleted=0,evaluated=0;
    std::uint32_t evaluation=1;
};
struct Services {
    void* context=nullptr;
    std::int32_t (*allocate)(void*,List&,std::uint32_t logical_bytes,std::uint32_t tag,Array**)=nullptr;
    // Must return the actual type-index factory object, with its real +4 field.
    // Compilation is not called by these source ConditionList bodies.
    std::int32_t (*factory)(void*,List&,std::int32_t kind,ConditionRef**)=nullptr;
    std::int32_t (*delete_virtual4)(void*,List&,ConditionRef*)=nullptr;
    std::int32_t (*deallocate)(void*,List&,Array*)=nullptr;
    std::int32_t (*eval_virtual8)(void*,List&,ConditionRef*,std::uint32_t*)=nullptr;
};
class Runtime {
    List& list_;Services services_;bool busy_=false;
public:
    Runtime(List& list,Services services):list_(list),services_(services){}
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    Status construct(Result*);
    Status assign_pydata(const Definition&,std::int32_t count,Result*);
    Status destroy(Result*);
    Status evaluate(Result*);
};
// Whole C1/C2, AssignPyData, D1/D2 and Eval callers. Definition +8 and count +0
// publish before allocation; nonpositive counts retain the current +4 array.
// Reassignment does not destroy a displaced array, matching the original. A
// failed provider retains reached stores and factory allocations. Finish the
// explicit destroy closure before retiring the factory/context and its arrays.
// Providers may alter the live count/array, but controls and captured storage
// must stay valid during delivery. Actual condition evaluation/deleting bodies
// remain mandatory providers; empty lists alone return the source result true.
}
