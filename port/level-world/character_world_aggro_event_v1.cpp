#include "character_world_aggro_event_v1.hpp"
namespace dh2::character {
int world_aggro_event_v1(AIEventState64& ai,std::uintptr_t attacker,DebugSwitches* debug,
 const DebugFileServices24* files,const WorldAggroEventServicesV1& services,std::string& error){
 error.clear();std::uint32_t ignored{};
 if(!ai.ai||!attacker||!debug||!files){error="Required actual CharAI/attacker/Debug owner";return -1;}
 if(dh2_character_debug_load(debug,files)!=1||dh2_character_debug_get(&ignored,debug,"isTracingCharAIEvents",files)!=1){error="Required OnAggro Debug prefix";return -2;}
 if(!ai.active)return 0;
 if(!ai.ais_virtuals){error="Required selected AIS callable table";return -2;}
 const auto method=ai.ais_virtuals[0x38/4];
 if(method==0x3dbea4u)return 0; // Original complete inherited body: bx lr.
 if(!method||!services.ais_method||services.ais_method(services.context,&ai,method,attacker)){
  error="Required selected AIS OnAggro continuation "+std::to_string(method);return -2;
 }
 return 0;
}
}
