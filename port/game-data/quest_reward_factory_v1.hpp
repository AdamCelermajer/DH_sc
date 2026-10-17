#pragma once
#include "quest_reward_list_v1.hpp"
#include <array>
#include <cstdint>
#include <cstring>
extern "C" {
#include "../pydata-constants/constants.h"
}
namespace dh2::data::quest_reward_factory_v1 {
using RewardFields=quest_reward_list_v1::RewardFields;
using RewardRef=quest_reward_list_v1::RewardRef;
enum class Dispatch:std::uint32_t {null_vtable,base,gold,xp,character_props,loot,consume_loot,uninitialized=UINT32_MAX};
struct Record {
 RewardFields fields;RewardRef ref;
 Dispatch dispatch_0=Dispatch::uninitialized;
 std::uint8_t compiled_8=0;std::array<std::uint8_t,3> allocation_padding_9{};
 quest_reward_list_v1::Definition compiled_py_data_14;
 std::uintptr_t property_owner_18=0;
 explicit Record(std::uintptr_t identity):ref{identity,&fields}{}
 Record(const Record&)=delete;Record& operator=(const Record&)=delete;
};
enum class Operation:std::uint32_t {none,allocate,default_fields,base_construct,get_constant,construct,destruct,deallocate,complete};
enum class Status:std::uint32_t {complete,invalid_argument,service_unavailable,service_failed,source_fault,reentrant,projection_changed};
struct Result {Status status=Status::complete;Operation last_operation=Operation::none;Record* record=nullptr;std::uint32_t service_calls=0,field_stores=0,logical_bytes=0;};
struct Services {
 void* context=nullptr;
 std::int32_t (*allocate)(void*,std::uint32_t logical_bytes,std::uint32_t tag,Record**)=nullptr;
 // The exact source call freshly queries the existing constants owner.
 std::int32_t (*get_constant)(void*,Record&,const char* group,const char* name,std::int32_t*)=nullptr;
 std::int32_t (*deallocate)(void*,Record*)=nullptr;
};
struct Constants {const dh2_pycst_view* view=nullptr;};
// Optional provider adapter borrowing the existing validated immutable constants
// view. The caller owns its original bytes. Missing keys return source value0.
inline std::int32_t borrowed_constant(void* raw,Record& record,const char* group,const char* name,std::int32_t* value){
 const auto* constants=static_cast<const Constants*>(raw);
 if(!constants||!constants->view||!group||!name||!value||reinterpret_cast<std::uintptr_t>(value)%alignof(std::int32_t))return 1;
 const auto aliases=[&](const void* input,std::size_t size){const auto a=reinterpret_cast<std::uintptr_t>(value),b=reinterpret_cast<std::uintptr_t>(input);return a>UINTPTR_MAX-sizeof(*value)||b>UINTPTR_MAX-size||(a&&b&&size&&a<b+size&&b<a+sizeof(*value));};
 if(aliases(constants,sizeof(*constants))||aliases(constants->view,sizeof(*constants->view))||aliases(constants->view->bytes,constants->view->size)||aliases(&record,sizeof(record))||aliases(group,std::strlen(group)+1)||aliases(name,std::strlen(name)+1))return 1;
 dh2_pycst_result result{};
 if(dh2_pycst_get(constants->view,group,std::uint32_t(std::strlen(group)),name,std::uint32_t(std::strlen(name)),&result))return 1;
 *value=result.found?result.value:0;return 0;
}
class Runtime {
 Services services_;bool busy_=false;
public:
 explicit Runtime(Services services):services_(services){}
 Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
 Status create(std::int32_t source_type,Result*);
 Status construct_base(Record&,Result*);
 Status destroy(Record&,bool deleting,Result*);
};
// Complete five source factory callers, Reward C1/C2 and base/derived D1/D0.
// One Record owns the same RewardFields projected into the existing List;
// allocation padding and unreached derived words are preserved. Allocator arena
// keeps each incomplete allocation and every displaced list lease stable.
// Compile/Give/effects are not implemented by these lifetime leaves. Gameplay
// must borrow the canonical inventory/property/Lua owners when those are wired.
}
