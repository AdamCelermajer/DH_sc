#pragma once
#include "character_skills_session_v3.hpp"
#include "character_skill_info_session_v3.hpp"
#include "character_deferred_script.hpp"
#include "character_skills_owner_v3.hpp"
#include "character_skill_callbacks_v3.hpp"
#include "character_buffs.hpp"
#include "character_ai_events.hpp"
#include "character_skill_ai_v3.hpp"
#include <functional>
namespace dh2::character::skills {
struct PlayerSkillGameplayServicesV3 {
 // Genuine FX effects only. Recalc and owned storage are implemented inside
 // the player owner; any reached missing FX operation fails explicitly.
 BuffServices16 effects{};
 // Borrow same source CharAI/controller/FSM projection. Script Timer35 relay
 // is delivered by the same Session; other AI/DoT events stay required here.
 AIEventState64* ai{};
 AIEventServices24 events{};
 // Original SG difficulty -1 consults the current global difficulty afresh.
 // Borrow the real selected World/host owner; no copied tier default.
 void* difficulty_context{};
 int(*difficulty)(void*,std::int32_t*){};
 // Same live owner flags and explicit SetSkillState/Animator/network/trophy
 // source services. There is no second FSM or accepted missing action.
 SkillAIOwnerV3* skill_owner{};
 SkillAIServices16V3 skill_services{};
 std::function<bool(std::uint32_t&)> live_character_flags_v68;
};
struct PlayerSkillInitServicesV3 {
 void* context{};
 // Exact reached _InitHpMp service. Caller may use the genuine frozen vitals
 // kernel. Missing/deeper vitals providers fail; no automatic full refill.
 int(*vitals)(void*,CharacterScriptSessionV3&,const ScriptLifecycleRequest32&){};
 PlayerSkillGameplayServicesV3 gameplay{};
};
// One retained player authority: actual owned ScriptOwnerV2/VM/timers/properties
// plus pinned source skill/faery tables and nullable stable instances. Debug,
// FSM, required providers remain borrowed through all calls and VM finalizers.
class CharacterPlayerSkillsV3 {
 struct Impl;std::unique_ptr<Impl> impl_;
 explicit CharacterPlayerSkillsV3(std::unique_ptr<Impl>);
public:
 static std::unique_ptr<CharacterPlayerSkillsV3> create(CharacterGameDesign::Borrow&&,
  const CharacterScriptSessionInputV3&,data::SkillTables::Borrow,data::FaeryTables::Borrow,
  const fx::PreloadServices16&,const NativeFsm24&,const PlayerSkillInitServicesV3&,std::string&);
 ~CharacterPlayerSkillsV3();
 CharacterPlayerSkillsV3(const CharacterPlayerSkillsV3&)=delete;
 CharacterPlayerSkillsV3& operator=(const CharacterPlayerSkillsV3&)=delete;
 // Original LoadNInit active guard, owned loading/publication, then complete
 // InitProcess vitals→configure→update→selected Post→optional Final. Caller
 // owns original upstream eligibility.0active/incomplete,1new,-1bad,-2required.
 int initialize(std::uint32_t final);
 int info(std::uint32_t index,std::uint32_t level,float* fraction);
 int update();int cleanup(); // Original cleanup callbacks; no deletion emulation.
 int callback(std::uint32_t index,std::uint32_t operation,std::uint32_t* result);
 int skill_ai(std::uint32_t operation,std::uint32_t index,std::uint32_t* result);
 int update_timers(std::uint32_t dt_ms,std::uint32_t source_script_blocked);
 std::uint32_t buff_count()const noexcept;
 int buff_snapshot(BuffSnapshot48*,std::uint32_t ordered_index)const;
 CharacterScriptSessionV3& session()noexcept;
 const State40& state()const noexcept;
 SkillAIStateV3& skill_ai()noexcept;
 bool ready()const noexcept;
 const std::string& error()const noexcept;
};
}
