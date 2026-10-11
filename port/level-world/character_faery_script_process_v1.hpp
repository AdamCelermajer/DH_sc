#pragma once

#include "character_faery_record_session_v1.hpp"
#include "character_script_lifecycle.hpp"

namespace dh2::character_faery_script_process_v1 {

namespace factory = character_runtime_factory_v1;
namespace session = character_faery_script_session_v1;

struct Services {
    // Owns construction and all AISFaery/CharAIScript operations on the same
    // pending AIS. This is the only VM/script owner used by the adapter.
    session::Services faery_script{};
    // Supplies real source calls outside the Faery-session leaves (CharAI
    // timers, vitals/skills, and their underlying Lua/Default AIS callbacks).
    // The adapter intercepts SetScript<AISFaery>, CharAIScript binding and
    // load, and dispatches their lifecycle through the Record session.
    struct SourceServices {
        void* context{};
        int (*invoke)(void*, character::ScriptLifecycleState64*,
                      const character::ScriptLifecycleRequest32*,
                      character::ScriptLifecycleResponse16*,
                      const character::ScriptLifecycleServices16* nested){};
    } source{};
};

struct Result {
    std::uintptr_t character{};
    std::uintptr_t char_ai{};
    std::uintptr_t faery_ais{};
    std::uint32_t lifecycle_calls{};
    std::uint32_t last_lifecycle_service{};
    std::int32_t source_return{};
    bool session_constructed{};
    bool load_process_complete{};
    bool init_process_complete{};
};

enum class Status {
    complete,
    invalid_record,
    service_unavailable,
    source_failed,
    identity_mismatch,
};

// Executes CharAI::LoadScriptProcess against the Record's canonical CharAI.
// `script_create_step` constructs exactly one AISFaery in Record::faery_script;
// all subsequent binding/load/OnInit calls use that session. The source
// callback remains responsible for CharAI::OnInit and its timer ownership.
Status load_process(factory::Record&, const Services&, Result*,
                    std::string& error);

// Executes CharAI::InitScriptProcess on the same active AISFaery/CharAI.
// The source callback owns source vitals, skills and CharAI wrappers; AIS
// OnInitPost/OnInitFinal callbacks reached by those wrappers are routed back
// through Record::faery_script.
Status init_process(factory::Record&, bool init_final,
                    const Services&, Result*, std::string& error);

} // namespace dh2::character_faery_script_process_v1
