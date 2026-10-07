#include "particle_bound_forces_v4.hpp"
#include <cstring>
namespace dh2::animation {
namespace {
float mul(float a,float b){volatile float v=a*b;return v;}
float add(float a,float b){volatile float v=a+b;return v;}
void multiply(const math::Matrix4f& a,const math::Matrix4f& b,math::Matrix4f& out){
 if(a.identity_hint){std::memcpy(&out,&b,65);return;}
 if(b.identity_hint){std::memcpy(&out,&a,65);return;}
 for(unsigned col=0;col<4;++col)for(unsigned row=0;row<4;++row){
  float value=mul(a.m[row],b.m[4*col]);value=add(value,mul(a.m[row+4],b.m[4*col+1]));
  value=add(value,mul(a.m[row+8],b.m[4*col+2]));out.m[4*col+row]=add(value,mul(a.m[row+12],b.m[4*col+3]));
 }
 out.identity_hint=0;
}
}
bool ParticleForceWorldOwnerV4::produce(std::uint32_t node,math::Matrix4f& out,std::string& error){
 if(!services_.owner||!services_.node_world){error="Required actual same-Scene force world68 producer";return false;}
 math::Matrix4f local{};if(!services_.node_world(services_.context,scene_,node,local,error))return false;
 if(outer_written_)multiply(outer_,local,out);else std::memcpy(&out,&local,65);return true;
}
bool ParticleForceWorldOwnerV4::matrix(std::uint32_t node,math::Matrix4f*& out,std::string& error){
 out=nullptr;if(node>=scene_.graph.size()){error="Required force node in SAME Scene";return false;}
 auto found=matrices_.find(node);
 if(found==matrices_.end()){math::Matrix4f value{};if(!produce(node,value,error))return false;found=matrices_.emplace(node,value).first;}
 out=&found->second;return true;
}
bool ParticleForceWorldOwnerV4::update_outer(const math::Matrix4f& actual,std::string& error){
 std::memcpy(&outer_,&actual,65);outer_written_=true;
 for(auto& entry:matrices_)if(!produce(entry.first,entry.second,error))return false;return true;
}
bool ParticleForceWorldOwnerV4::bindings(const resources::BresView& image,std::uint32_t emitter,std::vector<std::uint32_t>& indices,std::string& error){
 if(!declarations_attempted_){declarations_attempted_=true;image_bytes_=image.bytes;image_size_=image.size;
  declarations_ready_=decode_particle_force_scene_v2(image,scene_,forces_,error);if(!declarations_ready_)return false;
 }
 if(!declarations_ready_||image_bytes_!=image.bytes||image_size_!=image.size){error="Required SAME initialized particle force declarations";return false;}
 return particle_force_bindings_v2(image,emitter,scene_,forces_,indices,error);
}
bool ParticleBoundForcesV4::decode(const resources::BresView& image,const scene::Scene& scene,std::uint32_t emitter,
 std::shared_ptr<ParticleForceWorldOwnerV4> world,std::string& error){
 if(attempted_){error="Force proxy construction already attempted";return false;}attempted_=true;
 if(!world||&world->scene()!=&scene){error="Required SAME force Scene world owner";return false;}world_=std::move(world);
 std::vector<std::uint32_t> indices;if(!world_->bindings(image,emitter,indices,error))return false;
 bound_.reserve(indices.size());
 for(auto index:indices){bound_.push_back({index,{}});auto& bound=bound_.back();const auto& force=world_->force(index);
  math::Matrix4f* matrix{};if(!world_->matrix(force.node_index,matrix,error))return false;
  if(force.type==2&&dh2_particle_deflector_construct_v1(&bound.model,&force.deflector,matrix)){error="Required actual Deflector proxy constructor";return false;}
 }
 ready_=true;return true;
}
bool ParticleBoundForcesV4::update_outer(const math::Matrix4f& outer,std::string& error){
 if(!ready_){error="Required initialized source force bindings";return false;}return world_->update_outer(outer,error);
}
bool ParticleBoundForcesV4::apply(ParticleSeed100* particles,std::uint32_t count,float dt,std::int32_t* seed,std::string& error){
 if(!ready_){error="Required initialized source force proxies";return false;}
 for(auto& bound:bound_){const auto& force=world_->force(bound.force);math::Matrix4f* matrix{};
  if(!world_->matrix(force.node_index,matrix,error))return false;
  const int status=force.type==2?dh2_particle_deflector_apply_v1(particles,count,&bound.model,matrix,dt,seed):
   dh2_particle_gravity_apply_v1(particles,count,&force.gravity,matrix->m,dt);
  if(status){error="Required complete source particle force apply type "+std::to_string(force.type)+" status "+std::to_string(status);return false;}
 }
 return true;
}
}
