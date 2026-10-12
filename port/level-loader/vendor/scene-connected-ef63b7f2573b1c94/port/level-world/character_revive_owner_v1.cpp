#include "character_revive_owner_v1.hpp"
#include <cstring>
extern "C" int dh2_character_revive_v1(dh2::character::CharacterReviveResultV1* result,
 const dh2::character::CharacterReviveBorrowV1* in,std::uintptr_t,
 std::uint32_t physical,const dh2::character::CharacterReviveServicesV1* services){
 if(!result||!in||!in->identity||!in->life||!in->remote118||!in->network110||!in->network114||physical>255)return -1;
 auto f=*in;*result={};
 auto call=[&](std::uint32_t entry,std::uint32_t& word,std::uint32_t a=0,std::uint32_t b=0,std::uintptr_t payload=0){
  ++result->calls;result->last_entry=entry;word=0;
  const dh2::character::CharacterReviveRequestV1 request{entry,a,b,0,f.identity,payload};
  return services&&services->invoke&&services->invoke(services->context,&request,&word)==0;
 };
 std::uint32_t word{};
 if(f.life->dead&&!call(0x3a4d5c,word,3))return -2;
 f.life->low_health_armed=1;f.life->dead=0;
 if(!call(0x3b3a70,word))return -2;
 *f.remote118=0;
 if(!call(0x7fd794,word))return -2; // actual Application session byte5.
 if(word){*f.network110=-1;*f.network114=0;}
 if(physical&&!call(0x3b4088,word))return -2;
 if(!call(0x3a49f0,word))return -2;
 if(word){
  if(!call(0x7fd794,word))return -2;
  if(!word){
   if(!f.respawn_position1474)return -2;
   float position[3];std::memcpy(position,f.respawn_position1474,sizeof position);
   if(!call(0x525508,word,0,0,reinterpret_cast<std::uintptr_t>(position))||
      !call(0x393db4,word,1,0,reinterpret_cast<std::uintptr_t>(position)))return -2;
  }
 }
 if(!call(0x3d8894,word))return -2;
 result->completed=1;return 1;
}
