#pragma once
#include "quest_reward_factory_v1.hpp"
#include "fresh_inventory_owned_v4.hpp"
namespace dh2::data::quest_reward_execution_v1 {
using Record=quest_reward_factory_v1::Record;
using Status=quest_reward_factory_v1::Status;
struct CharacterRef {
 std::uintptr_t identity=0;FreshInventoryOwnedV4* inventory=nullptr;
 std::int32_t* reward_gold_1500=nullptr;std::int32_t* reward_xp_1504=nullptr;
};
enum class Operation:std::uint32_t {none,compile,read_definition,resolve_character,add_gold,give_xp,store_receipt,complete};
struct Result {Status status=Status::complete;Operation last_operation=Operation::none;std::uint32_t service_calls=0,field_stores=0,value=0;};
struct Services {
 void* context=nullptr;
 // Native pointer projection only. The returned Character and direct source
 // destinations stay alive for the synchronous call; no copied receipt store.
 CharacterRef* (*resolve_character)(void*,std::uintptr_t)=nullptr;
 std::int32_t (*add_gold)(void*,Record&,CharacterRef&,std::int32_t)=nullptr;
 std::int32_t (*give_xp)(void*,Record&,CharacterRef&,std::int32_t fixed_point,std::uint8_t source_flag,bool*)=nullptr;
};
struct InventoryEffects {const OwnedInventoryServicesV4* services=nullptr;};
inline std::int32_t inventory_add_gold(void* raw,Record&,CharacterRef& character,std::int32_t amount){
 const auto* binding=static_cast<const InventoryEffects*>(raw);
 if(!binding||!binding->services||!character.inventory||character.inventory->character()!=character.identity)return 1;
 std::string error;return character.inventory->add_gold(amount,*binding->services,error)?0:1;
}
class Runtime {
 Record& record_;Services services_;bool busy_=false;
public:
 Runtime(Record& record,Services services):record_(record),services_(services){}
 Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
 Status compile(Result*);
 Status give_gold(Result*);
 Status give_xp(Result*);
};
// All five Compile leaves plus complete Gold/XP Give callers. Definition reads
// borrow the immutable table generation retained in the actual Record. Gold
// composes the already ported inventory AddGold body and its mandatory effects;
// Character::_GiveXP remains an explicit reached dependency. No experience,
// timer, property, reward receipt or inventory mirror is created. Fresh reads
// after callbacks preserve source reentry/partial failure stores. Actual native
// pointer projection checks are guards, not original ARM success behavior.
}
