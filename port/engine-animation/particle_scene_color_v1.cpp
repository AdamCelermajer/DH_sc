#include "particle_scene_color_v1.hpp"
namespace dh2::animation {
int particle_scene_color_v1(std::uint32_t driver,const ParticleSceneColorServicesV1& services,std::uint32_t& color){
 color=0xffffffffu;
 if(!(driver&7u))return 0;
 if(!services.query)return -2;
 return services.query(services.context,&color);
}
}
