#include "character_authored_fx_forces_v4.hpp"
#include "source_fx_node_matrix_v4.hpp"
namespace dh2::fx {namespace {
struct BoundRuntimeV4{animation::ParticleBoundForcesV4 owner;};
}
bool CharacterAuthoredFxForceFactoryOwnerV4::create(void* raw,const resources::BresView& image,
 const scene::Scene& scene,std::shared_ptr<void> pin,std::uint32_t emitter,CharacterFxBoundForcesV4& out,std::string& error){
 out={};if(!raw||!pin){error="Required same retained force Scene lease";return false;}
 auto& factory=*static_cast<CharacterAuthoredFxForceFactoryOwnerV4*>(raw);auto world=factory.worlds_[&scene].lock();
 if(!world){animation::ParticleForceWorldServicesV4 services{};services.owner=std::move(pin);
  services.node_world=[](void*,const scene::Scene& actual,std::uint32_t node,math::Matrix4f& matrix,std::string& detail){return source_fx_node_world_matrix_v4(actual,node,matrix,detail);};
  world=std::make_shared<animation::ParticleForceWorldOwnerV4>(scene,std::move(services));factory.worlds_[&scene]=world;
 }
 auto bound=std::make_shared<BoundRuntimeV4>();if(!bound->owner.decode(image,scene,emitter,std::move(world),error))return false;
 out.lease=bound;out.context=bound.get();out.apply=[](void* pointer,animation::ParticleSeed100* particles,std::uint32_t count,float dt,std::int32_t* seed,const math::Matrix4f& outer,std::string& detail){
  auto& receiver=*static_cast<BoundRuntimeV4*>(pointer);return receiver.owner.update_outer(outer,detail)&&receiver.owner.apply(particles,count,dt,seed,detail)?0:-1;
 };return true;
}
}
