#pragma once
#include "character_script_session.hpp"
#include "character_script_owner_v2.hpp"
#include "character_skill_properties_v1.hpp"
#include "character_current_skill_v2.hpp"
namespace dh2::character {
struct CharacterScriptSessionInputV3:CharacterScriptSessionInput {
 void* property_context=nullptr;
 int(*external_property)(void*,std::uintptr_t,std::int32_t**)=nullptr;
 int(*recalculate_properties)(void*,data::PropertyView*)=nullptr;
 int(*normal_property_class)(void*,data::PropertyView*,std::int32_t)=nullptr;
 data::SkillTables::Borrow skill_tables;
 std::shared_ptr<data::PlayerSavegameV1> savegame;
 void* skill_number_context=nullptr;
 int(*skill_number)(void*,const dh2_script_value*,float*)=nullptr;
 // Optional genuine cache service, borrowed through VM finalizers. Return0
 // delivers found/missing; bytes remain valid until the loader consumes them.
 // Provider owns archive path/case semantics; Session never guesses aliases.
 void* cached_file_context=nullptr;
 int(*cached_file)(void*,const char*,data::Bytes*,bool*)=nullptr;
 // Reached source registration extensions. Return1 with an actual callback,
 // 0 unsupported, -1 delivery failure. Same Session/VM owns the registration.
 void* gameplay_context=nullptr;
 int(*gameplay_binding)(void*,std::uint32_t,dh2_script_function*,void**)=nullptr;
};
class CharacterScriptSessionV3 {
 struct Impl;
 std::unique_ptr<Impl> impl_;
 explicit CharacterScriptSessionV3(std::unique_ptr<Impl>);
public:
 static std::unique_ptr<CharacterScriptSessionV3> create(CharacterGameDesign::Borrow&&,
  const CharacterScriptSessionInputV3&,std::string& error);
 ~CharacterScriptSessionV3();
 CharacterScriptSessionV3(const CharacterScriptSessionV3&)=delete;
 CharacterScriptSessionV3& operator=(const CharacterScriptSessionV3&)=delete;
 CharacterScriptSessionV3(CharacterScriptSessionV3&&)=delete;
 CharacterScriptSessionV3& operator=(CharacterScriptSessionV3&&)=delete;
 int start(); // Fresh source loading, identical to advance(). No invented FSM.
 int advance();
 // End delivery/close private VM while all property/timer/design bindings stay
 // retained. Owning gameplay facade calls this before releasing Buff backing.
 void close()noexcept;
 const TimerServices32& timer_services()const noexcept;
 // Genuine private cache/owner services for source skill initialization.
 int load_file(std::uintptr_t script,const char* name,bool& loaded);
 int init_vcb(std::uintptr_t script);
 bool view(ScriptSessionView&) const noexcept;
 ScriptOwnerV2& owner() noexcept;
 const ScriptOwnerV2& owner() const noexcept;
 TimerStore32& timers() noexcept;
 const TimerStore32& timers() const noexcept;
 int update_timers(std::uint32_t dt_ms,std::uint32_t source_script_blocked);
 // Typed original native-call route into THIS owned TimerStore. Lowest free
 // ID/growth/expiry semantics are the same as the Lua StartTimer route.
 std::int32_t start_timer(std::uint32_t duration_ms,std::int32_t repeat,
  std::int32_t event,std::uintptr_t user_ref=0);
 int stop_timer(std::uint32_t id);
 const data::ClassTables& classes()const noexcept;
 // Reads this same retained native GameDesign constant authority. Missing
 // delivery is distinct from a legitimate source -1/0 constant value.
 int constant(const char* group,const char* name,std::int32_t& value)const;
 int source_is_player(std::uint32_t& value)const;
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
 // Borrow only during this session's actual scoped F_Attack native entry.
 // Validated fresh; never synthesize or retain it across Lua callback return.
 const dh2_script_callback_scope* current_skill_callback_scope()const noexcept;
 // Borrowed owner permits source inspection/call routing; lifecycle replacement,
 // arbitrary rebinding and ownership mutation are outside this adapter. Fresh
 // loading is supported; termination/replacement is required-delivery failure.
};
}
