#pragma once
#include "character_script_lifecycle.hpp"
#include <string>
namespace dh2::character {
class CharacterScriptSession;
// The source lifecycle request/response ABI is retained. A nonzero provider
// result stops this native prefix; it is never converted to source acceptance.
struct DeferredScriptServices16 {
 void* context;
 int(*invoke)(void*,ScriptLifecycleState64*,const ScriptLifecycleRequest32*,ScriptLifecycleResponse16*);
};
struct CharacterInitServices16 {
 void* context=nullptr;
 // Only refresh_vitals, configure_skills and update_skills are delivered here.
 // Callbacks receive the live source projection and may synchronously mutate it.
 // They must keep the session/receiver and all bound contexts alive.
 int(*invoke)(void*,CharacterScriptSession&,const ScriptLifecycleRequest32&)=nullptr;
};
static_assert(sizeof(DeferredScriptServices16)==16&&sizeof(CharacterInitServices16)==16);
// Nonowning, nonmoving phase adapter. Session.create already stages decoded
// delayed metadata; this adapter does not claim to reproduce the constructor's
// initial byte or perform InitPost/Idle/FSM/Character.Update eligibility.
class CharacterDeferredScript {
 CharacterScriptSession* session_;
 bool failed_=false;
 int last_virtual_status_=0;
 std::string error_;
 struct Invocation;
 static int service(void*,ScriptLifecycleState64*,const ScriptLifecycleRequest32*,ScriptLifecycleResponse16*);
public:
 explicit CharacterDeferredScript(CharacterScriptSession&) noexcept;
 CharacterDeferredScript(const CharacterDeferredScript&)=delete;
 CharacterDeferredScript& operator=(const CharacterDeferredScript&)=delete;
 // Source active!=NULL early return0; incomplete loading return0; newly loaded
 // InitProcess return1. -1 malformed, -2 unsupported/native provider failure.
 // final is the original bool. Provider/VM failure preserves its prefix and
 // quarantines this adapter; source Lua diagnostics are delivered/recorded.
 // The caller supplies actual Update eligibility and original upstream order.
 int load_and_init(std::uint32_t final,const CharacterInitServices16&);
 CharacterScriptSession& session() const noexcept;
 bool failed() const noexcept {return failed_;}
 int last_virtual_status() const noexcept {return last_virtual_status_;}
 const std::string& error() const noexcept {return error_;}
};
}
// Bounded checked form of the frozen original-derived kernel: operations
// LoadProcess1, InitProcess3, LoadNInit4, OnInitPost8 and OnInitFinal9 only.
// All source state mutations/reloads and callbacks remain in the same kernel.
// 0/1 source result; -1 malformed before effects; -2 required service failure.
extern "C" int dh2_character_deferred_script(dh2::character::ScriptLifecycleState64*,
 std::uint32_t operation,std::uint32_t argument,const dh2::character::DeferredScriptServices16*);
