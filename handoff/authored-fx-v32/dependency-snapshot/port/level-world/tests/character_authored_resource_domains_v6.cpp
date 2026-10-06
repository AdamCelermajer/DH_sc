#include "../character_authored_resource_v6.hpp"
#include "../character_authored_fx_forces_v4.hpp"
#include "../source_fx_node_matrix_v4.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace dh2;
static bool camera(void*,float* m,float* p,std::string&){const float id[16]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};std::copy_n(id,16,m);p[0]=17;p[1]=29;p[2]=500;return true;}
static bool driver(void*,std::uint32_t& out,std::string&){out=8;return true;}
int main(int argc,char** argv){if(argc!=2)return 1;const std::string dir=argv[1];std::ifstream list(dir+"/resources.txt");if(!list)return 1;unsigned total=0,accepted=0,frames=0;std::string name;
 while(std::getline(list,name)){if(!name.empty()&&name.back()=='\r')name.pop_back();++total;std::ifstream f(dir+"/"+name,std::ios::binary);if(!f){std::cerr<<"Missing actual test resource "<<dir<<'/'<<name<<std::endl;return 1;}auto raw=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());
  fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;fx::CharacterAuthoredResourceFactoryV6 factory({nullptr,camera,driver},forces.factory());auto api=factory.factory();std::shared_ptr<fx::CharacterParticleFxResourceV2> base;scene::Scene live;std::string e;
  bool ok=api.create(api.context,raw,live,base,e);auto resource=std::dynamic_pointer_cast<fx::CharacterAuthoredCompositeFxResourceV4>(base);
  unsigned meshes=0,clouds=0;math::Matrix4f outer{};const float pos[3]{113,227,331},rot[3]{.1f,.2f,.3f},scale[3]{1.2f,.9f,1.1f};fx::source_fx_trs_matrix_v4(outer,pos,rot,scale);
  if(ok&&!resource){ok=false;e="Required same composite receiver";}
  for(int ms=0;ok&&ms<600;ms+=32){ok=resource->sample_animation(std::min(ms,resource->end_ms()),e)&&resource->source_scene_frame_v4(ms,32,outer,e);if(!ok)break;
   std::vector<fx::CharacterFxMeshDrawSourceV4> m;std::vector<fx::CharacterParticleDrawSourceV3> p;ok=resource->mesh_draw_sources_v4(m,e)&&resource->draw_sources_v3(p,e);meshes=std::max(meshes,unsigned(m.size()));clouds=std::max(clouds,unsigned(p.size()));
   for(const auto& s:m)for(const auto& v:s.part.positions)if(!std::isfinite(v[0])||!std::isfinite(v[1])||!std::isfinite(v[2])){ok=false;e="Nonfinite actual source mesh vertex";}
   if(ok)++frames;
  }
  if(ok)++accepted;std::cout<<"resource "<<name<<" | "<<(ok?"PASS":"REQUIRED")<<" | meshes "<<meshes<<" | clouds "<<clouds<<" | "<<e<<'\n';
 }
 std::cout<<"{\"resources\":"<<total<<",\"accepted_cpu_domains\":"<<accepted<<",\"source_frames\":"<<frames<<",\"gpu_submission\":false,\"live_acceptance\":false}"<<std::endl;return total==45?0:1;
}
