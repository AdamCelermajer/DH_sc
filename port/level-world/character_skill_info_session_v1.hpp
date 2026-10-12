#pragma once
#include "character_skill_info_v1.hpp"
#include "character_script_session.hpp"
namespace dh2::character::skills {
// Actual Character identity -> its retained, nonmoving private Session. This
// map is required caller ownership, never an inferred active-script default.
// Session/provider/instance backing cannot be destroyed during either Lua call.
struct SkillInfoSessionsV1 {
 void* context{};
 CharacterScriptSession*(*resolve)(void*,std::uintptr_t){};
 std::string error;
};
// Bind to SkillInfoServicesV1{&sessions, skill_info_session_invoke_v1}. Every
// active query re-resolves the live Character, every Call freshly resolves its
// alias. The additive runtime first-return TU must replace frozen runtime.c.
int skill_info_session_invoke_v1(void*,const SkillInfoRequestV1*,SkillInfoResponseV1*);
}
