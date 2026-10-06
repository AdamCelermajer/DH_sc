#pragma once
#include <cstdint>

namespace dh2::character {
// Projection of CharAI+1c/+4 and the current owner's directly read fields.
// A service changing the owner must refresh this projection synchronously.
struct AIUpdateState80 {
 std::uintptr_t active,owner,target408,target418,visual,visual_node;
 std::int32_t state;
 std::uint32_t machine_present,zoned,flag85;
 float saved_position[3];
 std::uint32_t reserved;
};
enum AIUpdateService : std::uint32_t {
 ai_update_active=0,ai_update_disable_zoning,ai_update_sync_visibility,
 ai_update_is_zonable,ai_update_enable_zoning,ai_update_is_dead,
 ai_update_set_position,ai_update_node_set_position,ai_update_service_count
};
struct AIUpdateRequest32 {
 std::uint32_t service,argument;
 std::uintptr_t subject;
 float position[3];
 std::uint32_t reserved;
};
struct AIUpdateServices24 {
 void* context;
 // Return0 means the genuine borrowed service completed. Any other status
 // reports failure; the separate result preserves every32-bit query value.
 std::int32_t(*invoke)(void*,AIUpdateState80*,const AIUpdateRequest32*,std::uint32_t* result);
 std::uint32_t available,reserved;
};
struct AIUpdateResult16 {
 std::uint32_t phase,last_service,service_calls,reserved;
};
static_assert(sizeof(void*)==8&&sizeof(AIUpdateState80)==80&&sizeof(AIUpdateRequest32)==32);
static_assert(sizeof(AIUpdateServices24)==24&&sizeof(AIUpdateResult16)==16);
}
// Complete CharAI::OnUpdate coordinator: active virtual, state/target gates,
// zoning/visibility and saved-position restoration. The AIS/FSM/zone/position
// services are genuine synchronous dependencies, never substituted no-ops.
// 0 complete,1 malformed(top-level atomic),2 unavailable service,3 failed
// service. Runtime failures retain already completed external side effects.
// Phase=service+1 during an attempted call;9 means source epilogue reached.
extern "C" int dh2_character_ai_update(dh2::character::AIUpdateResult16*,
 dh2::character::AIUpdateState80*,const dh2::character::AIUpdateServices24*);
