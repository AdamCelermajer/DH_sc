#pragma once
#include "quest_objective_list_v1.hpp"
#include "player_save_section_writers_v1.hpp"
#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <variant>
extern "C" {
#include "../pydata-constants/constants.h"
}
namespace dh2::data::quest_objective_factory_v1 {
using ObjectiveFields=quest_objective_list_v1::ObjectiveFields;
using ObjectiveRef=quest_objective_list_v1::ObjectiveRef;
enum class Dispatch:std::uint32_t {
 null_vtable,base,event_receiver,kill_enemies,clear_enemies,trigger_plate,
 destroy_game_object,move_in_zone,talk_to_npc,automatic,open_game_object,
 trigger_on,picked_up_liftable,kill_enemy_template,clear_enemy_template,
 gather_loot,uninitialized=UINT32_MAX
};
struct Record {
 ObjectiveFields fields;ObjectiveRef ref;
 Dispatch dispatch_0=Dispatch::uninitialized;
 std::uint8_t compiled_8=0;std::array<std::uint8_t,3> allocation_padding_9{};
 std::uint8_t done_14=0;std::array<std::uint8_t,3> allocation_padding_15{};
 Dispatch secondary_18=Dispatch::uninitialized;
 std::uint32_t allocation_padding_1c=0,quantity_20=0;
 // Kill/Clear kinds cache their actual Definition at+24; Interact/Talk kinds
 // store a signed target ID there. The variant is the single field authority.
 std::variant<std::int32_t,quest_objective_list_v1::Definition> derived_24{std::int32_t(0)};
 std::uintptr_t derived_28=0; // Talk's borrowed objective-marker pointer.
 std::uint32_t derived_2c=0;
 explicit Record(std::uintptr_t identity):ref{{identity,&fields.character_10},&fields}{}
 Record(const Record&)=delete;Record& operator=(const Record&)=delete;
};
enum class Operation:std::uint32_t {none,allocate,default_fields,base_construct,get_constant,construct,destruct,deallocate,complete,save_data};
enum class Status:std::uint32_t {complete,invalid_argument,service_unavailable,service_failed,source_fault,reentrant,projection_changed};
struct Result {Status status=Status::complete;Operation last_operation=Operation::none;Record* record=nullptr;std::uint32_t service_calls=0,field_stores=0,logical_bytes=0;};
struct Services {
 void* context=nullptr;
 std::int32_t (*allocate)(void*,std::uint32_t logical_bytes,std::uint32_t tag,Record**)=nullptr;
 std::int32_t (*get_constant)(void*,Record&,const char* group,const char* name,std::int32_t*)=nullptr;
 std::int32_t (*deallocate)(void*,Record*)=nullptr;
};
struct Constants {const dh2_pycst_view* view=nullptr;};
// Borrows the caller's existing validated constants owner and original bytes.
// A genuine missing key returns source value0; missing/malformed owners fail.
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
 // Executes the original Objective::_saveData or Objective_SavedQty::_saveData
 // virtual body over this factory's already-owned Record. The sink receives
 // synchronous byte spans through the canonical source stream service.
 Status save_data(const Record&,const player_save_section_writers_v1::WriteServicesV1&,
                  Result*,std::string& error);
};
// Whole13 factory callers, baseC1/C2 and base/derivedD1/D0. Each Record owns
// the exact ObjectiveFields shared by the sole ObjectiveList and Quest actions.
// The provider arena retains incomplete allocations. Source constructors never
// initialize allocation padding. Retire a Record only after actual cleanup;
// Result.record is a borrowed diagnostic identity and may be retired by D0.
// Compile, events, payload readers and success predicates remain separate.
}
