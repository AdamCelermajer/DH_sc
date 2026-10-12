#include "navigation_producers.hpp"
#include <cmath>
namespace {
using namespace dh2::navigation;
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
}
extern "C" int dh2_nav_producer_traits(ObstacleTraits* out,std::uint32_t type){
 if(!out||type>4)return 1;
 *out=type==0?ObstacleTraits{0,0,0,0}:type==1?ObstacleTraits{1,50,20,0}:ObstacleTraits{1,150,10,0};return 0;
}
extern "C" int dh2_nav_update_game_object(const ProducerRequest* r){
 if(!r||!r->object||r->object->reserved)return 1;
 // The original gate is the embedded PFObject user pointer, not a separate
 // object-present boolean. No virtual dispatch or radius update follows it.
 if(!r->object->user)return 0;
 if(!r->fields||!r->key)return 1;
 const auto& fields=*r->fields;ObstacleTraits traits{};
 if(fields.reserved||fields.physical_present>1||dh2_nav_producer_traits(&traits,std::uint32_t(fields.type)))return 1;
 float radius;
 if(fields.physical_present){
  if(!std::isfinite(fields.physical_radius))return 1;
  radius=mul(fields.physical_radius,100);
 }else{
  for(unsigned i=0;i<2;++i)if(!std::isfinite(fields.minimum[i])||!std::isfinite(fields.maximum[i]))return 1;
  const float x=sub(fields.maximum[0],fields.minimum[0]);
  const float y=sub(fields.maximum[1],fields.minimum[1]);
  radius=mul(x<y?y:x,.5f);
 }
 if(!std::isfinite(radius)||radius<0)return 1;
 auto next=*r->object;
 if(traits.obstacle){
  // These historical object fields were named weight/extent before their
  // GameObject producers were traced. They hold obstacle radius/strength.
  const ObstacleInitRequest init{r->geometry,r->registry,&next,r->key,
                                traits.radius,traits.strength,fields.physical_present,0};
  const int status=dh2_nav_init_obstacle(&init);if(status)return status;
 }
 next.radius=radius;*r->object=next;return 0;
}
