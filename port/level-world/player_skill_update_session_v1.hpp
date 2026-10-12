#pragma once
#include "player_skill_session_v1.hpp"
#include "character_ai_update_all_skills.hpp"
#include "character_ai_update_skills.hpp"
#include "character_ai_skill_script_update.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include <memory>

namespace dh2::player_skill_update_session_v1 {
struct Result {
    character_ai_update_all_skills::Status status{};
    character_ai_update_all_skills::Result source{};
    std::uint32_t callbacks=0,lua_errors=0;
    int last_lua_status=0;
    // Required-service failures that tolerant mode treated like a Lua error
    // (see Runtime::continue_past_required_failures) and the first one's text.
    std::uint32_t required_failures=0;
    std::string first_required_failure;
};
struct SelectedFaeryResult {
    character_ai_update_skills::Status status{};
    character_ai_update_skills::Result source{};
    std::uint32_t callbacks=0,lua_errors=0;
    int last_lua_status=0;
    std::uint32_t required_failures=0;
    std::string first_required_failure;
};
// Composition adapter: uses the selected source list/FSM/update callers and
// the same retained preparation arguments and VM. No extra frame/timer store.
// ReturnValues owns native projected storage, not a 112B ARM Value overlay.
// The full source VM observer retains all results from one actual Lua call,
// including copied strings and native-width object identities; no replay.
class Runtime {
public:
    Runtime(player_skill_session_v1::Session&,
            character_player_skills_preparation_v3::Owner&,
            std::uintptr_t ai,std::uintptr_t character,const std::int32_t& state);
    ~Runtime();
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    int update(Result&,std::string& error);
    // Exact CharAI::UpdateSkills path. Uses the same borrowed Character,
    // prepared CharAI scripts, Save and VM as update(); it updates only saved
    // skill slots plus the one Faery selected for difficulty. No VM is made.
    int update_current_faery(const data::PlayerSavegameV1&,
        const std::int32_t& difficulty,SelectedFaeryResult&,std::string& error);
    std::size_t retained_failed_returns()const noexcept;
    // Default off: a callback that raises a required-service failure (a native
    // the port has not bound) stops the update with that failure, leaving the
    // earlier scripts' effects. The original has no such outcome: LuaScript::Call
    // logs and swallows every script failure and CharAI::UpdateAllSkills/
    // UpdateSkills go on to the next script. When enabled, such a failure is
    // counted in Result::required_failures (and reported through the first
    // failure text) and treated exactly like an ordinary Lua error: the script's
    // returns are discarded and the walk continues.
    void continue_past_required_failures(bool enabled)noexcept;
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
// Borrowed VM/preparation/state/control/output live through synchronous calls.
// One owning thread; no same Runtime reentry/destruction. Close VM before the
// borrowed preparation owner retires. Port failures retain completed Lua and
// ReturnValues effects; no invented rollback or cleanup runs after failure.
}
