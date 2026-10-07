#pragma once
#include "character_skills.hpp"
namespace dh2::character::skills {
enum SkillReloadServiceV6:std::uint32_t {skill_reload_delete_v6=1,skill_reload_reset_end_v6,skill_reload_save_v6,skill_reload_configure_v6,skill_reload_update_v6};
struct SkillReloadRequestV6 {std::uint32_t service,index;std::uintptr_t character,instance;};
struct SkillReloadServicesV6 {void* context;int(*invoke)(void*,State40*,const SkillReloadRequestV6*);};
struct SkillReloadOutputV6 {std::uint32_t phase,deleted,calls,reserved;};
// Source AI_ReloadSkills order. The borrowed skill slots are the actual mutable
// owner's storage despite their const public view. Deleting providers must not
// change their backing/extent; each cell is nulled only after its destructor.
// reset_end is an owned-vector storage primitive, followed by real SAME Save
// reload, configure and UpdateAllSkills providers. Reached failures retain prefix.
extern "C" int dh2_character_skill_reload_v6(SkillReloadOutputV6*,State40*,const SkillReloadServicesV6*);
}
