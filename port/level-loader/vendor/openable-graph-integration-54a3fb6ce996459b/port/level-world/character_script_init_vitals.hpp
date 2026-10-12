#pragma once
#include "character_deferred_script.hpp"
#include "visual_fx_preload.hpp"
#include "../game-data/vitals.hpp"
namespace dh2::character {
struct ScriptInitVitals32 {
 std::uintptr_t owner;
 data::PropertyView* properties;
 fx::PreloadServices16 debug;
};
struct ScriptInitVitals24 {data::VitalsChange hp,mp;};
// Borrowed stable contexts; this adapter owns neither VM nor Character. Skills
// services are mandatory when reached and remain actual caller implementations.
struct ScriptSessionInit32 {
 std::uintptr_t owner;
 const fx::PreloadServices16* debug;
 const CharacterInitServices16* skills;
 std::uint64_t reserved=0;
};
static_assert(sizeof(ScriptInitVitals32)==32&&sizeof(ScriptInitVitals24)==24&&sizeof(ScriptSessionInit32)==32);
}
// Complete HP→MP _InitHpMp projection, including reached shared Debug prefix
// and genuine PropertyAdd.1 completed,-1 malformed before effects,-2 required
// service failure preserving its prefix. Sheet/view/backing remain stable;
// callbacks may mutate their contents synchronously, never destroy/retarget them.
extern "C" int dh2_character_script_init_vitals(dh2::character::ScriptInitVitals24*,
 const dh2::character::ScriptInitVitals32*);
// CharacterInitServices16 callback. Context is ScriptSessionInit32*, persistent
// through the synchronous call. Exact refresh subject/session identity match;
// configure/update requests forward only to a genuine required skills provider.
// 0 delivered; nonzero explicit failure. No CombatActorState writes/refill flags.
extern "C" int dh2_character_script_session_init_service(void*,
 dh2::character::CharacterScriptSession&,const dh2::character::ScriptLifecycleRequest32&);
