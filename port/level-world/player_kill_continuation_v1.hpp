#pragma once
#include "../game-data/properties.hpp"
#include <cstdint>
#include <string>

namespace dh2::player_kill_continuation_v1 {
enum class Operation : std::uint32_t {
 is_player,property_add_int,application_player_manager,is_local_player,
 trophy_manager,property_get_int,trophy_id,trophy_unlock,online,
 get_local_player,general_continuation,is_dead,kill,event2
};
struct Request {
 Operation operation{};
 std::uintptr_t character=0,killer=0,subject=0;
 data::PropertyView* properties=nullptr;
 std::int32_t argument=0,index=0;
 std::uint32_t force=0;
 const char* name=nullptr;
};
struct Reply {std::uintptr_t identity=0,player_manager=0;std::int32_t word=0;};
struct Backend {
 void* context=nullptr;
 // Zero means the reached source service completed. Nonzero/throw preserves
 // its effects and stops the caller. Missing real locality is a failure.
 int (*invoke)(void*,const Request*,Reply*,std::string& error)=nullptr;
};
struct Bindings {
 std::uintptr_t character=0;
 const std::uint32_t* dead=nullptr;
 data::PropertyView* properties=nullptr;
 Backend backend{};
};
struct Result {
 std::uint32_t calls=0,last_operation=0,death_count_added=0,
 property_reads=0,local_player=0,trophy_calls=0,online=0,
 local_player_queried=0,general_completed=0,skipped=0,kill_completed=0,
 event2_completed=0;
 std::int32_t last_death_count=0,trophy_index=-1;
};
enum class Status {complete,invalid_argument,busy,consumed,failed};
// One already-reached Character::Kill episode. This borrows the canonical
// dead flag and PropertyView; it neither writes dead/HP nor owns their stores.
class Runtime {
 Bindings bindings_;bool busy_=false,consumed_=false;
public:
 explicit Runtime(Bindings);
 Status continue_after_hp0(std::uintptr_t killer,std::uint32_t force,
                           Result*,std::string& error);
};
// Exact outer Ctrl_Kill: fresh IsDead, genuine full Kill, then RaiseEvent(2,
// killer). The full Kill service may enter a separate borrowed Runtime above
// after performing its own fresh IsDead and dead/HP prefix. This wrapper does
// not infer a damage call or bypass either source entry gate.
class CtrlCaller {
 std::uintptr_t character_;Backend backend_;bool busy_=false,failed_=false;
public:
 CtrlCaller(std::uintptr_t character,Backend);
 std::uintptr_t character()const noexcept{return character_;}
 Status kill(std::uintptr_t killer,std::uint32_t force,Result*,std::string& error);
};
// All providers read their actual owner at each request. application returns
// both the actual Application identity and its PlayerManager (+0x40). Locality
// calls that manager's IsLocalPlayer(character), including its real Matching
// dependency. trophy_manager returns the current singleton; its identity is
// captured before the first GetInt and retained for UnlockTrophy. GetInt(25,
// false) returns signed fixed-point >>8 from this same live cached resolved
// sheet (+0xa94), without recalculating from base/saved/gear/buffs;
// AddInt(25,1) uses the same owner (raw delta 256). Trophy lookup is a fresh
// first-match name lookup, including -1; UnlockTrophy must deliver its genuine
// negative-index skip. online returns actual GetOnline()->byte_5. The online
// GetLocalPlayer request has index=0, argument=1 and a freshly read manager.
// IsPlayer=false/force!=0 requires the remaining genuine general Kill branch
// through general_continuation; absent providers fail at that reached branch.
// event2 delegates Character::RaiseEvent -> the existing AI event dispatcher,
// its fresh virtual+0x24 OnDied, then the same Coordinator.event(2,killer).
// Providers/declarations/owners outlive these single-threaded borrowers.
// Output/error must be separate from provider context. Invalid/busy/consumed
// calls leave outputs unchanged. A reached failed episode cannot be retried;
// revival requires a new episode borrower, never resetting dead here.
}
