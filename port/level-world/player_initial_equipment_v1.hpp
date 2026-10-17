#pragma once
#include "../game-data/fresh_inventory_owned_v4.hpp"
#include <string>

namespace dh2::player_initial_equipment_v1 {
enum class Operation : std::uint32_t {online,online_player_record,add_loot};
struct Request {
 Operation operation{};std::uintptr_t character=0;
 data::FreshInventoryOwnedV4* inventory=nullptr;
 data::PropertyView* properties=nullptr;
 const data::OwnedInventoryServicesV4* equipment_services=nullptr;
 // Full original AddLoot arguments: loot,0,0,-1,0. The online-record query
 // has argument0=0, the original GetOnlinePlayerRecord selector.
 std::int32_t arguments[5]{};
};
struct Reply {
 std::uint32_t word=0;
 // Full-width actual online record identity and its freshly read byte66c.
 // A reached null record is an explicit unsafe-source boundary, never1.
 std::uintptr_t record=0;
 std::uint8_t record_byte66c=0;
};
struct Backend {
 void* context=nullptr;
 // Zero means the actual source service completed. Failed/throw preserves
 // its completed effects. AddLoot operates on the supplied sole inventory,
 // source Item factory and retained Item identities with these full args.
 int (*invoke)(void*,const Request*,Reply*,std::string&)=nullptr;
};
struct Bindings {
 std::uintptr_t character=0;
 data::FreshInventoryOwnedV4* inventory=nullptr;
 data::PropertyView* properties=nullptr;
 // Borrow the stable descriptor returned by the existing equipment facade.
 // Its context retains real text/Skin/world/item retirement providers.
 const data::OwnedInventoryServicesV4* equipment_services=nullptr;
 Backend backend{};
};
enum class Decision : std::uint32_t {not_started,online_record_skipped,
 existing_items_skin,existing_gold_skin,empty_after_loot,equipped};
struct Result {
 Decision decision=Decision::not_started;
 std::uint32_t backend_calls=0,last_operation=0,online_queries=0,record_queries=0,
 item_count_reads=0,gold_reads=0,loot_property_reads=0,add_loot_calls=0,
 equippable_queries=0,auto_equip_calls=0,skin_calls=0;
 std::uint32_t captured_items=0,last_index=0;
 std::int32_t loot=-1,last_auto_equip=0;
};
enum class Status {complete,invalid_argument,busy,failed};
class Runtime {
 Bindings bindings_;bool busy_=false;
public:
 explicit Runtime(Bindings);
 Status initialize(Result*,std::string&);
};
// Adapted from Adam Celermajer's player_initial_grants_v2 at791e961b, only
// original Character::_InitEquipment276B@3b395c. The original invokes it
// from InitPost only after genuine SG_Load(4) and fresh IsLocalPlayer. That
// outer locality/profile/InitPost producer remains the native caller's task.
// No inventory/property/RNG/Save/VM/timer/profile owner or skill grants here.
// Borrowed owners, descriptor and contexts stay live on one owning thread;
// source calls read them freshly. Missing AddLoot continuations, text/Skin or
// item retirement providers fail at their reached prefix. No fixed starter
// IDs, fabricated locality, inventory clear, retry or success fallback.
}
