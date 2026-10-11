#pragma once

#include "character_ai_initialization.hpp"
#include "character_ai_set_skills_and_spells.hpp"
#include "character_ai_update_all_skills.hpp"
#include "character_runtime_factory_v1.hpp"
#include "character_script_lifecycle.hpp"
#include "../game-data/ai.hpp"

#include <cstdint>
#include <string>

namespace dh2::monster_external_script { class Session; }

namespace dh2::character_external_script_process_v1 {

namespace factory = character_runtime_factory_v1;
namespace set_skills = character_ai_set_skills_and_spells;
namespace update_skills = character_ai_update_all_skills;

// Providers borrow the one Session retained by Record::source_external_ais.
// SetSkillsAndSpells and UpdateAllSkills use the maintained source kernels;
// their callbacks receive that exact Session so Lua DeclareSkill/skill calls
// cannot silently switch to a second VM. State/vector backing remains owned by
// the caller's skill runtime.
struct SkillServices {
    void* context{};
    // Concrete owner adapters execute the maintained SetSkillsAndSpells and
    // UpdateAllSkills kernels over their one retained vector/instance store.
    // They receive this Record's already-loaded AIS Session; they must not
    // allocate a second VM or parallel vector owner.
    int (*prepare)(void*, factory::Record&, monster_external_script::Session&,
                   set_skills::Result*){};
    int (*update)(void*, factory::Record&, monster_external_script::Session&,
                  update_skills::Result*){};
};

struct Services {
    void* context{};
    // All other reached Character/CharAI dependencies are explicit. It may
    // invoke the supplied nested lifecycle service for original wrappers such
    // as CharAI::OnInitPost/OnInitFinal, but must preserve source order and
    // operate on this Record's Session and canonical owners.
    int (*invoke)(void*, factory::Record&,
        monster_external_script::Session*,
        character::ScriptLifecycleState64*,
        const character::ScriptLifecycleRequest32*,
        character::ScriptLifecycleResponse16*,
        const character::ScriptLifecycleServices16* nested){};
    SkillServices skills{};
};

struct Result {
    std::uintptr_t character{};
    std::uintptr_t char_ai{};
    std::uintptr_t vm{};
    std::uint32_t lifecycle_calls{};
    std::uint32_t last_lifecycle_service{};
    std::int32_t source_return{};
    set_skills::Result set_skills{};
    update_skills::Result update_skills{};
    bool set_skills_called{};
    bool update_skills_called{};
    bool active_external_ais{};
    bool init_process_complete{};
};

enum class Status : std::uint8_t {
    complete,
    not_initialized,
    invalid_record,
    invalid_ai_selection,
    service_unavailable,
    source_failed,
    identity_mismatch,
};

// Executes the source LoadScriptProcess caller against Record's one CharAI
// projection. Delayed actors may stop before publication; callers can continue
// with load_n_init when the source Character +0x3ec gate permits it.
Status load_process(factory::Record&, character_ai_initialization::State&,
                    const data::AiProps&, const Services&, Result*,
                    std::string& error);

// Exact CharAI::LoadNInitScriptProcess(true): load source steps, reload the
// active AIS, then execute InitScriptProcess iff publication completed. The
// source return is retained; zero means InitScriptProcess did not run (because
// the selected AIS is still unavailable or was already active at entry).
Status load_n_init(factory::Record&, character_ai_initialization::State&,
                   const data::AiProps&, const Services&, Result*,
                   std::string& error);

// Direct source InitScriptProcess continuation for callers that already
// executed LoadScriptProcess and published the selected AIS. `init_final`
// controls the source's optional OnInitFinal callback after OnInitPost.
Status init_process(factory::Record&, character_ai_initialization::State&,
                    const data::AiProps&, bool init_final,
                    const Services&, Result*, std::string& error);

} // namespace dh2::character_external_script_process_v1
