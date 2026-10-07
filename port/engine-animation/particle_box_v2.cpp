#include "particle_box_v2.hpp"
#include <cstring>
#include <algorithm>
namespace dh2::animation {
extern "C" int dh2_particle_box_construct_v2(ParticleBoxV2* out,const float* size){
 if(!out||!size)return -1;std::copy_n(size,3,out->dimensions);std::fill_n(out->edges,9,0.f);
 for(unsigned i=0;i<3;++i){out->minimum[i]=size[i]*-.5f;out->edges[i*3+i]=size[i];}return 0;
}
extern "C" int dh2_particle_box_transform_v2(ParticleBoxV2* out,const float* m){
 if(!out||!m)return -1;const float x=out->dimensions[0]*-.5f,y=out->dimensions[1]*-.5f,z=out->dimensions[2]*-.5f;
 for(unsigned row=0;row<3;++row){float v=x*m[row];v=v+y*m[row+4];v=v+z*m[row+8];out->minimum[row]=v+m[row+12];}
 for(unsigned axis=0;axis<3;++axis)for(unsigned row=0;row<3;++row){const float xaxis=axis==0?out->dimensions[0]:0.f,yaxis=axis==1?out->dimensions[1]:0.f,zaxis=axis==2?out->dimensions[2]:0.f;float v=xaxis*m[row];v=v+yaxis*m[row+4];v=v+zaxis*m[row+8];out->edges[axis*3+row]=v;}
 return 0;
}
extern "C" int dh2_particle_box_generate_v2(float* out,const ParticleBoxV2* b,std::int32_t* seed){
 if(!out||!b||!seed)return -1;const double a=dh2_particle_random_v1(seed),c=dh2_particle_random_v1(seed),d=dh2_particle_random_v1(seed);const float x=float(a),y=float(c),z=float(d);
 for(unsigned row:{1u,2u,0u}){float v=x*b->edges[row];v=v+b->minimum[row];v=v+y*b->edges[3+row];out[row]=v+z*b->edges[6+row];}return 0;
}
extern "C" int dh2_particle_box_emitter_init_v2(ParticleSeed100* p,std::uint32_t n,const ParticleBoxV2* box,const float* matrix,bool local,std::int32_t* seed){
 if(!box||!seed||n>65536||(n&&!p))return -1;auto transformed=*box;if(!local&&matrix&&dh2_particle_box_transform_v2(&transformed,matrix))return -1;
 for(unsigned i=0;i<n;++i){float xyz[3];if(dh2_particle_box_generate_v2(xyz,&transformed,seed))return -1;std::memcpy(p[i].data(),xyz,12);}return 0;
}
}
