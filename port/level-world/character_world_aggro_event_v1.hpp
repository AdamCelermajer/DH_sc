#pragma once
#include "character_ai_events.hpp"
#include "character_design_services.hpp"
#include <string>
namespace dh2::character {
// Exact CharAI::OnAggro debug prefix and selected AIS virtual+38 dispatch.
// The AIS callable identity must come from the retained original selector.
// Only inherited AISDefault::OnAggro is locally complete; other AIS methods
// require their real continuation. The attacker is a borrowed live identity.
struct WorldAggroEventServicesV1 {
 void* context{};
 int (*ais_method)(void*,AIEventState64*,std::uintptr_t method,std::uintptr_t attacker){};
};
int world_aggro_event_v1(AIEventState64&,std::uintptr_t attacker,DebugSwitches*,
 const DebugFileServices24*,const WorldAggroEventServicesV1&,std::string&);
// Whole CharAI::OnDeAggro3d2014: same debug prefix, then freshly selected
// AIS virtual+3c. The inherited Default body3dbea8 is literal bx lr.
int world_deaggro_event_v2(AIEventState64&,std::uintptr_t other,DebugSwitches*,
 const DebugFileServices24*,const WorldAggroEventServicesV1&,std::string&);
}
