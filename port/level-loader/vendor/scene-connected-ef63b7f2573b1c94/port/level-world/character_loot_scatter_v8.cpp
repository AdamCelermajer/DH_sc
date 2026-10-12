#include "character_loot_scatter_v8.hpp"
namespace dh2::character {
bool character_loot_scatter_v8(data::LootRandom8V2& random,const float source[3],const float* killer,float out[3],const LootScatterServicesV8& services,std::string& e){
 e.clear();if(!source||!out){e="Required source position and drop destination backing";return false;}
 std::int32_t draw{};
 if(!killer){
  out[0]=source[0];out[1]=source[1];out[2]=source[2];
  if(dh2_loot_v2_random(&random,500,&draw)){e="Required shared source scatter Random";return false;}
  out[0]=float(draw-250)+out[0];
  if(dh2_loot_v2_random(&random,500,&draw)){e="Required shared source scatter Random";return false;}
  out[1]=out[1]+float(draw-250);return true;
 }
 float direction[3]{killer[0]-source[0],killer[1]-source[1],killer[2]-source[2]};
 if(!services.normalize){e="Required original Point3D Normalize34d0b0";return false;}
 if(!services.normalize(services.context,direction,e))return false;
 if(!services.vec3f_k){e="Required actual source Vec3f_K global";return false;}
 const float x=direction[0],y=direction[1],z=direction[2];
 const float kx=services.vec3f_k[0],ky=services.vec3f_k[1],kz=services.vec3f_k[2];
 if(dh2_loot_v2_random(&random,200,&draw)){e="Required shared source scatter Random";return false;}
 const float along=float(draw+150);direction[0]=direction[0]*along;direction[1]=direction[1]*along;direction[2]=direction[2]*along;
 if(dh2_loot_v2_random(&random,300,&draw)){e="Required shared source scatter Random";return false;}
 const float across=float(draw-150);
 // Keep original scalar subtraction/multiply/add order (no fused operations).
 const float cross_y=(z*kx)-(kz*x);const float cross_z=(ky*x)-(y*kx);const float cross_x=(y*kz)-(z*ky);
 const float final_y=(across*cross_y+direction[1])+source[1];
 const float final_z=(across*cross_z+direction[2])+source[2];
 const float final_x=(across*cross_x+direction[0])+source[0];
 out[2]=final_z;out[0]=final_x;out[1]=final_y;return true;
}
}
