#include "character_world_handle_v1.hpp"
namespace dh2::character::skills {
extern "C" int dh2_world_handle_object_v1(std::uintptr_t* out,target_providers::Handle16* handle,target_providers::Registry24* registry,std::uint32_t asserted,const std::int32_t* mode,const WorldAiServicesV1* services){
 if(!out||!handle||asserted>255)return -1;std::uintptr_t value=0;
 if(handle->key){
  if(!registry||registry->reserved||registry->count>registry->capacity||registry->capacity>65536||(registry->capacity&&!registry->records))return -1;
  const auto frame=registry->frame;value=handle->cached;
  if(!value||handle->frame!=frame){
   unsigned low=0,high=registry->count;while(low<high){const auto mid=low+(high-low)/2;if(registry->records[mid].key<handle->key)low=mid+1;else high=mid;}
   if(low==registry->count||registry->records[low].key!=handle->key){
    if(registry->count==registry->capacity)return -2;
    for(auto i=registry->count;i>low;--i)registry->records[i]=registry->records[i-1];registry->records[low]={handle->key,0,0};++registry->count;
   }
   value=registry->records[low].object;handle->cached=value;handle->frame=frame;
  }
 }
 if(asserted&&!value){
  if(!mode)return -2;const auto level=*mode;if(level==2)return -3;
  if(level==1){if(!services||!services->invoke)return -2;WorldAiRequestV1 request{world_ai_assert,49,0,0,nullptr};WorldAiResponseV1 response{};if(services->invoke(services->context,&request,&response))return -2;}
 }
 *out=value;return 0;
}
}
