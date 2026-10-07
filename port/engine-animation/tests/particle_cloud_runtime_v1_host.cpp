#include "../particle_cloud_runtime_v1.hpp"
#include "../particle_billboard_v1.hpp"
#include "../particle_force_scene_v1.hpp"
#include "../particle_scene_color_v1.hpp"
#include "../../scene-materials/particle_scene_v1.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cmath>
using namespace dh2::animation;
extern "C" unsigned dh2_particle_cloud_models_test_v1(unsigned,const unsigned char*,unsigned,unsigned char*);
extern "C" int dh2_particle_billboard_test_v1(const void*,void*);
extern "C" int dh2_particle_billboard_apply_test_v1(const void*,void*);
extern "C" int dh2_particle_cloud_graph_test_v1(const unsigned char*,unsigned char*);
static void require(bool v,const char* e){if(!v)throw std::runtime_error(e);}
static std::uint32_t word(const std::uint8_t* p){std::uint32_t w;std::memcpy(&w,p,4);return w;}
static float scalar(std::uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
static bool host_float_equal(const void* left,const void* right,unsigned bytes){const auto* a=static_cast<const std::uint8_t*>(left);const auto* b=static_cast<const std::uint8_t*>(right);for(unsigned i=0;i<bytes;i+=4){auto x=word(a+i),y=word(b+i);if(scalar(x)==scalar(y))continue;const auto rank=[](std::uint32_t q){return q&0x80000000u?~q:q|0x80000000u;};const auto rx=rank(x),ry=rank(y);if((rx>ry?rx-ry:ry-rx)>4)return false;}return true;}
static std::vector<std::uint8_t> file(const char* p){std::ifstream f(p,std::ios::binary);require(bool(f),"input");return {std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};}
int main(int argc,char** argv){try{
 require(argc==7,"arguments");unsigned checks=0;
 const auto gold=file(argv[1]);require(word(gold.data())==0x314c434d,"model gold magic");std::size_t at=8;
 for(unsigned i=0;i<word(gold.data()+4);++i){const auto op=word(gold.data()+at),n=word(gold.data()+at+4);at+=8;const auto* input=gold.data()+at;at+=n;const auto length=word(gold.data()+at);at+=4;std::vector<std::uint8_t> actual(length);require(dh2_particle_cloud_models_test_v1(op,input,n,actual.data())==length&&std::memcmp(actual.data(),gold.data()+at,length)==0,"model gold mismatch");at+=length;++checks;}
 const auto corners=file(argv[2]);require(corners.size()%236==0,"corner gold length");for(at=0;at<corners.size();at+=236){std::uint8_t output[72];require(dh2_particle_billboard_test_v1(corners.data()+at,output)==72&&host_float_equal(output,corners.data()+at+164,72),"corner gold mismatch");++checks;}
 const auto render=file(argv[3]);for(at=0;at<render.size();){const auto n=word(render.data()+at);const auto input_length=84+n*100,output_length=24+n*100;std::vector<std::uint8_t> output(output_length);require(dh2_particle_billboard_apply_test_v1(render.data()+at,output.data())==int(output_length)&&std::memcmp(output.data(),render.data()+at+input_length,output_length)==0,"render gold mismatch");at+=input_length+output_length;++checks;}
 const auto graph=file(argv[6]);require(word(graph.data())==0x31475250,"graph gold magic");at=8;
 for(unsigned i=0;i<word(graph.data()+4);++i){const auto length=word(graph.data()+at);at+=4;const auto* input=graph.data()+at;at+=length;const auto output_length=word(graph.data()+at);at+=4;std::vector<std::uint8_t> output(output_length);require(dh2_particle_cloud_graph_test_v1(input,output.data())==int(output_length)&&std::memcmp(output.data(),graph.data()+at,output_length)==0,"whole graph gold mismatch");at+=output_length;++checks;}
 unsigned frames=0,nonempty=0,peak=0;
 for(int asset=4;asset<6;++asset){auto raw=std::make_shared<const std::vector<std::uint8_t>>(file(argv[asset]));ParticleEmitterInput authored;std::string error;require(decode_particle_emitter(raw,0,authored,error),"actual emitter");
  dh2::resources::BresView image{};require(dh2_bres_open(&image,raw->data(),raw->size())==dh2::resources::BresError::ok,"BRES");dh2::scene::Scene scene;require(dh2::scene::load_particle_scene_v1(image,scene,error),"actual scene");std::vector<ParticleGravitySceneV1> decoded;require(decode_particle_gravity_scene_v1(image,scene,decoded,error)&&decoded.size()==1,"actual gravity");
  auto read=[&](unsigned offset){require(offset+4<=raw->size(),"BRES span");return word(raw->data()+offset);};
  const auto visual=read(image.root_offset+156),nodes=read(visual+12);unsigned instance=0;const float* emitter_world=nullptr;for(unsigned j=0;j<read(visual+8);++j){auto node=nodes+80*j;const auto instances=read(node+68);for(unsigned k=0;k<read(node+64);++k)if(read(instances+8*k)==9){instance=read(instances+8*k+4);const auto name_offset=read(node);require(name_offset<raw->size(),"emitter node name");const auto* name=reinterpret_cast<const char*>(raw->data()+name_offset);require(std::memchr(name,0,raw->size()-name_offset)!=nullptr,"emitter name terminator");for(const auto& actual:scene.graph)if(actual.id==name){require(!emitter_world,"unique emitter node");emitter_world=actual.world.data();}}}
  require(emitter_world,"same actual emitter world matrix");
  std::vector<std::uint32_t> bindings;require(instance&&particle_gravity_bindings_v1(image,instance,scene,decoded,bindings,error)&&bindings.size()==1,"same scene force binding");
  ParticleCloudModelsV1 m;const auto& r=authored.record;m.life={scalar(r[10]),scalar(r[11])};m.size={scalar(r[12]),scalar(r[13]),scalar(r[14]),scalar(r[15])};const auto direction=r[19];m.motion={{scalar(read(direction)),scalar(read(direction+4)),scalar(read(direction+8))},scalar(read(direction+12)),scalar(r[16]),scalar(r[17])};m.spin={scalar(r[30]),scalar(r[31]),scalar(r[32]),scalar(r[33]),r[34],{0,0,0},0};const float center[3]{0,0,0};require(dh2_particle_sphere_construct_v1(&m.sphere,center,scalar(authored.shape[0]),0)==0,"actual sphere");
  auto generation=ParticleGenerationOwner::create({});require(register_particle_cloud_models_v1(*generation,m),"sole registry");generation->set_word(generation->hash_name("MaxParticles"),r[6]);generation->set_word(generation->hash_name("BirthRate"),r[8]);ParticleEmissionOwner emission(generation);
  auto animation=ParticleAnimationResource::create(raw->data(),raw->size(),error);require(animation&&animation->track_count()==1,"actual BirthRate animation");ParticleBirthRateBinding birth{animation,generation->parameter("BirthRate"),0,0};
  float view[16]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},camera[3]{0,0,20},uv[8];dh2_particle_billboard_uv_v1(uv);ParticleBillboardBasisV1 basis;require(dh2_particle_billboard_basis_v1(&basis,view)==0,"camera fixture producer");std::vector<ParticleBillboardVertexV1> vertices;ParticleBillboardBoundsV1 bounds;
  ParticleCloudRenderV1 renderer{[&](){vertices.clear();vertices.reserve(generation->generation().max_particles*4);return 0;},[&](ParticleSeed100* p,unsigned n){int rc=dh2_particle_billboard_apply_v1(p,n,camera,view,false,&bounds);if(rc)return rc;vertices.resize(n*4);for(unsigned i=0;i<n;++i){rc=dh2_particle_billboard_vertices_v1(vertices.data()+4*i,&basis,p+i,uv);if(rc)return rc;}return 0;}};
  std::int32_t seed=0;ParticleCloudRuntimeV1 runtime(emission,m,seed,123456789,renderer);require(runtime.initialize()==0&&seed==123456789,"whole init source seed");std::uint32_t color=0;require(particle_scene_color_v1(8,{},color)==0&&color==0xffffffff,"actual source GLES2 color");
  const auto& force=decoded[bindings[0]];ParticleGravityV1 gravity{force.strength,force.falloff,force.point_mode};ParticleCloudFrameV1 frame{emitter_world,false,&color,{{&gravity,force.matrix()->data()}}};ParticleSeed100 untouched;untouched.fill(0xcd);
  for(int ms=0;ms<=2000;ms+=10){require(birth.sample_apply(ms)==0,"actual BirthRate cached sample");require(runtime.update(float(ms)/1000,untouched,frame)==0,"actual blood full model graph");++frames;peak=std::max(peak,unsigned(emission.particles().size()));if(!vertices.empty())++nonempty;for(const auto& v:vertices){for(float f:v.position)require(std::isfinite(f),"finite baked vertex");require(v.color==color,"source color word");}}
  require(peak>0&&runtime.update(0,untouched,frame)==0&&seed==123456789,"source backwards frame reset");
  ParticleCloudRuntimeV1 unavailable(emission,m,seed,123456789,{});require(unavailable.initialize()==-2,"missing render init explicit");++checks;
 }
 require(nonempty>0,"positive actual blood frames");std::cout<<"{\"validation\":\"PASS\",\"gold_checks\":"<<checks<<",\"actual_blood_frames\":"<<frames<<",\"nonempty_baked_frames\":"<<nonempty<<",\"peak_particles\":"<<peak<<",\"sanitizer_findings\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
