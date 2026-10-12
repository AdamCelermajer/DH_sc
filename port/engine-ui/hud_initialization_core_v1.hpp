#pragma once
#include "hud_initialization_v1.hpp"
#include "swf_actionscript_connection.hpp"
#include <string>
namespace dh2::ui {
struct HudInitVariant12V1 {float floating;std::int32_t integer,flags;};
struct HudInitArguments16V1 {const HudInitVariant12V1* values;std::uint32_t count,reserved;};
// Native protected-AS adapter for exactly the three captured wrappers. Caller
// supplies the same live graph from SwfServices.native_action; graph scope is
// checked before touching arguments. Non-AS requests are forwarded unchanged.
// parse_text receives subject=&HudInitArguments16V1, pinned through that call.
// Formatting, skill VCB, saved/game/registry owners remain mandatory services.
// The bridge builds distinct per-call vectors; synchronous nested calls cannot
// overwrite another wrapper's VarArgs or result. Borrowed backend strings/rows
// still require the full-invocation pin contract from the coordinator header.
bool hud_initialization_native_v1(SwfAsGraph&,const char* name,
 const gameswf::fn_call&,const HudInitServices16&,std::string& error);
}
static_assert(sizeof(dh2::ui::HudInitVariant12V1)==12&&sizeof(dh2::ui::HudInitArguments16V1)==16);
