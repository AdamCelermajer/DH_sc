#pragma once
#include <cstdint>
namespace dh2::character {
enum ScriptKind : std::uint32_t {
 script_default=0,script_monster,script_player_iphone,script_faery,
 script_external,script_player
};
// CharAI+30 filename and +2c scripted flag; factory object ownership is a
// separate service. Filename identities are borrowed from immutable game data.
struct ScriptSelectionState16 {
 std::uintptr_t external_name=0;
 std::uint32_t scripted=0,reserved=0;
};
struct ScriptCreationFacts24 {
 std::uint32_t script_length=0,reserved=0;
 const char* script_name=nullptr;
 const char* owner_name=nullptr;
};
struct ScriptSelectionServices16 {
 void* context;
 void(*construct)(void*,ScriptSelectionState16*,std::uint32_t kind);
};
static_assert(sizeof(void*)==8&&sizeof(ScriptSelectionState16)==16);
static_assert(sizeof(ScriptCreationFacts24)==24&&sizeof(ScriptSelectionServices16)==16);
}
// Source SetScriptByName and StepCreateScript decision/store order. 1 completed,
// -1 malformed input, no callback on rejection. Constructors are synchronous
// services; actual AIS allocation, initialization and update are not replaced.
extern "C" int dh2_character_script_select(
 dh2::character::ScriptSelectionState16*,const char*,
 const dh2::character::ScriptSelectionServices16*);
extern "C" int dh2_character_script_create_step(
 dh2::character::ScriptSelectionState16*,const dh2::character::ScriptCreationFacts24*,
 const dh2::character::ScriptSelectionServices16*);
