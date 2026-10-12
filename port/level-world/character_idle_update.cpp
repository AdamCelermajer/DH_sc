#include "character_idle_update.hpp"
#include <cmath>
#include <cstring>
namespace {
using namespace dh2::character;
bool valid(const IdleCharacter40* c){return c&&!(reinterpret_cast<std::uintptr_t>(c)&7)&&c->identity&&c->heading_active<=255&&!c->reserved;}
std::int32_t signed_word(std::uint32_t word){std::int32_t result;std::memcpy(&result,&word,4);return result;}
bool call(const IdleUpdateServices16& s,IdleCharacter40* c,std::uint32_t op,std::uint32_t arg,std::uintptr_t subject,IdleUpdateResponse16& out,const float* p=nullptr){
 IdleUpdateRequest32 request{op,arg,subject,{0,0,0},0};if(p)std::memcpy(request.point,p,12);out={};return s.invoke(s.context,c,&request,&out)==0&&!out.reserved;
}
}
extern "C" int dh2_character_idle_update(dh2::character::IdleCharacter40* c,const dh2::character::IdleUpdateServices16* s){
 using namespace dh2::character;
 if(!valid(c)||!s||!s->invoke)return -1;
 IdleUpdateResponse16 response{};
 if(c->heading_active)return call(*s,c,idle_raise_event,0,c->identity,response)?1:-2;
 if(!call(*s,c,idle_is_player,0,c->identity,response))return -2;if(!response.word)return 1;
 if(!call(*s,c,idle_online_disabled,0,0,response))return -2;if(response.word&255)return 1;
 if(!call(*s,c,idle_delay,0,0,response))return -2;const std::uint32_t delay=response.word;if(delay>c->state_time_ms)return 1;
 if(!call(*s,c,idle_player_count,0,0,response))return -2;const auto manager=response.identity;const auto count=signed_word(response.word);if(count<=0)return 1;if(!manager)return -2;
 for(std::int32_t i=0;i<count;++i){
  if(!call(*s,c,idle_player_character,std::uint32_t(i),manager,response))return -2;
  auto* other=reinterpret_cast<IdleCharacter40*>(response.identity);if(!other||other==c)continue;if(!valid(other))return -2;if(other->identity==c->identity)continue;
  if(!call(*s,c,idle_get_state,0,other->identity,response))return -2;if(signed_word(response.word)!=3||delay>other->state_time_ms)continue;
  if(!call(*s,c,idle_distance,0,0,response))return -2;
  const float distance=float(signed_word(response.word));
  const float dx=c->position[0]-other->position[0],dy=c->position[1]-other->position[1],dz=c->position[2]-other->position[2];
  const float xx=dx*dx,yy=dy*dy,zz=dz*dz;const float xy=xx+yy;const float square=xy+zz;const float limit=distance*distance;
  if(!(limit>square)||!(square>0.0f))continue;
  const float length=std::sqrt(square);const float remaining=distance-length;const float scaled=remaining*0.75f;const float factor=scaled/length;
  const float x=factor*dx,y=factor*dy,z=factor*dz;
  float point[3];point[1]=y+c->position[1];point[2]=z+c->position[2];point[0]=x+c->position[0];
  if(!call(*s,c,idle_move_to,0,c->controller,response,point))return -2;
  point[1]=other->position[1]-y;point[2]=other->position[2]-z;point[0]=other->position[0]-x;
  if(!call(*s,c,idle_move_to,0,other->controller,response,point))return -2;
 }
 return 1;
}
