#include "../particle_billboard_v1.hpp"
#include <cstring>
extern "C" int dh2_particle_billboard_test_v1(const void* input,void* output){
 using namespace dh2::animation;const auto* bytes=static_cast<const unsigned char*>(input);float view[16];ParticleSeed100 p;std::memcpy(view,bytes,64);std::memcpy(p.data(),bytes+64,100);ParticleBillboardBasisV1 basis;float corners[12];int rc=dh2_particle_billboard_basis_v1(&basis,view);if(rc)return rc;rc=dh2_particle_billboard_corners_v1(corners,&basis,&p);if(rc)return rc;std::memcpy(output,&basis,24);std::memcpy(static_cast<unsigned char*>(output)+24,corners,48);return 72;
}
extern "C" int dh2_particle_billboard_apply_test_v1(const void* input,void* output){
 using namespace dh2::animation;const auto* b=static_cast<const unsigned char*>(input);unsigned n,local;float camera[3],world[16];std::memcpy(&n,b,4);if(n>30)return -1;std::memcpy(camera,b+4,12);std::memcpy(world,b+16,64);std::memcpy(&local,b+80,4);ParticleSeed100 p[30];std::memcpy(p,b+84,n*100);ParticleBillboardBoundsV1 bounds;int rc=dh2_particle_billboard_apply_v1(p,n,camera,world,local!=0,&bounds);if(rc)return rc;std::memcpy(output,&bounds,24);std::memcpy(static_cast<unsigned char*>(output)+24,p,n*100);return 24+n*100;
}
