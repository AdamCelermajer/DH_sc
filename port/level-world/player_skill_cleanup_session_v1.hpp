#pragma once
#include "player_skill_session_v1.hpp"
#include <memory>

namespace dh2::player_skill_cleanup_session_v1 {
using List=character_player_skills_preparation_v3::source::List;
enum class Phase {not_started,vector,construct,set_skill,erase,callback,destroy,complete};
enum class Decision {none,no_script,set_skill_error,callback_discard};
struct Result {
    Phase phase{};Decision decision{};List list=List::skill;
    std::uint32_t index=0,skill_slots=0,faery_slots=0,examined=0,null_slots=0;
    std::uint32_t completed=0,constructed=0,set_calls=0,cleanup_calls=0;
    std::uint32_t erased=0,destroyed=0,lua_errors=0,return_count=0;
    std::uintptr_t instance=0;
    int last_lua_status=0;
};
// Complete source _SkillCleanUp/_SpellCleanUp loops and the 212-byte
// CharAISkillScript::OnSkillCleanUp caller over the sole retained Session.
// source_script_slot is the live Character LuaScript association, not an
// owning pointer. A null association takes the actual construct/destroy path.
class Runtime {
public:
    Runtime(player_skill_session_v1::Session* const* source_script_slot,
            character_player_skills_preparation_v3::Owner&,std::uintptr_t character);
    ~Runtime();
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    int cleanup(List,std::uint32_t index,Result&,std::string& error);
    int cleanup(List,Result&,std::string& error);
    // Exact AI_SetDead ordering: skills first, then source spell/faery vector.
    int cleanup_all(Result&,std::string& error);
    std::size_t retained_failed_returns()const noexcept;
private:
    struct Impl;std::unique_ptr<Impl> impl_;
};
// Zero completes source caller(s), including ordinary Lua errors. Negative
// required/unsupported failures retain reached effects and ReturnValues;
// no extra destructor, callback, rollback, timer stop or loop continuation.
// A successful SetSkill erases prior values only when nonempty, then invokes
// no-argument OnSkillCleanUp and destroys normal returns regardless of its
// ordinary Error. No boolean result is queried or manufactured.
// One owning thread. Slot, canonical Session, preparation/Arguments/flags and
// actor/providers outlive Runtime and every synchronous call. Do not retire,
// rebind or reprepare them from callbacks. Close the VM before retiring Owner.
// Canonical nonnull Session identity is fixed at construction; detaching the
// live slot does not create a replacement VM. Lists remain nullable/stable.
}
