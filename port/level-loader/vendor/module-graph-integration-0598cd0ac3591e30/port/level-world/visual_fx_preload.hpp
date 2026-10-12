#pragma once
#include "character_design_services.hpp"
namespace dh2::fx {
struct PreloadStep8 {std::int32_t effect_id,type;};
struct PreloadSet16 {const PreloadStep8* steps;std::int32_t count;std::uint32_t reserved;};
struct PreloadTable16 {const PreloadSet16* sets;std::uint32_t set_count,effect_count;};
struct PreloadQueue16 {std::int32_t* ids;std::uint32_t count,capacity;};
enum PreloadOperation :std::uint32_t {debug_load=1,debug_module=2,debug_switch=3};
struct PreloadServices16 {void* context;int(*call)(void*,std::uint32_t,const char*,std::uint32_t*);};
struct DebugModules;
static_assert(sizeof(PreloadStep8)==8&&sizeof(PreloadSet16)==16&&sizeof(PreloadTable16)==16&&sizeof(PreloadQueue16)==16&&sizeof(PreloadServices16)==16);
}
// Original registration only: no library metadata/factory/play/render acceptance.
// Caller owns stable table/queue storage. Callbacks may synchronously reenter and
// change valid counts/step pointers; captured set row and next-step rereads follow
// source. No concurrent access, queue overlap, or storage destruction is allowed.
// 1 completed (including original invalid-ID/module-off returns), -1 malformed
// atomic rejection, -2 required service failure (prefix retained), -3 exhausted
// queue/recursion workspace. The depth128 bound is an explicit native contract;
// source has no recursion-cycle guard. Negative set counts act as source empty.
extern "C" int dh2_fx_register_set(const dh2::fx::PreloadTable16*,dh2::fx::PreloadQueue16*,std::int32_t,const dh2::fx::PreloadServices16*);
extern "C" int dh2_fx_register_effect(const dh2::fx::PreloadTable16*,dh2::fx::PreloadQueue16*,std::int32_t,const dh2::fx::PreloadServices16*);
// Genuine missing-file DebugSwitches projection plus its independent module map.
// Both borrowed owners/providers outlive this object and every callback. The
// existing-file parser remains unsupported and its diagnostic is propagated.
extern "C" dh2::fx::DebugModules* dh2_fx_debug_modules_create(dh2::character::DebugSwitches*,const dh2::character::DebugFileServices24*);
extern "C" void dh2_fx_debug_modules_destroy(dh2::fx::DebugModules*);
extern "C" int dh2_fx_debug_module_get(std::uint32_t*,dh2::fx::DebugModules*,const char*);
// Exact synchronous adapter for PreloadServices16; context is DebugModules*.
extern "C" int dh2_fx_debug_preload_service(void*,std::uint32_t,const char*,std::uint32_t*);
