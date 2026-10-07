#include "character_world_ai_relationship_v1.hpp"
namespace dh2::character::skills {
extern "C" std::int32_t dh2_world_ai_faction_v1(const std::int32_t* resolved,std::uint32_t count){const auto id=resolved[0];return id>=0&&std::uint32_t(id)<count?id:10;}
extern "C" int dh2_world_ai_relationship_v1(WorldAiOutputV1* out,const WorldAiRelationshipV1* state,std::uint32_t enemy,std::uintptr_t target,const WorldAiServicesV1* services){
 if(!out||!state||!state->character||state->reserved||enemy>1||state->count>65536||(state->count&&!state->rows)||!services||!services->invoke)return -1;
 WorldAiOutputV1 result{};auto end=[&](int status){*out=result;return status;};
 auto call=[&](unsigned op,std::uintptr_t subject,std::uintptr_t other,WorldAiResponseV1& response,target_providers::Handle16* handle=nullptr,unsigned argument=0){result.phase=op;++result.calls;WorldAiRequestV1 request{op,argument,subject,other,handle};response={};return services->invoke(services->context,&request,&response)==0;};
 if(!target){if(!state->current_target)return end(-2);target=*state->current_target;}if(!target)return end(0);
 WorldAiResponseV1 response{};
 if(!call(world_ai_handle,target,0,response))return end(-2);auto handle=response.handle;
 if(!call(world_ai_object,target,0,response,&handle))return end(-2);const auto resolved=response.identity;
 bool character=false;if(resolved){if(!call(world_ai_kind,resolved,0,response))return end(-2);character=response.value==0;}
 if(!character){
  if(!enemy)return end(0);
  if(!call(world_ai_interactive,target,state->character,response))return end(-2);if(!response.value)return end(0);
  if(!call(world_ai_interaction_type,target,state->character,response))return end(-2);result.result=response.value==8;return end(0);
 }
 auto assertion=[&](unsigned line){result.assert_line=line;if(!state->assert_mode)return -2;const auto mode=*state->assert_mode;if(mode==2)return -3;if(mode==1){if(!call(world_ai_assert,0,0,response,nullptr,line))return -2;}return 0;};
 const auto target_negative=enemy?263u:196u,owner_negative=enemy?265u:198u;
 if(!call(world_ai_faction,resolved,0,response))return end(-2);if(response.value<0){auto code=assertion(target_negative);if(code)return end(code);}
 if(!call(world_ai_faction,resolved,0,response))return end(-2);if(response.value>=std::int32_t(state->count)){auto code=assertion(target_negative+1);if(code)return end(code);}
 if(!call(world_ai_faction,state->character,0,response))return end(-2);if(response.value<0){auto code=assertion(owner_negative);if(code)return end(code);}
 if(!call(world_ai_faction,state->character,0,response))return end(-2);if(response.value>=std::int32_t(state->count)){auto code=assertion(owner_negative+1);if(code)return end(code);}
 if(enemy){if(!call(world_ai_player,state->character,0,response))return end(-2);if(response.value){if(!call(world_ai_player,resolved,0,response))return end(-2);if(response.value)return end(0);}}
 if(!call(world_ai_faction,state->character,0,response))return end(-2);const auto row_index=response.value;
 if(row_index<0||std::uint32_t(row_index)>=state->count)return end(-3);const auto& row=state->rows[row_index];
 if(!call(world_ai_faction,resolved,0,response))return end(-2);const auto faction=response.value;
 if(row.reserved||row.count>65536||(row.count&&!row.entries))return end(-3);
 for(unsigned i=0;i<row.count;++i)if(row.entries[i].id==faction){result.result=enemy?row.entries[i].value<0:row.entries[i].value>0;break;}
 return end(0);
}
}
