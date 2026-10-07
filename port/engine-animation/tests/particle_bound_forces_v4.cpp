#include "../particle_bound_forces_v4.hpp"
#include "../../scene-materials/particle_scene_v1.hpp"
#include "../../level-world/source_fx_node_matrix_v4.hpp"
#include <fstream>
#include <iterator>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <functional>
struct Asset {std::vector<std::uint8_t> bytes;dh2::scene::Scene scene;};
static std::uint32_t word(const std::vector<std::uint8_t>& bytes,std::size_t p){assert(p+4<=bytes.size());std::uint32_t n;std::memcpy(&n,bytes.data()+p,4);return n;}
static float number(const dh2::animation::ParticleSeed100& particle,unsigned at){float value;std::memcpy(&value,particle.data()+at,4);return value;}
int main(){
 using namespace dh2::animation;
 auto asset=std::make_shared<Asset>();std::ifstream stream("/data/local/tmp/dh2-bash-deflector-v1.bdae",std::ios::binary);assert(stream);
 asset->bytes.assign(std::istreambuf_iterator<char>(stream),{});dh2::resources::BresView image{};
 assert(dh2_bres_open(&image,asset->bytes.data(),asset->bytes.size())==dh2::resources::BresError::ok);
 std::string error;assert(dh2::scene::load_particle_scene_v1(image,asset->scene,error));
 std::vector<ParticleForceSceneV2> decoded;assert(decode_particle_force_scene_v2(image,asset->scene,decoded,error));
 assert(decoded.size()==2&&decoded[0].type==2&&decoded[1].type==0);
 assert(decoded[0].deflector.bounce==0.27f&&decoded[0].deflector.friction==0.5f&&decoded[0].deflector.width==1000.f&&decoded[1].gravity.strength==5.f);
 std::vector<std::uint32_t> emitters;const auto& bytes=asset->bytes;
 std::function<void(std::uint32_t)> visit=[&](std::uint32_t node){
  for(unsigned i=0;i<word(bytes,node+64);++i){const auto at=word(bytes,node+68)+8*i;if(word(bytes,at)==9)emitters.push_back(word(bytes,at+4));}
  for(unsigned i=0;i<word(bytes,node+56);++i)visit(word(bytes,node+60)+80*i);
 };
 const auto visual=word(bytes,image.root_offset+156);for(unsigned i=0;i<word(bytes,visual+8);++i)visit(word(bytes,visual+12)+80*i);
 assert(emitters.size()==2);
 ParticleForceWorldServicesV4 services;services.owner=asset;
 services.node_world=[](void*,const dh2::scene::Scene& scene,unsigned node,dh2::math::Matrix4f& out,std::string& e){return dh2::fx::source_fx_node_world_matrix_v4(scene,node,out,e);};
 auto world=std::make_shared<ParticleForceWorldOwnerV4>(asset->scene,services);
 ParticleBoundForcesV4 first,second;assert(first.decode(image,asset->scene,emitters[0],world,error));assert(second.decode(image,asset->scene,emitters[1],world,error));
 assert(first.size()==2&&second.size()==0);
 std::vector<std::uint32_t> order;assert(particle_force_bindings_v2(image,emitters[0],asset->scene,decoded,order,error));assert(order==std::vector<std::uint32_t>({0,1}));
 dh2::math::Matrix4f outer{};const float position[3]={0,0,0},rotation[3]={0,0,0},scale[3]={1,1,1};dh2::fx::source_fx_trs_matrix_v4(outer,position,rotation,scale);
 assert(first.update_outer(outer,error));dh2::math::Matrix4f* actual{};assert(world->matrix(decoded[0].node_index,actual,error));
 dh2::math::Matrix4f* same{};assert(world->matrix(decoded[0].node_index,same,error));assert(actual==same);
 dh2::math::Vector3f normal{actual->m[8],actual->m[9],actual->m[10]};dh2_vec3_normalize(&normal);const float n[3]={normal.x,normal.y,normal.z};
 ParticleSeed100 initial{};for(unsigned k=0;k<3;++k){const float p=actual->m[12+k]-n[k]*.5f,v=n[k]*20.f;std::memcpy(initial.data()+4*k,&p,4);std::memcpy(initial.data()+12+4*k,&v,4);}
 auto a=initial,b=initial;std::int32_t seed_a=123456789,seed_b=123456789;
 assert(first.apply(&a,1,.1f,&seed_a,error));assert(second.apply(&b,1,.1f,&seed_b,error));assert(b==initial&&seed_a==seed_b&&seed_a==123456789);
 assert(a!=initial);for(unsigned k=0;k<6;++k)assert(std::isfinite(number(a,k*4)));
 for(unsigned i=0;i<25;++i){assert(first.update_outer(outer,error));assert(first.apply(&a,1,.01f,&seed_a,error));}
 assert(!first.decode(image,asset->scene,emitters[0],world,error));
 ParticleBoundForcesV4 missing;auto unavailable=std::make_shared<ParticleForceWorldOwnerV4>(asset->scene,ParticleForceWorldServicesV4{});
 assert(!missing.decode(image,asset->scene,emitters[0],unavailable,error));
 std::cout<<"actual BashDown forces PASS: debris2/dust0/shared2nodes/orderedDeflectorGravity/sourceparams/finitecollision/history/missingproducer\n";
}
