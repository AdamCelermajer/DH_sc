#pragma once
#include "player_skill_session_v1.hpp"
#include "character_ai_skill_script_check.hpp"
#include "script_value_boolean_lua.hpp"
#include <memory>

namespace dh2::player_skill_use_session_v1 {
using List=character_player_skills_preparation_v3::source::List;
using Check=character_ai_skill_script_check::Check;
enum class Callback {pre,use,post};
enum class Phase {not_started,construct,set_skill,erase,callback,boolean,destroy,complete};
enum class Decision {converted,empty_success,no_script,set_skill_error,callback_error,post_discard};
struct Result {
    Phase phase{};Decision decision{};
    std::uint32_t value=0,call_count=0,return_count=0,constructed=0,erased=0,destroyed=0;
    int last_lua_status=0;
    character_ai_skill_script_check::Result source_check{};
};
// Adapted Adam c3ae797 callback choreography over our selected source check,
// getBool and SAME retained Player VM. Pre/Use/Post boundaries follow original
// 3da8b8/292,3da794/292,3da6c0/212. This port adapter adds no FSM/input/animation
// schedule, slot selection, property, timer, inventory or target-list owner.
class Runtime {
public:
    Runtime(player_skill_session_v1::Session&,
            character_player_skills_preparation_v3::Owner&,
            std::uintptr_t character);
    ~Runtime();
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    int check(List,std::uint32_t slot,Check,Result&,std::string& error);
    int invoke(List,std::uint32_t slot,Callback,Result&,std::string& error);
    std::size_t retained_failed_returns()const noexcept;
private:
    struct Impl;std::unique_ptr<Impl> impl_;
};
// Zero completes the requested caller, including source false/normal Lua Error.
// Negative VM/required-service errors stop without extra destroy/rollback.
// Pre/Use successful empty returns mean true; Post ignores normal callback
// Error. Each check independently executes SetSkill+OnSkillCheck; no replay to
// inspect a second return. All native Value/string storage survives until the
// source destroy operation or outer Runtime teardown after failure.
// Borrowed Session/preparation/actor/providers/output stay live through return;
// one owning thread, no same Runtime reentry/destruction or owner reprepare.
// Close the VM before retiring preparation/providers. Caller schedules source
// phases: this API never automatically consumes mana or advances the FSM.
}
