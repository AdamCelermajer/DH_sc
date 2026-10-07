#pragma once
#include "character_game_design.hpp"
#include "character_script_owner.hpp"
#include "character_host_context.hpp"
#include "character_timers.hpp"
#include "character_spatial_bindings.hpp"
#include "character_target_bindings.hpp"
#include "character_native_fsm.hpp"
#include "character_script_commands.hpp"
#include "../script-runtime/script_object_bridge.h"
#include "../game-data/combat_application.hpp"
#include "../script-runtime/script_scalar_bindings.h"
#include <array>
#include <functional>

namespace dh2::character {
struct ScriptSessionFile {std::string filename;std::vector<std::uint8_t> bytes;};
struct CharacterScriptSessionInput {
 std::uintptr_t identity=0;
 std::string name;
 std::shared_ptr<data::PropertyState> properties;
 std::shared_ptr<data::CombatActorState> combat;
 // Optional genuine shared CharProperties::s_temp producer. Missing temporary
 // backing leaves GetProp(...,true) explicitly unsupported, not zero-filled.
 std::shared_ptr<data::PropertySheet> temporary;
 std::array<float,3> position{};
 // Genuine entity's IsCharacter virtual projection, not AIS/script kind.
 std::uint32_t source_is_character=0;
 data::Bytes common{},external{};
 // Exact source cache paths. Files are copied; absent supplied files are an
 // explicit missing-file snapshot, not a complete Application cache claim.
 std::vector<ScriptSessionFile> include_files;
 const HostContextBindings16* host=nullptr;
 const LevelServices16* level=nullptr;
 // Optional genuine live target/FSM projections. Borrowed records, service
 // contexts and object identities outlive this session and its VM finalizers.
 // Missing producers retain explicit unsupported globals. These bindings do
 // not create a state machine, choose a target or resolve scene lifetimes.
 TargetBindings48* target=nullptr;
 NativeFsm24* state_machine=nullptr;
 const dh2_script_object_services* objects=nullptr;
 // Optional original Lua command wrappers. Controller/path/combat queries
 // remain genuine borrowed providers; a session never fabricates movement.
 ScriptCommandBindings40* commands=nullptr;
 // Expiry is genuine caller ownership. Without it initialization can create
 // timers, but update_timers rejects without changing timer state.
 const TimerServices32* timer_services=nullptr;
 void* budget_context=nullptr;
 int(*budget)(void*,std::uint32_t requested,std::int32_t* available)=nullptr;
 ScriptNativeIntegerConfiguration32 integers{};
 dh2_script_scalar_bindings scalar{};
 std::uint32_t timer_capacity=20;
 std::size_t vm_memory_limit=16u*1024u*1024u;
 // Optional source-live projection before every command wrapper, including
 // HasPath. Preserves historical fixture behavior when absent. Context and
 // borrowed command owner survive the Session and all scoped callbacks.
 void* commands_refresh_context=nullptr;
 int(*commands_refresh)(void*)=nullptr;
 //Original global RegisterSummon uses one retained process cachedCharOID map.
 //The provider pins that map independently and borrows the SAME Scene asset
 //owner; absent provider keeps the reached native global explicitly required.
 std::shared_ptr<void> summon_cache_v81;
 std::function<bool(std::int32_t,std::uint32_t,std::string&)> register_summon_v81;
 void* gameplay_context=nullptr;
 int(*gameplay_binding)(void*,std::uint32_t,dh2_script_function*,void**)=nullptr;
};
struct ScriptSessionRegistration {
 std::uint32_t phase,index,original_callback;
 std::string name;
 bool supported;
 bool installed; // Unsupported source globals are installed failing closures.
};
// Source-kernel composition with pinned per-character contexts. Noncopy/nonmove.
// Borrowed host/debug/expiry/coercion providers remain alive through VM close;
// callback providers never destroy/retarget this session during synchronous Lua.
class CharacterScriptSession {
 struct Impl;
 std::unique_ptr<Impl> impl_;
 explicit CharacterScriptSession(std::unique_ptr<Impl>);
public:
 static std::unique_ptr<CharacterScriptSession> create(CharacterGameDesign::Borrow&&,
  const CharacterScriptSessionInput&,std::string& error);
 ~CharacterScriptSession();
 CharacterScriptSession(const CharacterScriptSession&)=delete;
 CharacterScriptSession& operator=(const CharacterScriptSession&)=delete;
 CharacterScriptSession(CharacterScriptSession&&)=delete;
 CharacterScriptSession& operator=(CharacterScriptSession&&)=delete;
 int start(); // Fresh source loading, identical to advance(). No invented FSM.
 int advance();
 // Genuine private cache/owner services for source skill initialization.
 int load_file(std::uintptr_t script,const char* name,bool& loaded);
 int init_vcb(std::uintptr_t script);
 bool view(ScriptSessionView&) const noexcept;
 ScriptOwner& owner() noexcept;
 const ScriptOwner& owner() const noexcept;
 TimerStore32& timers() noexcept;
 const TimerStore32& timers() const noexcept;
 // SAME retained CharTimers growth/expiry transport used by source natives.
 // Borrow survives until session destruction; it does not create a store.
 const TimerServices32& native_timer_services() const noexcept;
 int update_timers(std::uint32_t dt_ms,std::uint32_t source_script_blocked);
 // Original AISExternal target wrappers, with fresh alias/global lookup and
 // the live InitVCB availability flags. Event numbers are object_identity's
 // TargetEvent domain, not Character event IDs. This neither discovers an
 // enemy nor decides range/sight/hostility. Native failure preserves Lua and
 // target prefixes; error() records diagnostics. An optional genuine callback
 // scope permits nested delivery only to this same privately owned VM.
 int dispatch_target(std::uint32_t event,std::uintptr_t enemy=0,
  const dh2_script_callback_scope* scope=nullptr);
 std::shared_ptr<data::PropertyState> properties() const noexcept;
 std::shared_ptr<data::CombatActorState> combat_state() const noexcept;
 data::PropertyView& property_view() noexcept;
 const float* position() const noexcept;
 void set_position(const std::array<float,3>&) noexcept;
 const std::string& script_name() const noexcept;
 const std::vector<ScriptSessionRegistration>& registrations() const noexcept;
 const std::vector<std::string>& missing_bindings() const noexcept;
 const std::string& error() const noexcept;
 const dh2_script_callback_scope* current_skill_callback_scope()const noexcept;
 // Borrowed owner permits source inspection/call routing; lifecycle replacement,
 // arbitrary rebinding and ownership mutation are outside this adapter. Fresh
 // loading is supported; termination/replacement is required-delivery failure.
};
}
