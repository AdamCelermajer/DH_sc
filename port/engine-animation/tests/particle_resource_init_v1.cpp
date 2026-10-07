#include "../particle_resource_init_v1.hpp"
#include "../particle_force_scene_v1.hpp"
#include "../../scene-materials/particle_scene_v1.hpp"
#include "../particle_scene_color_v1.hpp"
#include <fstream>
#include <iterator>
#include <map>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace dh2::animation;
namespace resources=dh2::resources;
struct Models {std::map<std::string,std::uint32_t> words;std::map<std::string,std::array<std::uint32_t,3>> vectors;unsigned renders{};
 static int emitter(void* p,ParticleGenerationOwner& o,std::uint32_t v){auto& s=*static_cast<Models*>(p);s.words["EmitterType"]=v;o.register_parameter(o.hash_name("EmitterType"),&s.words["EmitterType"]);return 0;}
 static int shape(void* p,ParticleGenerationOwner& o,const char* n,std::uint32_t v){auto& s=*static_cast<Models*>(p);s.words[n]=v;o.register_parameter(o.hash_name(n),&s.words[n]);return 0;}
 static int vector(void* p,ParticleGenerationOwner&,const char* n,const std::uint32_t v[3]){auto& s=*static_cast<Models*>(p);std::copy_n(v,3,s.vectors[n].begin());return 0;}
 static int render(void* p,ParticleGenerationOwner&,const ParticleEmitterInput&){++static_cast<Models*>(p)->renders;return 0;}
};
int main(int argc,char** argv){assert(argc==3);unsigned checks=0;for(int f=1;f<3;++f){std::ifstream file(argv[f],std::ios::binary);auto b=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());ParticleEmitterInput in;std::string error;assert(decode_particle_emitter(b,0,in,error));++checks;
 resources::BresView image{};assert(dh2_bres_open(&image,b->data(),b->size())==resources::BresError::ok);dh2::scene::Scene scene;assert(dh2::scene::load_particle_scene_v1(image,scene,error));std::vector<ParticleGravitySceneV1> forces;assert(decode_particle_gravity_scene_v1(image,scene,forces,error));assert(forces.size()==1&&forces[0].strength==1&&forces[0].falloff==0&&forces[0].point_mode==0&&forces[0].matrix());checks+=4;
 auto word=[&](unsigned p){std::uint32_t value;assert(p+4<=b->size());std::memcpy(&value,b->data()+p,4);return value;};auto vs=word(image.root_offset+156);auto nodes=word(vs+12);unsigned emitter_instance=0;for(unsigned j=0;j<word(vs+8);++j){auto node=nodes+80*j;auto instances=word(node+68);for(unsigned k=0;k<word(node+64);++k)if(word(instances+8*k)==9)emitter_instance=word(instances+8*k+4);}assert(emitter_instance);std::vector<std::uint32_t> bound;assert(particle_gravity_bindings_v1(image,emitter_instance,scene,forces,bound,error)&&bound.size()==1&&bound[0]==0);auto& matrix=scene.graph[forces[0].node_index].world;auto old=matrix[12];matrix[12]=123;assert(forces[0].matrix()->at(12)==123);matrix[12]=old;checks+=3;
 Models s;ParticleContextSeed92 seed{};auto o=ParticleGenerationOwner::create(seed);
 const char* keys[]{"Life","LifeVariation","TargetSize","SizeVariation","SizeGrowthTime","SizeFadeTime","Speed","SpeedVariation","DirectionVariation","AnimKeyMappingType","AnimOffset","AnimOffsetVariation","AnimLength","AnimLengthVariation","AnimScaleMultiplier","AnimScaleMultiplierVariation","SpinTime","SpinVariation","SpinPhase","SpinPhaseVariation","SpinAxisType","SpinAxisVariation"};
 for(auto key:keys){s.words[key]=0xdeadbeef;o->register_parameter(o->hash_name(key),&s.words[key]);}
 assert(initialize_particle_resource_v1(*o,in,{&s,Models::emitter,Models::shape},{&s,Models::vector,Models::render})==0);++checks;
 assert((s.renders==1&&s.vectors["SpinAxis"]==std::array<std::uint32_t,3>{}));++checks;
 for(unsigned i=0;i<8;++i){assert(s.words[keys[i]]==in.record[0x28/4+i]);++checks;}
 assert(s.words["AnimKeyMappingType"]==in.record[0x64/4]);++checks;
 assert(o->generation().max_particles==30);++checks;
 assert(s.words["SpinAxisVariation"]==0);++checks;
 assert(initialize_particle_resource_v1(*o,in,{&s,Models::emitter,Models::shape},{&s,Models::vector,nullptr})==-2);++checks;
 auto bad=in;bad.record[0x48/4]=7;auto before=s.words;assert(initialize_particle_resource_v1(*o,bad,{&s,Models::emitter,Models::shape},{&s,Models::vector,Models::render})==-1&&before==s.words);++checks;
 }std::uint32_t color=0;assert(particle_scene_color_v1(8,{},color)==0&&color==0xffffffffu);++checks;assert(particle_scene_color_v1(1,{},color)==-2&&color==0xffffffffu);++checks;ParticleSceneColorServicesV1 light{nullptr,[](void*,std::uint32_t* p){*p=0x12345678;return -6;}};assert(particle_scene_color_v1(3,light,color)==-6&&color==0x12345678);++checks;std::cout<<"PASS "<<checks<<" actual blood resource/model parameter checks\n";}
