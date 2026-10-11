#pragma once
#include "quest_table_bindings_v1.hpp"
#include "player_saved_quests_v1.hpp"
#include "v2_event_table_v1.hpp"
#include <vector>
namespace dh2::data::quest_objective_list_v1 {
// Source v2QuestObjectiveStub pointer, retaining the same immutable definition
// generation. Either a list element or the real accept/end StubRef is borrowed.
struct Definition {
 quest_table_bindings_v1::View view;
 const quest_table_bindings_v1::ListRef* list=nullptr;
 std::uint32_t index=0;
 const quest_table_bindings_v1::StubRef* stub=nullptr;
 // GameEvent ObjectiveLists borrow their immutable trigger row from the
 // retained v2Event table rather than pretending it is a Quest table row.
 const v2_event_table_v1::Event* event=nullptr;
};
struct ObjectiveFields {
 std::int32_t type_4=0;
 Definition py_data_c;
 std::uintptr_t character_10=0;
};
// Factory-owned objective projection. ActionRef points at the one canonical
// Character word; Quest accept/end actions borrow this same ActionRef.
struct ObjectiveRef {
 quest_runtime_fields_v1::ActionRef action;
 ObjectiveFields* fields=nullptr;
};
struct Array {std::uintptr_t identity=0;std::vector<ObjectiveRef*> slots;};
struct List {
 std::int32_t count_0=0;Array* children_4=nullptr;Definition py_data_8;
 List()=default;List(const List&)=delete;List& operator=(const List&)=delete;
};
struct StreamCall {
 ObjectiveRef* objective=nullptr;
 player_saved_quests_v1::StreamRef* stream=nullptr;
 std::uintptr_t adjusted_target=0;
 std::uint32_t function=0;
 std::int32_t encoded_adjustment=0;
 bool virtual_call=false;
};
enum class MemberOperation:std::uint32_t {
 reset_data, invalidate_compile, compile, register_objectives, unregister,
 remove_markers, install_markers
};
// One ObjectiveList::LoopOnAll member dispatch. `function` and the encoded
// adjustment preserve the source member-pointer encoding; implementations
// remain borrowed canonical Objective providers.
struct MemberCall {
 ObjectiveRef* objective=nullptr;
 MemberOperation operation=MemberOperation::compile;
 std::uintptr_t adjusted_target=0;
 std::uint32_t function=0;
 std::int32_t encoded_adjustment=0;
 bool virtual_call=false;
 std::string* error=nullptr;
};
enum class Operation:std::uint32_t {none,allocate,read_definition,factory,bind_definition,publish,set_owner,stream_member,member_call,delete_virtual4,deallocate,complete};
enum class Status:std::uint32_t {complete,invalid_argument,service_unavailable,service_failed,source_fault,reentrant,unsafe_storage};
struct Result {
 Status status=Status::complete;Operation last_operation=Operation::none;
 std::uint32_t service_calls=0,published=0,owner_stores=0,loaded=0,member_calls=0,deleted=0;
};
struct Services {
 void* context=nullptr;
 std::int32_t (*allocate)(void*,List&,std::uint32_t logical_bytes,std::uint32_t tag,Array**)=nullptr;
 // Actual type-index factory body; no empty/default Objective can substitute.
 std::int32_t (*factory)(void*,List&,const Definition&,std::int32_t kind,ObjectiveRef**)=nullptr;
 std::int32_t (*stream_member)(void*,List&,const StreamCall&)=nullptr;
 std::int32_t (*delete_virtual4)(void*,List&,ObjectiveRef*)=nullptr;
 std::int32_t (*deallocate)(void*,List&,Array*)=nullptr;
 void* member_context=nullptr;
 std::int32_t (*member)(void*,List&,const MemberCall&)=nullptr;
};
class Runtime {
 List& list_;Services services_;bool busy_=false;
public:
 Runtime(List& list,Services services):list_(list),services_(services){}
 Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
 Status construct(Result*);
 Status assign_pydata(const Definition&,std::int32_t count,Result*);
 Status create_objective(const Definition&,ObjectiveRef**,Result*);
 Status set_owner(std::uintptr_t character,Result*);
 Status load(player_saved_quests_v1::StreamRef&,Result*);
 // Complete ARM member-pointer stream loop. The source load wrapper uses
 // function=0x28, encoded_adjustment=1 (virtual+28, zero this adjustment).
 Status loop_stream(std::uint32_t function,std::int32_t encoded_adjustment,
                    player_saved_quests_v1::StreamRef&,Result*);
 Status loop_member(MemberOperation,std::uint32_t function,
                    std::int32_t encoded_adjustment,Result*,std::string& error);
 Status reset_data(Result*,std::string& error);
 Status invalidate_compile(Result*,std::string& error);
 Status compile(Result*,std::string& error);
 Status register_objectives(Result*,std::string& error);
 Status destroy(Result*);
};
// Whole C1/C2, AssignPyData, CreateObjectiveWithPyData, SetOwner, _loadData,
// stream LoopOnAll and D1/D2 callers. Assign stores definition/count before
// allocation; nonpositive counts retain existing array. Source reassignment
// does not free displaced arrays; provider arena retains their actual leases.
// Publication and destructor slot clears use captured backing, then refresh
// live count/backing in the exact source order. Destructor retains count/row.
// Finish explicit destroy before retiring factory/context and its arrays.
// Synchronous callbacks may change live count/backing but must keep controls,
// captured arrays and immutable View leases valid during delivery. Bounds,
// output aliases, exceptions and reentry are separate native safety policies.
// Actual factory/action/deleting bodies and array allocation remain mandatory
// providers. This module creates no VM, quest state, properties or timer store.
}
