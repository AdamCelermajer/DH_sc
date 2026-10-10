#pragma once
#include "../../../level-world/character_skill_callback_session_v3.hpp"
#include "../../../level-world/character_skills_owner_v6.hpp"
#include "../../../level-world/character_skill_ai_v3.hpp"
#include "../../../level-world/character_skill_state_v4.hpp"
namespace dh::foundation::skills_animation {
using SkillLuaCallbackTransport=std::function<int(const dh2::character::skills::Instance32*,
 std::uint32_t operation,std::uint32_t& result,std::string& error)>;
// Composes real Check/Pre/Use/Post through an EXISTING selected actor VM and
// existing instances; no player-only facade or private FSM is constructed.
class NativeSkillLuaServices final {
 dh2::character::CharacterScriptSessionV3& session_;
 dh2::character::skills::CharacterSkillOwnerV6& instances_;
 dh2::character::NativeFsm24& fsm_;
 dh2::character::skills::SkillAIContextV3& ai_;
 dh2::character::skills::SkillStateV4& state_;
 dh2::character::skills::SkillAIServices16V3 required_ai_;
 dh2::character::skills::SkillStateServices16V4 required_state_;
 std::function<bool(std::uint32_t&)> flags_;
 std::string error_;
 SkillLuaCallbackTransport callback_;
 static int ai_service(void*,dh2::character::skills::SkillAIContextV3*,const dh2::character::skills::SkillAIRequest32V3*,dh2::character::skills::SkillAIResponse32V3*);
 static int state_service(void*,dh2::character::skills::SkillStateV4*,const dh2::character::skills::SkillStateRequest32V4*,dh2::character::skills::SkillStateResponse16V4*);
 bool coherent()const;
public:
 NativeSkillLuaServices(dh2::character::CharacterScriptSessionV3&,
  dh2::character::skills::CharacterSkillOwnerV6&,dh2::character::NativeFsm24&,
  dh2::character::skills::SkillAIContextV3&,dh2::character::skills::SkillStateV4&,
  dh2::character::skills::SkillAIServices16V3,dh2::character::skills::SkillStateServices16V4,
  std::function<bool(std::uint32_t&)> actual_live_flags);
 NativeSkillLuaServices(const NativeSkillLuaServices&)=delete;
 NativeSkillLuaServices& operator=(const NativeSkillLuaServices&)=delete;
 dh2::character::skills::SkillAIServices16V3 ai_services()noexcept{return {this,ai_service};}
 dh2::character::skills::SkillStateServices16V4 state_services()noexcept{return {this,state_service};}
 int command(std::uint32_t operation,std::uint32_t index,std::uint32_t& answer);
 // Supply the actual scoped indexed-return transport for callbacks reached
 // inside the same VM's DoSkill native. Default unscoped transport rejects busy.
 void set_callback_transport(SkillLuaCallbackTransport callback){callback_=std::move(callback);}
 const std::string& error()const noexcept{return error_;}
};
// Actual Lua DoSkill/BeginSkill/EndSkill registration factory. Source Value
// getUInteger then getNumber and CharSkillList count are separate borrowed
// producers, preserving original reload/order. No numeric conversion guessed.
struct SkillLuaCommandServices {
 std::function<bool(const dh2_script_value&,std::uint32_t&,std::string&)> unsigned_value;
 std::function<bool(const dh2_script_value&,std::int32_t&,std::string&)> number_integer;
 std::function<bool(std::uint32_t&,std::string&)> source_list_count;
 std::function<bool(std::uint32_t operation,std::uint32_t index,std::uint32_t& answer,std::string&)> command;
};
class NativeSkillLuaCommands final {
 struct Binding {NativeSkillLuaCommands* owner;std::uint32_t operation;};
 SkillLuaCommandServices services_;Binding commands_[3];std::string error_;
 static int invoke(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
public:
 explicit NativeSkillLuaCommands(SkillLuaCommandServices);
 NativeSkillLuaCommands(const NativeSkillLuaCommands&)=delete;
 NativeSkillLuaCommands& operator=(const NativeSkillLuaCommands&)=delete;
 static int binding(void*,std::uint32_t address,dh2_script_function*,void**);
 const std::string& error()const noexcept{return error_;}
};
// CanonicalNpcSkillsV84 retains the original Session/SkillOwner versions. Borrow
// that graph directly; do not create a parallel V6 owner for NPCs.
class NativeSkillLuaServicesV1 final {
 dh2::character::CharacterScriptSession& session_;
 dh2::character::skills::CharacterSkillOwner& instances_;
 dh2::data::SkillTables::Borrow tables_;
 dh2::character::NativeFsm24& fsm_;
 dh2::character::skills::SkillAIContextV3& ai_;
 dh2::character::skills::SkillStateV4& state_;
 dh2::character::skills::SkillAIServices16V3 required_ai_;
 dh2::character::skills::SkillStateServices16V4 required_state_;
 std::function<bool(std::uint32_t&)> flags_;std::string error_;
 SkillLuaCallbackTransport callback_;
 bool coherent()const;
 const dh2::data::SkillRecord* row(std::uint32_t);
 static int ai_service(void*,dh2::character::skills::SkillAIContextV3*,const dh2::character::skills::SkillAIRequest32V3*,dh2::character::skills::SkillAIResponse32V3*);
 static int state_service(void*,dh2::character::skills::SkillStateV4*,const dh2::character::skills::SkillStateRequest32V4*,dh2::character::skills::SkillStateResponse16V4*);
public:
 NativeSkillLuaServicesV1(dh2::character::CharacterScriptSession&,dh2::character::skills::CharacterSkillOwner&,
  dh2::data::SkillTables::Borrow,dh2::character::NativeFsm24&,
  dh2::character::skills::SkillAIContextV3&,dh2::character::skills::SkillStateV4&,
  dh2::character::skills::SkillAIServices16V3,dh2::character::skills::SkillStateServices16V4,
  std::function<bool(std::uint32_t&)> actual_live_flags);
 NativeSkillLuaServicesV1(const NativeSkillLuaServicesV1&)=delete;
 NativeSkillLuaServicesV1& operator=(const NativeSkillLuaServicesV1&)=delete;
 dh2::character::skills::SkillAIServices16V3 ai_services()noexcept{return {this,ai_service};}
 dh2::character::skills::SkillStateServices16V4 state_services()noexcept{return {this,state_service};}
 int command(std::uint32_t operation,std::uint32_t index,std::uint32_t& answer);
 void set_callback_transport(SkillLuaCallbackTransport callback){callback_=std::move(callback);}
 const std::string& error()const noexcept{return error_;}
};
}
