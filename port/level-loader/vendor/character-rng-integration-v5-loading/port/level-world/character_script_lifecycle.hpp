#pragma once
#include <cstdint>
namespace dh2::character {
struct ScriptLifecycleState64 {
 std::uintptr_t owner,active,pending,external_name;
 std::int32_t load_step,timer33,timer34;
 std::uint32_t delayed,scripted,reserved0,reserved1,reserved2;
};
enum ScriptLifecycleService : std::uint32_t {
 script_ai_terminate=0,script_destroy,script_construct_iphone,
 script_create_step,script_set_character,script_bind_functions,
 script_load_common,script_load_external,script_owner_is_character,
 script_query_budget,script_ai_init,script_pending_init_vcb,
 script_refresh_vitals,script_configure_skills,script_update_skills,
 script_ai_init_post,script_ai_init_final,script_owner_is_dead,
 script_timer_stop,script_design_tick,script_timer_start,
 script_ais_init,script_ais_init_post,script_ais_init_final,
 script_skill_cleanup,script_spell_cleanup,script_ais_terminate
};
struct ScriptLifecycleRequest32 {
 std::uint32_t service,argument0,argument1,reserved;
 std::uintptr_t subject,payload;
};
struct ScriptLifecycleResponse16 {std::uint32_t word,reserved;std::uintptr_t identity;};
struct ScriptLifecycleServices16 {
 void* context;
 // Borrowed synchronous services. Queries return word/identity; callback
 // mutations of this live projection are observed at original reload points.
 void(*invoke)(void*,ScriptLifecycleState64*,const ScriptLifecycleRequest32*,ScriptLifecycleResponse16*);
};
enum ScriptLifecycleOperation : std::uint32_t {
 script_replace_iphone=0,script_load_process,script_init_step,
 script_init_process,script_load_and_init,script_on_init,
 script_ai_script_init,script_cleanup,script_on_init_post,script_on_init_final
};
static_assert(sizeof(void*)==8&&sizeof(ScriptLifecycleState64)==64);
static_assert(sizeof(ScriptLifecycleRequest32)==32&&sizeof(ScriptLifecycleResponse16)==16&&sizeof(ScriptLifecycleServices16)==16);
}
// 1 completed wrapper; LoadNInit returns source 0/1. -1 malformed before effects.
// argument is original bool InitFinal for InitScriptProcess/LoadNInit.
// Construction, destruction, Lua/skills/property/timer bodies are explicit services.
extern "C" int dh2_character_script_lifecycle(dh2::character::ScriptLifecycleState64*,
 std::uint32_t operation,std::uint32_t argument,const dh2::character::ScriptLifecycleServices16*);
