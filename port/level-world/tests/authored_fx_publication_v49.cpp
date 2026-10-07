#include "../character_authored_resource_v32.hpp"
#include "../character_authored_fx_forces_v4.hpp"
#include "../source_fx_node_matrix_v4.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdexcept>
using namespace dh2;
static unsigned checks;
static void check(bool value,const std::string& error){++checks;if(!value)throw std::runtime_error(error);}
static bool camera(void*,float* m,float* p,std::string&){const float view[]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};std::copy_n(view,16,m);p[0]=17;p[1]=29;p[2]=500;return true;}
static bool driver(void*,std::uint32_t& out,std::string&){out=8;return true;}
int main(int argc,char** argv){try{
 check(argc==2,"actual cache directory required");std::string error;
 for(const char* filename:{"asset_38.bdae","asset_42.bdae","asset_43.bdae"}){
  std::ifstream f(std::string(argv[1])+"/"+filename,std::ios::binary);check(bool(f),filename);
  auto bytes=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());
  fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;fx::CharacterAuthoredResourceFactoryV32 factory({nullptr,camera,driver},forces.factory());auto api=factory.factory();scene::Scene live;std::shared_ptr<fx::CharacterParticleFxResourceV2> base;
  check(api.create(api.context,bytes,live,base,error),error);auto r=std::dynamic_pointer_cast<fx::CharacterAuthoredCompositeFxResourceV4>(base);check(bool(r),"same composite receiver");
  math::Matrix4f sampled{};const float p[]{113,227,331},rot[]{0,0,0},scale[]{1,1,1};fx::source_fx_trs_matrix_v4(sampled,p,rot,scale);
  check(r->sample_animation(64,error)&&r->source_scene_frame_v4(64,64,sampled,error),error);
  std::vector<fx::CharacterFxMeshDrawSourceV4> old;check(r->mesh_draw_sources_v4(old,error),error);check(!old.empty(),"actual authored meshes required");
  std::vector<fx::CharacterParticleDrawSourceV3> clouds;check(r->draw_sources_v3(clouds,error),error);
  for(unsigned heading=0;heading<8;++heading){
   math::Matrix4f current{};const float moved[]{p[0]+37,p[1]-19,p[2]+11},turned[]{.15f,.2f,float(heading)*.785398185f};fx::source_fx_trs_matrix_v4(current,moved,turned,scale);
   std::vector<fx::CharacterFxMeshDrawSourceV4> fresh;check(r->mesh_draw_sources_at_outer_v49(current,fresh,error)&&fresh.size()==old.size(),error);
   for(unsigned i=0;i<fresh.size();++i){math::Matrix4f node{},expected{};check(fx::source_fx_node_world_matrix_v4(*fresh[i].scene,fresh[i].node,node,error),error);fx::source_fx_matrix_multiply_v4(expected,current,node);
    // Exact draw contract is rigid current*node or already-skinned current.
    bool node_match=true,skin_match=true;for(unsigned k=0;k<16;++k){node_match&=fresh[i].part.world[k]==expected.m[k];skin_match&=fresh[i].part.world[k]==current.m[k];}
    check(node_match||skin_match,"current source root transform was not published");check(fresh[i].part.world!=old[i].part.world,"stale sampled outer survived draw");check(fresh[i].part.positions==old[i].part.positions,"draw-only publication changed animation geometry");
   }
   std::vector<fx::CharacterParticleDrawSourceV3> after;check(r->draw_sources_v3(after,error)&&after.size()==clouds.size(),error);for(unsigned i=0;i<after.size();++i){check(after[i].part.positions==clouds[i].part.positions,"draw-only publication moved/emitted world-space particles");check(after[i].part.world==clouds[i].part.world,"draw-only publication transformed particles twice");}
   std::vector<fx::CharacterFxMeshDrawSourceV4> unchanged;check(r->mesh_draw_sources_v4(unchanged,error)&&unchanged.size()==old.size(),error);for(unsigned i=0;i<old.size();++i)check(unchanged[i].part.world==old[i].part.world,"draw borrow mutated retained sample outer");
  }
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"resources\":3,\"headings\":8,\"draw_only\":true,\"live_visual_after_fix\":false}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
