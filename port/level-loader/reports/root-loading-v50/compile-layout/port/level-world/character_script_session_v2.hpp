#pragma once
#include "character_script_session.hpp"
#include "character_script_owner_v2.hpp"
#include "character_skill_properties_v1.hpp"
#include "character_current_skill_v2.hpp"
namespace dh2::character {
struct CharacterScriptSessionInputV2:CharacterScriptSessionInput {
 void* property_context=nullptr;
 int(*external_property)(void*,std::uintptr_t,std::int32_t**)=nullptr;
 int(*recalculate_properties)(void*,data::PropertyView*)=nullptr;
 int(*normal_property_class)(void*,data::PropertyView*,std::int32_t)=nullptr;
 data::SkillTables::Borrow skill_tables;
 std::shared_ptr<data::PlayerSavegameV1> savegame;
 void* skill_number_context=nullptr;
 int(*skill_number)(void*,const dh2_script_value*,float*)=nullptr;
};
class CharacterScriptSessionV2 {
 struct Impl;
 std::unique_ptr<Impl> impl_;
 explicit CharacterScriptSessionV2(std::unique_ptr<Impl>);
public:
 static std::unique_ptr<CharacterScriptSessionV2> create(CharacterGameDesign::Borrow&&,
  const CharacterScriptSessionInputV2&,std::string& error);
 ~CharacterScriptSessionV2();
 CharacterScriptSessionV2(const CharacterScriptSessionV2&)=delete;
 CharacterScriptSessionV2& operator=(const CharacterScriptSessionV2&)=delete;
 CharacterScriptSessionV2(CharacterScriptSessionV2&&)=delete;
 CharacterScriptSessionV2& operator=(CharacterScriptSessionV2&&)=delete;
 int start(); // Fresh source loading, identical to advance(). No invented FSM.
 int advance();
 // Genuine private cache/owner services for source skill initialization.
 int load_file(std::uintptr_t script,const char* name,bool& loaded);
 int init_vcb(std::uintptr_t script);
 bool view(ScriptSessionView&) const noexcept;
 ScriptOwnerV2& owner() noexcept;
 const ScriptOwnerV2& owner() const noexcept;
 TimerStore32& timers() noexcept;
 const TimerStore32& timers() const noexcept;
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
 // Borrowed owner permits source inspection/call routing; lifecycle replacement,
 // arbitrary rebinding and ownership mutation are outside this adapter. Fresh
 // loading is supported; termination/replacement is required-delivery failure.
};
}
