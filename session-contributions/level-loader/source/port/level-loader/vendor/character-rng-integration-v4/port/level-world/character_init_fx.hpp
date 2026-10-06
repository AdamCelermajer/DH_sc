#pragma once
#include "visual_fx_preload.hpp"
#include "../game-data/effects_tables.hpp"
namespace dh2::character {
struct InitFxRows16 {const data::CharacterEffects* rows;std::uint32_t count;std::int32_t effects_index;};
static_assert(sizeof(InitFxRows16)==16);
}
// Source Character::RegisterCharacterFXTable. Captures all three genuine getters
// before Debug.load/GetSwitch and ordered nonnegative set registrations. The
// caller supplies the genuine cached resolved Effects index7, original table
// rows and the same persistent DebugModules/services used by the preload owner.
// Table storage and callbacks outlive synchronous calls/reentry. No factory is
// accepted here.1 complete,-1 malformed,-2 required service failure,-3 preload
// capacity/recursion boundary. Earlier delivered effects are retained.
extern "C" int dh2_character_init_fx_register(const dh2::character::InitFxRows16*,
 const dh2::fx::PreloadTable16*,dh2::fx::PreloadQueue16*,const dh2::fx::PreloadServices16*);
// Source GrabAnimFX Debug.load/GetModule and signed ID/count guard. OutputNULL
// on the original empty path. A valid enabled nonnegative ID returns-3 WITHOUT
// changing output: the genuine FX factory/continuation remains required.
// Do not treat -3 as a successful grab. Native validation does not touch output.
extern "C" int dh2_character_init_fx_negative_grab(std::uintptr_t*,std::int32_t,
 std::uint32_t set_count,const dh2::fx::PreloadServices16*);
