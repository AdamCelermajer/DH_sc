#pragma once
#include <cstdint>
namespace dh2::character {
// Borrowed owner and callable-table projections. Callback replacements must
// remain coherent and live until the synchronous dispatcher returns.
struct AIEventOwner48 {
 std::uintptr_t owner,controller,state_machine,properties;
 std::uint32_t forced,locked,reserved0,reserved1;
};
struct AIEventState64 {
 std::uintptr_t ai;
 AIEventOwner48* owner;
 const std::uintptr_t* ai_virtuals; // 51 ARM32 byte-slot identities / 4.
 std::uintptr_t active;
 const std::uintptr_t* ais_virtuals; // Same bounded source slot domain.
 std::uint32_t paused,seeking,global_blocked,reserved0,reserved1,reserved2;
};
struct AIEventPayload24 {
 std::uintptr_t value,timer_get_id;
 std::uint32_t reserved0,reserved1;
};
enum AIEventService : std::uint32_t {
 ai_event_state_event=0,ai_event_virtual,ai_event_timer_id,
 ai_event_helper,ai_event_state_getter,ai_event_ais_virtual,ai_event_service_count
};
// operation is the actual virtual byte slot or original helper entry address.
// event is dispatcher context; argument/payload are actual semantic call args.
struct AIEventRequest40 {
 std::uint32_t service,operation,event,argument;
 std::uintptr_t subject,callee,payload;
};
struct AIEventServices24 {
 void* context;
 std::int32_t(*invoke)(void*,AIEventState64*,const AIEventRequest40*,std::uint32_t* result);
 std::uint32_t available,reserved;
};
struct AIEventResult16 {std::uint32_t phase,last_service,service_calls,reserved;};
static_assert(sizeof(void*)==8&&sizeof(AIEventOwner48)==48&&sizeof(AIEventState64)==64);
static_assert(sizeof(AIEventPayload24)==24&&sizeof(AIEventRequest40)==40&&sizeof(AIEventServices24)==24&&sizeof(AIEventResult16)==16);
}
// Complete original CharAI::RaiseAIEvent dispatcher. No source acceptance is
// invented: helper queries return their real word through explicit services.
// 0 complete,1 malformed atomic,2 unavailable,3 failed; completed phase7.
// Callback effects and captured callable identities survive nested reentry.
extern "C" int dh2_character_ai_event(dh2::character::AIEventResult16*,
 dh2::character::AIEventState64*,std::uint32_t event,
 const dh2::character::AIEventPayload24*,const dh2::character::AIEventServices24*);
// Actual CharAI::OnScriptTimer active gate and AIS virtual+90 relay, for binding
// the selected AIS/VM service. Timer id is the exact uint32 GetID result.
extern "C" int dh2_character_ai_event_script_timer(dh2::character::AIEventResult16*,
 dh2::character::AIEventState64*,std::uint32_t timer_id,const dh2::character::AIEventServices24*);
