#pragma once
#include "character_ai_events.hpp"
#include "object_identity.hpp"
// Original active-AIS relay. Selected AISDefault::OnStateChanged is genuinely
// empty; unknown nonnull callees require ai_event_ais_virtual delivery.
// Request: service=ais_virtual,operation=0x20,event=0x1d,subject=captured active,
// callee=captured source function key,payload=uint32(new),argument=uint32(old).
// Source signed words are preserved bit-for-bit without sign extension.
// 0 complete,1 malformed,2 unavailable,3 provider failed. No rollback/reentry
// guard: synchronous callbacks may change the live state before nested calls.
extern "C" int dh2_character_ai_state_changed(dh2::character::AIEventState64*,
 std::int32_t new_state,std::int32_t previous,const dh2::character::AIEventServices24*);
// Source CharAI::OnEndOfAnim active/+98 relay. Default/IPhone empty body is
// handled natively. External is nonempty and requires the genuine endpoint.
// Its request has operation=0x98,event=0,payload=0,argument=0: this helper has
// no source Character-event argument; caller retains outer22/23 metadata.
extern "C" int dh2_character_ai_end_anim(dh2::character::AIEventState64*,
 const dh2::character::AIEventServices24*);
// Exact AISExternal endpoint: Default empty call then unconditional zero-arg
// LuaScript.Call("OnEndOfAnim"). Availability bits are not consulted.
// 0 delivered,1 malformed,2 failed borrowed script-call provider.
extern "C" int dh2_character_ais_external_end_anim(
 const dh2::object_identity::TargetScript16*,const dh2::object_identity::TargetServices16*);
