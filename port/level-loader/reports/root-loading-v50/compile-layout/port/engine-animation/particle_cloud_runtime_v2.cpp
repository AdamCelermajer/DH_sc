// Versioned whole cloud update: same source models/order, typed real emitter.
#include "particle_cloud_runtime_v2.hpp"
#include <cstring>
#include <cmath>
#include <utility>
namespace dh2::animation {
ParticleCloudRuntimeV2::ParticleCloudRuntimeV2(ParticleEmissionOwner& e,ParticleCloudModelsV1& m,std::int32_t& seed,std::int32_t initial,ParticleCloudRenderV1 r,ParticleCloudEmitterV2 emitter):emission_(e),models_(m),seed_(seed),initial_seed_(initial),render_(std::move(r)),emitter_(emitter){}
int ParticleCloudRuntimeV2::initialize(){
 seed_=initial_seed_;now_=last_=0;initialized_=false;int rc=emission_.initialize();if(rc)return rc;
 // Size/Color/Motion/Spin/Life model init are original empty virtual methods;
 // sphere domain is already produced by the real authored emitter model.
 if(!render_.initialize)return -2;rc=render_.initialize();if(rc)return rc;initialized_=true;return 0;
}
int ParticleCloudRuntimeV2::update(float seconds,const ParticleSeed100& untouched,const ParticleCloudFrameV1& frame){
 if(!initialized_||!std::isfinite(seconds))return -1;
 if(seconds-now_<0){const int rc=initialize();if(rc)return rc;}
 last_=now_;now_=seconds;const float dt=now_-last_;ParticleEmissionRange8 range{};
 int rc=emission_.generate(now_,last_,untouched,range);if(rc)return rc;
 if(range.first>range.end)return -1;
 const auto n=range.end-range.first;auto* p=emission_.particle(range.first);
 rc=dh2_particle_life_init_v1(p,n,&models_.life,dt,&seed_);if(rc)return rc;
 // Actual PColor null/null init leaves bytes untouched and consumes no RNG.
 rc=dh2_particle_size_init_v1(p,n,&models_.size,&seed_);if(rc)return rc;
 if(!emitter_.initialize)return -2;rc=emitter_.initialize(emitter_.context,p,n,frame.world_matrix,frame.local_space,&seed_);if(rc)return rc;
 // Source PGravity init method has no per-particle writes.
 rc=dh2_particle_motion_init_v1(p,n,&models_.motion,frame.world_matrix,&seed_);if(rc)return rc;
 rc=dh2_particle_spin_init_v1(p,n,&models_.spin,&seed_);if(rc)return rc;
 // Whole original selected billboard initPRenderData 64c504 is bx lr.
 auto count=static_cast<std::uint32_t>(emission_.particles().size());p=emission_.particle(0);
 rc=dh2_particle_life_apply_v1(p,count,dt);if(rc)return rc;
 count=static_cast<std::uint32_t>(emission_.compact_expired_source_v1());p=emission_.particle(0);
 rc=dh2_particle_color_fallback_v1(p,count,frame.scene_color);if(rc)return rc;
 rc=dh2_particle_size_apply_v1(p,count,&models_.size);if(rc)return rc;
 for(const auto& force:frame.forces){rc=dh2_particle_gravity_apply_v1(p,count,force.model,force.matrix,dt);if(rc)return rc;}
 rc=dh2_particle_motion_apply_v1(p,count,dt);if(rc)return rc;
 rc=dh2_particle_spin_apply_v1(p,count,dt);if(rc)return rc;
 if(!render_.apply)return -2;return render_.apply(p,count);
}
}
