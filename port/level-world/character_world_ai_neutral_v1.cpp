#include "character_world_ai_neutral_v1.hpp"
namespace dh2::character::skills {
extern "C" int dh2_world_ai_neutral_v1(WorldAiOutputV1* out,const WorldAiRelationshipV1* state,std::uintptr_t target,const WorldAiServicesV1* services){
 if(!out||!state||!state->character||state->reserved||state->count>65536||(state->count&&!state->rows)||!services||!services->invoke)return -1;
 WorldAiOutputV1 result{};result.result=1;auto end=[&](int status){*out=result;return status;};
 auto call=[&](unsigned op,std::uintptr_t subject,WorldAiResponseV1& response,target_providers::Handle16* handle=nullptr,unsigned argument=0){result.phase=op;++result.calls;WorldAiRequestV1 request{op,argument,subject,0,handle};response={};return services->invoke(services->context,&request,&response)==0;};
 if(!target){if(!state->current_target)return end(-2);target=*state->current_target;}if(!target)return end(0);
 WorldAiResponseV1 response{};if(!call(world_ai_handle,target,response))return end(-2);auto handle=response.handle;
 if(!call(world_ai_object,target,response,&handle))return end(-2);const auto resolved=response.identity;if(!resolved)return end(0);
 if(!call(world_ai_kind,resolved,response))return end(-2);if(response.value!=0)return end(0);
 auto assertion=[&](unsigned line){result.assert_line=line;if(!state->assert_mode)return -2;const auto mode=*state->assert_mode;if(mode==2)return -3;if(mode==1){if(!call(world_ai_assert,0,response,nullptr,line))return -2;}return 0;};
 if(!call(world_ai_faction,resolved,response))return end(-2);if(response.value<0){const auto code=assertion(226);if(code)return end(code);}
 if(!call(world_ai_faction,resolved,response))return end(-2);if(response.value>=std::int32_t(state->count)){const auto code=assertion(227);if(code)return end(code);}
 if(!call(world_ai_faction,state->character,response))return end(-2);if(response.value<0){const auto code=assertion(228);if(code)return end(code);}
 if(!call(world_ai_faction,state->character,response))return end(-2);if(response.value>=std::int32_t(state->count)){const auto code=assertion(229);if(code)return end(code);}
 if(!call(world_ai_faction,state->character,response))return end(-2);const auto index=response.value;if(index<0||std::uint32_t(index)>=state->count)return end(-3);const auto& row=state->rows[index];
 if(!call(world_ai_faction,resolved,response))return end(-2);const auto faction=response.value;
 if(row.reserved||row.count>65536||(row.count&&!row.entries))return end(-3);
 for(unsigned i=0;i<row.count;++i)if(row.entries[i].id==faction){result.result=row.entries[i].value==0;break;}
 return end(0);
}
}
