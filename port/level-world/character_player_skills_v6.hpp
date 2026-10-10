#pragma once
#include "character_player_skills_v3.hpp"
#include "character_skill_save_reload_v6.hpp"
#include "character_skills_owner_v6.hpp"
namespace dh2::character::skills {
class CharacterPlayerSkillsV6 {
 struct Impl;std::unique_ptr<Impl> impl_;
 explicit CharacterPlayerSkillsV6(std::unique_ptr<Impl>);
public:
 static std::unique_ptr<CharacterPlayerSkillsV6> create(CharacterGameDesign::Borrow&&,
  const CharacterScriptSessionInputV3&,data::SkillTables::Borrow,data::FaeryTables::Borrow,
  const fx::PreloadServices16&,const NativeFsm24&,const PlayerSkillInitServicesV3&,std::string&);
 ~CharacterPlayerSkillsV6();
 CharacterPlayerSkillsV6(const CharacterPlayerSkillsV6&)=delete;
 CharacterPlayerSkillsV6& operator=(const CharacterPlayerSkillsV6&)=delete;
 // Original LoadNInit active guard, owned loading/publication, then complete
 // InitProcess vitals→configure→update→selected Post→optional Final. Caller
 // owns original upstream eligibility.0active/incomplete,1new,-1bad,-2required.
 int initialize(std::uint32_t final);
 int info(std::uint32_t index,std::uint32_t level,float* fraction);
 int update();int cleanup(); // Original cleanup callbacks; no deletion emulation.
 // Original death calls these separately and reloads the retained vector
 // between them. Borrow the existing owner; no duplicate skill/VM storage.
 int cleanup_skills();int cleanup_spells();
 int native_unload_script_v105(bool final);
 int callback(std::uint32_t index,std::uint32_t operation,std::uint32_t* result);
 int skill_ai(std::uint32_t operation,std::uint32_t index,std::uint32_t* result);
 // Exact same AI_Event dispatcher, preserving the reached Lua/native error.
 int native_skill_animation_event(std::uint32_t* result);
 int update_timers(std::uint32_t dt_ms,std::uint32_t source_script_blocked);
 std::uint32_t buff_count()const noexcept;
 int buff_snapshot(BuffSnapshot48*,std::uint32_t ordered_index)const;
 CharacterScriptSessionV3& session()noexcept;
 // Borrow the existing constructed Skill/Spell owner and same VM graph.
 // This does not initialize it or establish ready(). The player and source
 // services must outlive this loan; reacquire after graph replacement.
 CharacterSkillOwnerV6* native_skill_owner()noexcept;
 const CharacterSkillOwnerV6* native_skill_owner()const noexcept;
 const State40& state()const noexcept;
 SkillAIStateV3& skill_ai()noexcept;
 bool ready()const noexcept;
 const std::string& error()const noexcept;
 // Borrow only through this retained player lifetime; never close/destroy it
 // or reenter buff mutation during a Buff service callback. Native callers
 // use the same property groups and TimerStore as the VM's Lua Buff bindings.
 BuffOwner* native_buffs()noexcept;
 // Preferred narrow same-owner native PROPS_DelBuff operation. 1 delivered,
 // -1 malformed/reentrant, -2 reached required provider or unavailable player.
 int native_delete_buff(BuffResult24*,std::int32_t id,std::uintptr_t instance=0);
 // Source Character::CancelSneaking on this retained owner. source_byte415
 // borrows the actual Character field; required AI_EndSkill is same graph.
 int native_cancel_sneaking(std::uint8_t* source_byte415);
 const data::PlayerSavegameV1* native_savegame()const noexcept;
 int native_reload_skills(const SkillSaveReloadServicesV6*);
 // CharAI::InitSkills3d8cfc, configure existing SAME vectors. This is not
 // AI_ReloadSkills: no delete/reset or Save.Load(8) is introduced.
 int native_initialize_skill_instances();
 int native_ai_init_final_v70();
 int native_update_mapped_skills_v70();
 int native_source_update_all_skills_v70();
 // Source InitProcess3ce7c0 after an earlier LoadScriptProcess3cf1f0.
 // Does not replay loading or treat the active-session guard as initialization.
 int native_init_loaded_script_v62(std::uint32_t final);
 // Same retained Skill/Spell instance and private VM callback choreography.
 int native_instance_callback_v68(std::uint32_t kind,std::uint32_t index,std::uint32_t operation,std::uint32_t* result);
 std::shared_ptr<data::PlayerSavegameV1> native_saved_owner()const noexcept;

};
}

