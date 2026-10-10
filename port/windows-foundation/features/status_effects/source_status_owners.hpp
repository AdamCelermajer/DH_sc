#pragma once
#include "status_live_binding.hpp"
#include "../../../level-world/character_design_services.hpp"
#include "../../../level-world/character_script_lifecycle.hpp"
#include "../../../script-runtime/script_design_bindings.h"

namespace dh::foundation::status {
// Fresh CharProperties C2 bounded native projection. The host supplies the
// actual retained inline sheets and SAME process temporary sheet; this API
// allocates only the recovered source BuffOwner map, never a Character/FSM.
// Call only at source construction, before class/gear/loading/VM publication.
struct SourcePropertyConstructionBorrow {
 std::uintptr_t character{};
 dh2::data::PropertyState* sheets{};
 dh2::data::PropertyView* properties{};
 dh2::data::PropertySheet* process_temporary{};
 dh2::character::NativeFsm24* fsm{};
 dh2::character::BuffBindings32 bindings{};
};
int construct_source_property_owner(const SourcePropertyConstructionBorrow&,
 dh2::character::BuffOwner*& published,std::string&);

// CharAI::OnInit3d12b0 service composition over externally retained owners.
// It calls the already recovered lifecycle kernel, including source reloads.
// Full Character C1, CharAI global-deque registration, FSM state registration,
// table loading and original AIS Init remain explicit host source boundaries.
struct SourceStatusInitBorrow {
 std::uintptr_t character{};
 dh2::data::PropertyView* properties{};
 dh2::character::NativeFsm24* fsm{};
 dh2::character::ScriptLifecycleState64* lifecycle{};
 const dh2::character::TimerOwner8* life{};
 dh2::character::TimerStore32* timers{};
 const dh2::character::TimerServices32* timer_services{};
 const dh2_script_design_bindings* design{};
 void* context{};
 // Required only when pending/active AIS source Init is reached. Return0
 // synchronous delivery; nonzero failure. Receives actual borrowed lifecycle.
 int(*ais)(void*,dh2::character::ScriptLifecycleState64*,
  const dh2::character::ScriptLifecycleRequest32*,
  dh2::character::ScriptLifecycleResponse16*){};
};
class SourceStatusInitBinding {
public:
 explicit SourceStatusInitBinding(SourceStatusInitBorrow);
 SourceStatusInitBinding(const SourceStatusInitBinding&)=delete;
 SourceStatusInitBinding& operator=(const SourceStatusInitBinding&)=delete;
 // Source OnInit: timers then pending AIS Init even if dead gate skipped timers.
 // Returns1 complete,-1 owner mismatch,-2 missing/reached native service. A
 // negative result retains earlier stops/starts and original lifecycle stores.
 int on_init();
 const std::string& error()const{return error_;}
private:
 bool valid();
 static void invoke(void*,dh2::character::ScriptLifecycleState64*,
  const dh2::character::ScriptLifecycleRequest32*,
  dh2::character::ScriptLifecycleResponse16*);
 SourceStatusInitBorrow b_;dh2::character::ScriptLifecycleServices16 services_;
 std::string error_;bool busy_{};
};
}
