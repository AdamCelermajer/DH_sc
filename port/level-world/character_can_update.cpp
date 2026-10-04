#include "character_can_update.hpp"
namespace {
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
using namespace dh2::character;
int query(CanUpdateOwner40* owner,const CanUpdateServices24* services,std::uint32_t op,
 CanUpdateResponse16& out){
 if(!services)return 2;
 if(!aligned(services)||services->reserved)return 1;
 if(!services->invoke||!(services->available&(1u<<op)))return 2;
 const CanUpdateRequest24 request{op,op==can_update_player?1u:0u,
                                owner->identity,op==can_update_culling?owner->bounds:0};
 out={};
 if(services->invoke(services->context,owner,&request,&out))return 3;
 if(out.reserved||(op==can_update_online&&out.word>255))return 1;
 return 0;
}
bool node(CanUpdateVisual8* visual){return aligned(visual)&&aligned(visual->node);}
}
extern "C" int dh2_character_can_update(CanUpdateOwner40* owner,
 const CanUpdateServices24* services,std::uint32_t* accepted){
 if(!aligned(owner)||!aligned(accepted)||!owner->identity||owner->reserved||owner->reserved_word)return 1;
 const auto out=reinterpret_cast<std::uintptr_t>(accepted);
 const auto base=reinterpret_cast<std::uintptr_t>(owner);
 if(out>=base&&out<base+sizeof(*owner))return 1;
 auto* const visual=owner->visual;
 if(visual){if(!node(visual))return 1;visual->node->animate_enabled=0;}
 CanUpdateResponse16 result{};
 int status=query(owner,services,can_update_online,result);if(status)return status;
 bool offline_path=true;
 if(result.word){
  status=query(owner,services,can_update_remote,result);if(status)return status;
  offline_path=!result.word;
 }
 if(offline_path){
  if(visual){
   // Source captures owner+418 before GetPlayer. The visual/node and force
   // fields used after its callback are reloaded, not precomputed predicates.
   const auto link=owner->player_link;
   status=query(owner,services,can_update_player,result);if(status)return status;
   if(link!=result.identity){
    if(!node(owner->visual))return 1;
    if(owner->visual->node->culling||owner->force_update){
     status=query(owner,services,can_update_culling,result);if(status)return status;
     if(!result.word&&!owner->interaction){*accepted=0;return 0;}
    }
   }
  }
  status=query(owner,services,can_update_dead,result);if(status)return status;
  if(result.word&&!owner->enabled){
   status=query(owner,services,can_update_respawn,result);if(status)return status;
   if(!result.word){*accepted=0;return 0;}
  }
 }
 if(visual&&owner->enabled){
  if(!node(visual))return 1;
  visual->node->animate_enabled=1;
 }
 *accepted=1;return 0;
}
