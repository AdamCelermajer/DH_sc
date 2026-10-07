#pragma once
#include "../script-runtime/script_runtime.h"
#include "../script-runtime/script_function_alias.h"
#include <cstdint>
#include <memory>
namespace dh2::character {
// Original _State owns four strings at0/18/30/48, whose C strings are read at
//14/2c/44/5c. Native representation exposes strings, not ARM STL layout.
struct ScriptStateRecord32 {const char* update;const char* conditions;const char* init;const char* post;};
struct ScriptStatesRuntime16 {ScriptStateRecord32* current;std::uint32_t flags,reserved;};
enum ScriptStatesService:std::uint32_t {states_lookup_insert,states_assign,states_lua_call,states_default_update};
struct ScriptStatesRequest32 {std::uint32_t service,index;const char* text;ScriptStateRecord32* record;std::uintptr_t reserved;};
struct ScriptStatesServices16 {void* context;int(*invoke)(void*,ScriptStatesRuntime16*,const ScriptStatesRequest32*,ScriptStateRecord32**);};
static_assert(sizeof(ScriptStateRecord32)==32&&sizeof(ScriptStatesRuntime16)==16&&sizeof(ScriptStatesRequest32)==32&&sizeof(ScriptStatesServices16)==16);
// All callback deliveries are synchronous and must not throw. Lua errors are source diagnostics
// ignored by the original Call; provider must distinguish those from native
// failure and return0 for delivered Lua errors. Nonzero delivery fails -2.
// Records/strings and the receiver outlive the entire outer call. Providers may
// change current/flags synchronously at the original reload points. The argument
// array stays immutable throughout register. No erased records during reentry.
struct ScriptStatesCalls24 {void* context;int(*lua_call)(void*,const char*);int(*default_update)(void*);};
class ScriptStates {
 public:
 ScriptStates();~ScriptStates();ScriptStates(const ScriptStates&)=delete;ScriptStates& operator=(const ScriptStates&)=delete;
 int register_values(const dh2_script_value*,std::uint32_t);
 // Captures found target before Post; unknown names are source no-effects.
 int change_name(const char*,const ScriptStatesCalls24&);
 int call_current(std::uint32_t index,const ScriptStatesCalls24&);
 int external_update(const ScriptStatesCalls24&);
 // Additive scoped bindings; registry, VM and aliases must survive the entire
 // outer Lua call and VM finalizers. Does not own or publish an AIS/VM.
 // Change supports source string/nil/boolean getString. Numeric/identity
 // formatting remains an explicit unsupported failure until reconstructed.
 int bind_source(dh2_script_vm*,const dh2_script_aliases*);
 //The original registration walk installs these at separate source indices.
 int bind_source_entry_v108(dh2_script_vm*,const dh2_script_aliases*,bool change);
 int last_vm_status() const noexcept;
 ScriptStatesRuntime16& runtime() noexcept;
 const char* current_name() const noexcept;
 bool find(const char*,ScriptStateRecord32&) const noexcept;
 std::size_t size() const noexcept;
 private:struct Impl;std::unique_ptr<Impl> impl_;
};
}
//1 source completed (including original rejected arity/types),-1 malformed,
//-2 provider failure. Register accepts2..5 strings, preserves omitted members.
extern "C" int dh2_character_script_states_register(dh2::character::ScriptStatesRuntime16*,const dh2_script_value*,std::uint32_t,const dh2::character::ScriptStatesServices16*);
extern "C" int dh2_character_script_states_change(dh2::character::ScriptStatesRuntime16*,dh2::character::ScriptStateRecord32* captured_target,const dh2::character::ScriptStatesServices16*);
extern "C" int dh2_character_script_states_call(dh2::character::ScriptStatesRuntime16*,std::uint32_t index,const dh2::character::ScriptStatesServices16*);
// Complete External ordering; actual Default body is a required service, not
// accepted no-op. Provider can compose existing dh2_character_script_update0.
extern "C" int dh2_character_script_states_update(dh2::character::ScriptStatesRuntime16*,const dh2::character::ScriptStatesServices16*);
