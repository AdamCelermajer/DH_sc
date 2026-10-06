#include "../character_authored_particle_fx_v4.hpp"
#include "../character_authored_fx_forces_v4.hpp"
#include "../source_fx_node_matrix_v4.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
using namespace dh2;
static unsigned checks;
static void check(bool b,const std::string& e){++checks;if(!b)throw std::runtime_error(e);}
static bool camera(void*,float* v,float* p,std::string&){const float a[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};std::copy_n(a,16,v);p[0]=17;p[1]=29;p[2]=500;return true;}
static bool driver(void*,std::uint32_t& v,std::string&){v=8;return true;}
int main(int argc,char** argv){try{
 check(argc==3,"actual BashDown and Charge cache paths required");
 fx::CharacterAuthoredFxForceFactoryOwnerV4 force;
 fx::CharacterAuthoredParticleFxFactoryV4 factory({nullptr,camera,driver},force.factory());auto api=factory.factory();
 std::vector<skinning::VisualDrawPartV6> retained;
 for(int a=1;a<argc;++a){std::ifstream in(argv[a],std::ios::binary);check(bool(in),argv[a]);
  auto raw=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(in),std::istreambuf_iterator<char>());
  scene::Scene live;std::string e;std::shared_ptr<fx::CharacterParticleFxResourceV2> base;
  check(api.create(api.context,raw,live,base,e),e);auto resource=std::dynamic_pointer_cast<fx::CharacterAuthoredCompositeFxResourceV4>(base);check(bool(resource),"same composite receiver");
  math::Matrix4f outer{};const float pos[3]={113,227,331},rot[3]={.1f,.2f,.3f},scale[3]={1.2f,.9f,1.1f};fx::source_fx_trs_matrix_v4(outer,pos,rot,scale);
  bool particles=false;unsigned mesh_count=0;
  for(int ms=0;ms<2000;ms+=16){check(resource->sample_animation(std::min(ms,resource->end_ms()),e),e);check(resource->source_scene_frame_v4(ms,16,outer,e),e);
   std::vector<fx::CharacterFxMeshDrawSourceV4> meshes;check(resource->mesh_draw_sources_v4(meshes,e),e);check(meshes.size()==unsigned(a==1?3:2),"actual mesh primitive bindings");mesh_count+=meshes.size();
   for(auto& m:meshes){check(m.resource_bytes==raw&&m.image&&m.scene&&m.node_identity&&m.source_texture_matrix68,"retained actual mesh metadata");check(m.part.geometry&&m.part.material_table&&m.part.retention,"retained actual geometry/material");for(auto& p:m.part.positions)check(std::isfinite(p[0])&&std::isfinite(p[1])&&std::isfinite(p[2]),"finite transformed source mesh");retained.push_back(m.part);}
   std::vector<skinning::VisualDrawPartV6> all;check(resource->draw_parts(all,e),e);if(all.size()>meshes.size())particles=true;
  }
  check(particles,"actual authored emitter generation produced particles");check(mesh_count>0,"authored meshes present before/after particle generation");
 }
 for(auto& p:retained)check(p.geometry&&p.material_table&&!p.positions.empty(),"snapshot survives resource receiver destruction");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"gpu_submission\":false,\"live_actor_binding\":false}"<<std::endl;return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
