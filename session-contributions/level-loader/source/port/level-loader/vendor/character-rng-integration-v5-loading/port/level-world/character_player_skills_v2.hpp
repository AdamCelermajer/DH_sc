#pragma once
#include "character_skills_session_v2.hpp"
#include "character_skill_info_session_v2.hpp"
#include "character_deferred_script.hpp"
namespace dh2::character::skills {
struct PlayerSkillInitServicesV2 {
 void* context{};
 // Exact reached _InitHpMp service. Caller may use the genuine frozen vitals
 // kernel. Missing/deeper vitals providers fail; no automatic full refill.
 int(*vitals)(void*,CharacterScriptSessionV2&,const ScriptLifecycleRequest32&){};
};
// One retained player authority: actual owned ScriptOwnerV2/VM/timers/properties
// plus pinned source skill/faery tables and nullable stable instances. Debug,
// FSM, required providers remain borrowed through all calls and VM finalizers.
class CharacterPlayerSkillsV2 {
 struct Impl;std::unique_ptr<Impl> impl_;
 explicit CharacterPlayerSkillsV2(std::unique_ptr<Impl>);
public:
 static std::unique_ptr<CharacterPlayerSkillsV2> create(CharacterGameDesign::Borrow&&,
  const CharacterScriptSessionInputV2&,data::SkillTables::Borrow,data::FaeryTables::Borrow,
  const fx::PreloadServices16&,const NativeFsm24&,const PlayerSkillInitServicesV2&,std::string&);
 ~CharacterPlayerSkillsV2();
 CharacterPlayerSkillsV2(const CharacterPlayerSkillsV2&)=delete;
 CharacterPlayerSkillsV2& operator=(const CharacterPlayerSkillsV2&)=delete;
 // Original LoadNInit active guard, owned loading/publication, then complete
 // InitProcess vitals→configure→update→selected Post→optional Final. Caller
 // owns original upstream eligibility.0active/incomplete,1new,-1bad,-2required.
 int initialize(std::uint32_t final);
 int info(std::uint32_t index,std::uint32_t level,float* fraction);
 int update();int cleanup(); // Original cleanup callbacks; no deletion emulation.
 CharacterScriptSessionV2& session()noexcept;
 const State40& state()const noexcept;
 bool ready()const noexcept;
 const std::string& error()const noexcept;
};
}
