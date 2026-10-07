#include "character_update_queued.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool valid(UpdateStartupOwner64* owner,std::uint32_t* stage){
 return aligned(owner)&&aligned(stage)&&aligned(owner->eligibility)&&owner->eligibility->identity
  &&owner->reserved[0]==0&&owner->reserved[1]==0&&owner->reserved[2]==0&&owner->reserved_word==0;
}
int call(UpdateStartupOwner64* owner,const UpdateStartupServices24* services,std::uint32_t op,
 UpdateStartupResponse16& result,std::uint32_t arg=0,std::uint32_t arg2=0,
 std::uintptr_t subject=0,const char* name=nullptr){
 if(!services)return 2;
 if(!aligned(services)||services->reserved)return 1;
 if(!services->invoke||!(services->available&(1u<<op)))return 2;
 const UpdateStartupRequest40 request{op,arg,arg2,0,owner->eligibility->identity,subject,name};
 result={};if(services->invoke(services->context,owner,&request,&result))return 3;
 return result.reserved?1:0;
}
int debug(UpdateStartupOwner64* owner,const UpdateStartupServices24* services,const char* literal,std::uint32_t& value){
 UpdateStartupResponse16 result{};int status=call(owner,services,update_debug_load,result);if(status)return status;
 {
  char name[16]{};for(std::size_t i=0;literal[i];++i)name[i]=literal[i];
  status=call(owner,services,update_debug_construct,result,0,0,0,name);if(status)return status;
  status=call(owner,services,update_debug_query,result,0,0,0,name);if(status)return status;
  const auto captured=result.word;
  status=call(owner,services,update_debug_destroy,result,0,0,0,name);if(status)return status;
  value=captured;return 0;
 }
}
int player(const UpdateStartupResponse16& response,UpdateStartupPlayer24*& p){
 p=reinterpret_cast<UpdateStartupPlayer24*>(response.identity);
 return aligned(p)&&p->identity&&p->reserved[0]==0&&p->reserved[1]==0&&p->reserved[2]==0?0:1;
}
}
extern "C" int dh2_character_update_queued(UpdateStartupOwner64* owner,
 const UpdateStartupServices24* services,const UpdateQueuedBinding24* binding,std::uint32_t* stage){
 if(!valid(owner,stage)||!aligned(binding)||!binding->queue||!aligned(binding->actor)
  ||binding->actor->identity!=owner->eligibility->identity)return 1;
 UpdateStartupResponse16 result{};std::uint32_t enabled=0;
 int status=debug(owner,services,"KillPlayerOne",enabled);if(status)return status;
 if(enabled){
  status=call(owner,services,update_get_player,result,0,0);if(status)return status;
  UpdateStartupPlayer24* current=nullptr;status=player(result,current);if(status)return status;
  if(owner->eligibility->identity==current->character){
   const auto hp=owner->resolved_hp;
   const auto value=hp>0?static_cast<std::uint32_t>(hp)-200u:0u;
   status=call(owner,services,update_set_property,result,36,value);if(status)return status;
   if(hp<=0){
    status=call(owner,services,update_is_dead,result);if(status)return status;
    if(!result.word){status=call(owner,services,update_cmd_kill,result,0,0,owner->controller);if(status)return status;}
   }
  }
 }
 status=debug(owner,services,"Give50Potions",enabled);if(status)return status;
 if(enabled){
  status=call(owner,services,update_is_player,result);if(status)return status;
  if(result.word){status=call(owner,services,update_set_potions,result,50);if(status)return status;}
 }
 std::uint32_t eligible=0;status=dh2_character_can_update(owner->eligibility,owner->eligibility_services,&eligible);if(status)return status;
 if(!eligible){*stage=update_ineligible;return 0;}
 status=call(owner,services,update_get_state,result);if(status)return status;
 if(result.word){
  status=call(owner,services,update_get_state,result);if(status)return status;
  if(result.word!=12){
   status=call(owner,services,update_get_state,result);if(status)return status;
   if(result.word!=2&&!owner->eligibility->interaction&&!owner->active_ai){
    status=call(owner,services,update_get_current_level,result);if(status)return status;
    status=dh2_character_deferred_queue_evict(binding->queue,result.word,binding->queue_services);if(status)return status;
    status=call(owner,services,update_load_and_init,result,1);if(status)return status;
    if(result.word){
     status=call(owner,services,update_is_monster,result);if(status)return status;
     if(result.word){
      status=call(owner,services,update_is_miniboss,result);if(status)return status;
      if(!result.word){
       status=call(owner,services,update_is_boss,result);if(status)return status;
       if(!result.word){
        status=call(owner,services,update_online,result);if(status)return status;
        if(result.word>255)return 1;
        if(!result.word&&owner->delayed_load){
         status=call(owner,services,update_real_time,result);if(status)return status;
         std::int32_t time;std::memcpy(&time,&result.word,sizeof time);
         status=dh2_character_deferred_queue_assign(binding->queue,time,binding->actor,nullptr);if(status)return status;
        }
       }
      }
     }
    }
   }
  }
 }
 if(!aligned(owner->application_updates))return 1;
 ++*owner->application_updates;
 *stage=update_controller_ready;return 0;
}
