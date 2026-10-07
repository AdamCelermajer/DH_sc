#include "../character_authored_particle_fx_v3.hpp"
#include "../../engine-animation/material_color_v3.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <stdexcept>
using namespace dh2;using Raw=std::vector<std::uint8_t>;
static unsigned checks=0;static void require(bool b,const std::string& e){++checks;if(!b)throw std::runtime_error(e);}
static Raw read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error(p);return {std::istreambuf_iterator<char>(f),{}};}
static bool camera(void*,float* v,float* p,std::string&){const float m[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};std::copy_n(m,16,v);p[0]=17;p[1]=29;p[2]=500;return true;}
static bool driver(void*,std::uint32_t& v,std::string&){v=8;return true;}
int main(int argc,char** argv){try{require(argc==4,"three actual assets");std::string e;fx::CharacterAuthoredParticleFxFactoryV3 factory({nullptr,camera,driver});auto f=factory.factory();scene::Scene live;unsigned positive=0,vertices=0,changed=0;const std::array<float,16> outer={1,0,0,0,0,1,0,0,0,0,1,0,17,29,41,1};std::vector<skinning::VisualDrawPartV6> retained;
 for(int a=1;a<argc;++a){auto raw=std::make_shared<const Raw>(read(argv[a]));std::shared_ptr<fx::CharacterParticleFxResourceV2> resource;require(f.create(f.context,raw,live,resource,e),e);bool emitted=false;for(int ms=0;ms<3200;ms+=16){require(resource->sample_animation(std::min(ms,resource->end_ms()),e),e);require(resource->scene_frame(ms,16,outer,e),e);std::vector<fx::CharacterParticleDrawSourceV3> parts;require(resource->draw_sources_v3(parts,e),e);for(const auto& s:parts){emitted=true;++positive;vertices+=s.part.positions.size();require(s.resource_bytes==raw&&s.image->bytes==raw->data(),"same resource");require(s.source_node_render_fields_ready&&s.camera_offset_word==0&&s.rendering_layer==0,"source node constructor render fields");require(s.part.positions.size()%4==0&&s.part.geometry->primitives[0].indices.size()==s.part.positions.size()/4*6,"actual quads");const auto& material=s.part.material_table->at(s.material);if(a==3){require(material.id=="_1_-_Defaultjh","actual fire material");if(material.color[0]>0&&material.color[1]>0)++changed;require(material.color[2]==0&&material.color[3]>=254.f/255.f&&material.color[3]<=1,"actual fire color components");}for(const auto& p:s.part.positions)require(std::isfinite(p[0])&&std::isfinite(p[1])&&std::isfinite(p[2]),"finite source particles");retained.push_back(s.part);}}require(emitted,"positive authored emission");bool done=false;require(resource->completed(done,e)&&done,"authored emission drains");}
 require(changed>0,"actual fire material timeline changes");for(const auto& p:retained)require(p.geometry&&!p.geometry->positions.empty(),"retained snapshots");
 auto gold=read("port/engine-animation/reference/particle-cloud-models-v1/material-color-v3-gold.bin");std::uint32_t count;std::memcpy(&count,gold.data()+4,4);require(gold.size()==8+std::size_t(count)*48,"color gold");for(unsigned i=0;i<count;++i){auto p=gold.data()+8+i*48;std::uint32_t k,n;float frac;std::memcpy(&k,p,4);std::memcpy(&n,p+4,4);std::memcpy(&frac,p+8,4);animation::MaterialColorAccessorV3 accessor{p+12,8};std::uint8_t value[4];require(dh2_material_color_between_v3(value,&accessor,k,n,frac)==0&&!std::memcmp(value,p+44,4),"original color replay");}
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"positive_frames\":"<<positive<<",\"vertices\":"<<vertices<<",\"fire_color_frames\":"<<changed<<",\"gpu_submission\":false}"<<std::endl;return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
