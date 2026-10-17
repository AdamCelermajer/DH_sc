#pragma once
#include "../game-data/player_save_load_owner_v1.hpp"

namespace dh2::character_gameplay_save_v1 {
// A projection over the existing Save and two existing QuestSavegame owners.
// These are borrowed fields, not newly constructed quest logs.
struct SaveRef {
 std::uintptr_t identity=0;
 data::PlayerSavegameV1* save=nullptr;
 data::PlayerSaveLoadOwnerV1* loader=nullptr;
 std::uintptr_t* quest_character_174=nullptr;
 std::uintptr_t* quest_character_114=nullptr;
};
// Project the actual embedded owner words; never bind detached shadow fields.
// A supplied loader must borrow this same Save. Rejected bindings return an
// invalid empty ref without mutating either owner.
inline SaveRef borrow_save(std::uintptr_t identity,data::PlayerSavegameV1& save,
                           data::PlayerSaveLoadOwnerV1* loader=nullptr) noexcept {
 if(!identity||(loader&&&loader->save()!=&save))return {};
 return {identity,&save,loader,&save.source_quest_log_118().character_5c,
         &save.source_quest_log_b8().character_5c};
}
struct Character {
 std::uintptr_t identity=0;
 SaveRef** current_save_14e8=nullptr;
};
enum class Operation {allocate_save,construct_blank_save,virtual_init_post,virtual_init_final};
struct Request {
 Operation operation{};
 std::uintptr_t character=0;
 SaveRef* save=nullptr;
 // Source logical allocation size and allocator tag. The native allocation
 // provider must allocate its real C++ owner size, not overlay ARM storage.
 std::uint32_t allocation_bytes=0,allocator_tag=0,virtual_slot=0;
};
struct Services {
 void* context=nullptr;
 int (*invoke)(void*,const Request&,SaveRef*&,std::string&)=nullptr;
};
enum class Status {complete,invalid_argument,busy,failed};
enum class Stage {not_started,allocate,construct,publish,quest_174,save_10,
 quest_114,slot_4,load,init_post,init_final,complete};
struct Result {
 Stage stage=Stage::not_started;
 std::uintptr_t captured_character=0,captured_save=0;
 std::uint32_t provider_calls=0,stores=0,load_calls=0;
 std::int32_t mask=0;
};
class Runtime {
 Character character_;Services services_;bool busy_=false;
 Status execute(unsigned,std::uintptr_t,Result*,std::string&);
public:
 Runtime(Character,Services={});
 Status initialize_player_savegame(Result*,std::string&);
 Status set_player(std::uintptr_t,Result*,std::string&);
 Status set_slot(std::uint32_t,Result*,std::string&);
 Status load(std::int32_t,Result*,std::string&);
 Status init_all(Result*,std::string&);
};
// Whole Character InitializePlayerSavegame52B, SG_SetPlayer28B, SG_SetSlot20B,
// SG_Load20B and InitAll40B. Initialize delivers allocation, blank constructor,
// publishes +14e8, then stores quest+174, Save+10, quest+114 in source order.
// Those Quest Character borrows must equal this Save's embedded +118/+b8 log
// fields respectively. Other fields inside the Save, swapped logs and foreign
// fields are invalid projections, even if their storage is otherwise writable.
// InitAll dispatches InitPost at vtable+1c then freshly reread InitFinal at
// vtable+58 on captured this. There is no separate Character::Init call.
// SG_Load null guard creates no Save; nonnull delegates the exact mask to its
// actual matching LoadOwner. No class/profile copy, VM/inventory/property/
// quest owner, InitPost body, automatic retry, rollback or destruction here.
// Borrowed fields/providers stay live through synchronous calls. Failed
// allocation/constructor backing stays with its existing lifetime owner.
// Void callee r0 values are discarded; provider status is separate port policy.
}
