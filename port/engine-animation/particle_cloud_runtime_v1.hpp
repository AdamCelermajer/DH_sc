#pragma once
#include "particle_cloud_models_v1.hpp"
#include <functional>
namespace dh2::animation {
struct ParticleCloudModelsV1 {
 ParticleLifeModelV1 life;
 ParticleSizeModelV1 size;
 ParticleMotionModelV1 motion;
 ParticleSpinModelV1 spin;
 ParticleSphereV1 sphere;
};
struct ParticleGravityBorrowV1 {const ParticleGravityV1* model;const float* matrix;};
struct ParticleCloudFrameV1 {
 const float* world_matrix;
 bool local_space;
 const std::uint32_t* scene_color;
 std::vector<ParticleGravityBorrowV1> forces;
};
// Retained renderer owns its real buffers/alpha-sort/BBox. These required
// continuations mutate the same particle vector; missing delivery is explicit.
struct ParticleCloudRenderV1 {
 std::function<int()> initialize;
 std::function<int(ParticleSeed100*,std::uint32_t)> apply;
};
class ParticleCloudRuntimeV1 {
 ParticleEmissionOwner& emission_;
 ParticleCloudModelsV1& models_;
 std::int32_t& seed_;
 const std::int32_t initial_seed_;
 ParticleCloudRenderV1 render_;
 float now_=0,last_=0;
 bool initialized_=false;
public:
 // References are sole source storage and must outlive this runtime. Initial
 // seed is the genuine factory constructor value, not emitter authored +24.
 ParticleCloudRuntimeV1(ParticleEmissionOwner&,ParticleCloudModelsV1&,std::int32_t&,
                        std::int32_t constructor_seed,ParticleCloudRenderV1);
 int initialize();
 // Current null animation-pointer PColor domain, including actual blood.
 // No scene color/force/world/renderer defaults are manufactured.
 int update(float seconds,const ParticleSeed100& untouched,const ParticleCloudFrameV1&);
 float current_time()const{return now_;}
 float last_time()const{return last_;}
};
// Register exact existing model storage into same source hash registry. This
// does not overwrite an existing unique parameter registration.
bool register_particle_cloud_models_v1(ParticleGenerationOwner&,ParticleCloudModelsV1&);
}
