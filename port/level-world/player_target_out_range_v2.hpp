#pragma once
#include "character_state.hpp"
#include <string>
namespace dh2::character {
// Whole AISDefault::OnTargetOutOfRange3dc698, also reached by IPhone3de1b4.
// Callbacks read the live owner at every source reload. No cached target is
// used after Cmd_MoveTo, whose controllable may synchronously reenter AI.
struct PlayerTargetOutRangeServicesV2 {
 void* context{};
 int(*seeking)(void*,bool&){};
 int(*state)(void*,std::int32_t&){};
 int(*has_path)(void*,bool&){};
 int(*interaction_spot)(void*,float*){};
 int(*move_point)(void*,const float*){};
 int(*clear_target)(void*){};
 int(*sync_last)(void*){};
 int(*stop_seeking)(void*){};
};
inline int player_target_out_range_v2(const PlayerTargetOutRangeServicesV2& s,std::string& error){
 auto missing=[&](const char* name){error=std::string("Required source AISDefault OutRange ")+name;return -1;};
 bool seeking=false;if(!s.seeking||s.seeking(s.context,seeking))return missing("IsTargetSeeking");
 if(!seeking)return 0;
 std::int32_t state=-1;if(!s.state||s.state(s.context,state))return missing("SM_GetState");
 if(!dh2_character_state_is_idle(state,0)&&state!=5){
  bool path=false;if(!s.has_path||s.has_path(s.context,path))return missing("HasPath before movement");
  if(path)return 0;
 }
 float point[3];if(!s.interaction_spot||s.interaction_spot(s.context,point))return missing("GetInteractionSpot");
 if(!s.move_point||s.move_point(s.context,point))return missing("Cmd_MoveTo(Point)");
 bool path=false;if(!s.has_path||s.has_path(s.context,path))return missing("HasPath after movement");
 if(path)return 0;
 if(!s.clear_target||s.clear_target(s.context))return missing("SetTarget(NULL,false)");
 if(!s.sync_last||s.sync_last(s.context))return missing("SyncLastTarget");
 if(!s.stop_seeking||s.stop_seeking(s.context))return missing("StopTargetSeeking");
 return 0;
}
}
