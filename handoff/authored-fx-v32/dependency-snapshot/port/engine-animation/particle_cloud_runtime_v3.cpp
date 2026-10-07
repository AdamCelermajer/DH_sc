#include "particle_cloud_runtime_v3.hpp"
#include <cmath>
#include <utility>
namespace dh2::animation {
ParticleCloudRuntimeV3::ParticleCloudRuntimeV3(ParticleEmissionOwner& e,ParticleCloudModelsV1& m,std::int32_t& seed,std::int32_t initial,ParticleCloudRenderV1 render,ParticleCloudEmitterV2 emitter,ParticleCloudForcesV3 forces):emission_(e),models_(m),seed_(seed),initial_seed_(initial),render_(std::move(render)),emitter_(emitter),forces_(forces){}
int ParticleCloudRuntimeV3::initialize(){
 seed_=initial_seed_;now_=last_=0;initialized_=false;
 int rc=emission_.initialize();if(rc)return rc;
 if(!render_.initialize)return -2;rc=render_.initialize();if(rc)return rc;initialized_=true;return 0;
}
int ParticleCloudRuntimeV3::update(float seconds,const ParticleSeed100& untouched,const ParticleCloudFrameV1& frame){
 if(!initialized_||!std::isfinite(seconds)||!frame.forces.empty())return -1;
 if(seconds-now_<0){int rc=initialize();if(rc)return rc;}
 last_=now_;now_=seconds;const float dt=now_-last_;ParticleEmissionRange8 range{};
 int rc=emission_.generate(now_,last_,untouched,range);if(rc)return rc;
 if(range.first>range.end)return -1;auto n=range.end-range.first;auto* p=emission_.particle(range.first);
 rc=dh2_particle_life_init_v1(p,n,&models_.life,dt,&seed_);if(rc)return rc;
 rc=dh2_particle_size_init_v1(p,n,&models_.size,&seed_);if(rc)return rc;
 if(!emitter_.initialize)return -2;rc=emitter_.initialize(emitter_.context,p,n,frame.world_matrix,frame.local_space,&seed_);if(rc)return rc;
 rc=dh2_particle_motion_init_v1(p,n,&models_.motion,frame.world_matrix,&seed_);if(rc)return rc;
 rc=dh2_particle_spin_init_v1(p,n,&models_.spin,&seed_);if(rc)return rc;
 auto count=std::uint32_t(emission_.particles().size());p=emission_.particle(0);
 rc=dh2_particle_life_apply_v1(p,count,dt);if(rc)return rc;
 count=std::uint32_t(emission_.compact_expired_source_v1());p=emission_.particle(0);
 rc=dh2_particle_color_fallback_v1(p,count,frame.scene_color);if(rc)return rc;
 rc=dh2_particle_size_apply_v1(p,count,&models_.size);if(rc)return rc;
 if(!forces_.apply)return -2;rc=forces_.apply(forces_.context,p,count,dt,&seed_);if(rc)return rc;
 rc=dh2_particle_motion_apply_v1(p,count,dt);if(rc)return rc;
 rc=dh2_particle_spin_apply_v1(p,count,dt);if(rc)return rc;
 if(!render_.apply)return -2;return render_.apply(p,count);
}
}
