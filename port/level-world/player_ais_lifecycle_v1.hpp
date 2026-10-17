#pragma once
#include "character_ai_initialization.hpp"
#include "ais_external_initialization.hpp"
#include "ais_player_init_vcb.hpp"
#include "character_script_lifecycle.hpp"
#include "character_coordinator.hpp"
#include "character_level_runtime.hpp"
#include "player_skill_session_v1.hpp"
#include "player_skill_update_session_v1.hpp"
#include "player_skill_use_session_v1.hpp"
#include "../game-data/ai.hpp"
#include "../game-data/player_savegame_v1.hpp"

namespace dh2::player_ais_lifecycle_v1 {
// Native caller-owned projection of the inline AISPlayerIPhone constructor.
// callback_flags is the existing Session's sole live flags receiver. The
// constructor's CharAIScript projection has no independent operational flags.
struct PlayerFields {
    ais_external_initialization::State script{};
    std::uintptr_t vector_begin=0,vector_end=0,vector_capacity=0;
    std::uint32_t skill_d0=0,skill_d4=0;
};
struct Tables {std::uintptr_t char_ai_script=0,ais_player=0,ais_player_iphone=0;};
enum class Operation {construct_vm,configure_skills};
struct Request {
    Operation operation;
    std::uintptr_t character,ais;
    std::uint32_t allocation_bytes=0,skip_bind=0;
};
struct Backend {
    void* context=nullptr;
    // construct_vm performs the real deferred Lua/Session construction or
    // transfers its already-created unbound Session into session_slot. It
    // must honor skip_bind=1; no registration or source loading here.
    // configure_skills constructs/prepares the sole retained Owner and its
    // update/use adapters, publishing their borrowed slots below. Source
    // SetSkillsAndSpells and its Player InitVCB run in this SAME Session.
    // Zero delivers the actual operation. Missing/nonzero/throw is failure.
    int (*invoke)(void*,const Request*,std::string&)=nullptr;
};
struct Bindings {
    character_ai_initialization::State* ai=nullptr;
    PlayerFields* fields=nullptr;
    ais_player_init_vcb::State* callback_flags=nullptr;
    const Tables* tables=nullptr;
    const data::AiProps* declaration=nullptr;
    const char* owner_name=nullptr;
    player_skill_session_v1::Session** session_slot=nullptr;
    character_player_skills_preparation_v3::Owner** preparation_slot=nullptr;
    player_skill_update_session_v1::Runtime** update_slot=nullptr;
    player_skill_use_session_v1::Runtime** use_slot=nullptr;
    const data::PlayerSavegameV1* savegame=nullptr;
    character::Coordinator* coordinator=nullptr;
    const std::uint32_t* dead=nullptr;
    const dh2_pycst_view* design=nullptr;
    character_level_runtime::Runtime* vitals=nullptr;
    const character_level_runtime::Storage* vitals_storage=nullptr;
    Backend backend{};
};
enum class Status {complete,invalid_argument,busy,failed};
struct Result {
    int source_return=0;
    std::uint32_t service_calls=0,last_service=0,init_phase_mask=0;
    ais_external_initialization::Result constructor{};
    character_level_runtime::Result vitals{};
    player_skill_update_session_v1::Result update{};
};
// Borrowed composition, adapted from Adam791 V6's unchanged V3 source. It
// drives the existing selection/lifecycle kernels and publishes their actual
// stores into CharAI +1c/+20/+28, never a fabricated load/readiness flag.
// Owns no VM, timer, Save, property, preparation, skill instance or AIS.
class Runtime {
public:
    explicit Runtime(Bindings);
    // Exact LoadScriptProcess: advances load/publication only. It does not
    // deliver vitals, preparation, skill update, Post or Final. Source phase 7
    // permits the caller's real profile/equipment/slot work before InitProcess.
    Status load(Result*,std::string& error);
    // Exact InitScriptProcess, delivered by its owning caller after load.
    // This source operation has no active/replay guard: every explicit call
    // runs vitals/configure/update/Post and optional Final. Deliver it once at
    // the actual InitPost boundary; this adapter invents no readiness guard.
    Status initialize_process(std::uint32_t init_final,Result*,std::string& error);
    // Complete is successful source-call delivery, not a readiness verdict.
    // For initialize (LoadNInit), source_return 0 means no newly completed
    // InitProcess; callers must retain
    // their full initialization result across the original active guard.
    Status initialize(std::uint32_t init_final,Result*,std::string& error);
private:
    Status execute(character::ScriptLifecycleOperation,std::uint32_t,Result*,std::string&);
    Bindings bindings_;
    bool busy_=false,failed_=false;
};
// One owning thread. All borrowed controls/backing and canonical callback
// receiver live through dispatch and VM close. No reentry/rebinding/destruction
// from providers. Failure preserves completed source effects and stops future
// automatic retries; teardown belongs to the existing native owner. The caller
// must retire Runtime before replacing its declaration or owner backing.
// The source Player initial virtuals are AISDefault's proven empty leaves;
// Player InitVCB is invoked by the actual preparation provider after publication.
}
