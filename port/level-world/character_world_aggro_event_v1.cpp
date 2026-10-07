#include "character_world_aggro_event_v1.hpp"
namespace dh2::character {namespace {
int dispatch(AIEventState64& ai,std::uintptr_t attacker,DebugSwitches* debug,
 const DebugFileServices24* files,const WorldAggroEventServicesV1& services,std::string& error,bool cleared){
 error.clear();std::uint32_t ignored{};
 if(!ai.ai||!attacker||!debug||!files){error="Required actual CharAI/attacker/Debug owner";return -1;}
 if(dh2_character_debug_load(debug,files)!=1||dh2_character_debug_get(&ignored,debug,"isTracingCharAIEvents",files)!=1){error=cleared?"Required OnDeAggro Debug prefix":"Required OnAggro Debug prefix";return -2;}
 if(!ai.active)return 0;
 if(!ai.ais_virtuals){error="Required selected AIS callable table";return -2;}
 const auto method=ai.ais_virtuals[(cleared?0x3c:0x38)/4];
 if(method==(cleared?0x3dbea8u:0x3dbea4u))return 0; // Whole inherited bx lr.
 if(!method||!services.ais_method||services.ais_method(services.context,&ai,method,attacker)){
  error=std::string(cleared?"Required selected AIS OnDeAggro continuation ":"Required selected AIS OnAggro continuation ")+std::to_string(method);return -2;
 }
 return 0;
}
}
int world_aggro_event_v1(AIEventState64& ai,std::uintptr_t other,DebugSwitches* debug,
 const DebugFileServices24* files,const WorldAggroEventServicesV1& services,std::string& error){return dispatch(ai,other,debug,files,services,error,false);}
int world_deaggro_event_v2(AIEventState64& ai,std::uintptr_t other,DebugSwitches* debug,
 const DebugFileServices24* files,const WorldAggroEventServicesV1& services,std::string& error){return dispatch(ai,other,debug,files,services,error,true);}
}
