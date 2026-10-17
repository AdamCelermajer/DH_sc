#pragma once
#include "quest_table_bindings_v1.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::data::quest_reward_list_v1 {
struct Definition {
 quest_table_bindings_v1::View view;
 const quest_table_bindings_v1::ListRef* list=nullptr;
 std::uint32_t index=0;
};
struct RewardFields {
 std::int32_t type_4=0;Definition py_data_c;std::uintptr_t character_10=0;
};
struct RewardRef {std::uintptr_t identity=0;RewardFields* fields=nullptr;};
struct Array {std::uintptr_t identity=0;std::vector<RewardRef*> slots;};
struct List {
 std::int32_t count_0=0;Array* children_4=nullptr;
 // The source's embedded +8 string has one native owner. Lifecycle providers
 // operate on this exact string, not a copied reward-text/property store.
 std::string text_8;Definition py_data_20;
 List()=default;List(const List&)=delete;List& operator=(const List&)=delete;
};
enum class Operation:std::uint32_t {
 none,text_allocate16,text_clear,allocate,read_definition,factory,publish,
 bind_definition,bind_type,set_owner,delete_virtual4,deallocate,text_destroy,complete
};
enum class Status:std::uint32_t {
 complete,invalid_argument,service_unavailable,service_failed,source_fault,reentrant,unsafe_storage
};
struct Result {
 Status status=Status::complete;Operation last_operation=Operation::none;
 std::uint32_t service_calls=0,published=0,owner_stores=0,deleted=0;
};
struct Services {
 void* context=nullptr;
 std::int32_t (*text_allocate16)(void*,List&)=nullptr;
 std::int32_t (*text_clear)(void*,List&)=nullptr;
 std::int32_t (*allocate)(void*,List&,std::uint32_t logical_bytes,std::uint32_t tag,Array**)=nullptr;
 std::int32_t (*factory)(void*,List&,std::int32_t actual_type,RewardRef**)=nullptr;
 std::int32_t (*delete_virtual4)(void*,List&,RewardRef*)=nullptr;
 std::int32_t (*deallocate)(void*,List&,Array*)=nullptr;
 std::int32_t (*text_destroy)(void*,List&)=nullptr;
};
class Runtime {
 List& list_;Services services_;bool busy_=false;
public:
 Runtime(List& list,Services services):list_(list),services_(services){}
 Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
 Status construct(Result*);
 Status assign_pydata(const Definition&,std::int32_t count,Result*);
 Status set_owner(std::uintptr_t actual_character,Result*);
 Status destroy(Result*);
};
// Whole RewardList C1/C2, AssignPyData, SetOwner and D1/D2. Source assignment
// publishes PyData/count and clears text before reaching array allocation.
// A nonpositive count retains the existing array. Reassignment does not delete
// displaced arrays/children; their actual allocator arena keeps their leases.
// Factories must return actual type-specific Reward bodies, and deleting4 must
// perform their real destruction. Gold/XP/property/loot effects and Compile/Give
// are external and never replaced by a successful empty provider here.
// Definitions retain their original immutable table View. Providers may mutate
// live count/array and fields; captured controls/storage stay alive and stable
// on one owning thread. Failed delivery preserves stores already reached.
// Finish explicit destroy before retiring the factory, list and array arena.
}
