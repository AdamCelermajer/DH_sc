#include "../character_authored_resource_v32.hpp"
#include "../character_authored_fx_forces_v4.hpp"
#include "../authored_fx_alignment_v39.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
using namespace dh2;
static unsigned checks,mesh_frames,cloud_frames;
static void check(bool v,const std::string& e){++checks;if(!v)throw std::runtime_error(e);}
struct Camera{float view[16]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};};
static bool camera(void* raw,float* m,float* p,std::string&){std::copy_n(static_cast<Camera*>(raw)->view,16,m);p[0]=17;p[1]=29;p[2]=500;return true;}
static bool driver(void*,std::uint32_t& out,std::string&){out=8;return true;}
static void exact(const math::Matrix4f& expected,const std::array<float,16>& actual,const char* e){for(unsigned i=0;i<16;++i)check(expected.m[i]==actual[i],e);}
int main(int argc,char** argv){try{
 check(argc==2,"actual cache directory required");std::string error;const std::string dir=argv[1];
 for(const char* filename:{"asset_38.bdae","asset_42.bdae","asset_43.bdae"})for(unsigned heading=0;heading<8;++heading){
  std::ifstream f(dir+"/"+filename,std::ios::binary);check(bool(f),filename);auto bytes=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());
  Camera c;fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;fx::CharacterAuthoredResourceFactoryV32 factory({&c,camera,driver},forces.factory());auto api=factory.factory();scene::Scene live;std::shared_ptr<fx::CharacterParticleFxResourceV2> base;
  check(api.create(api.context,bytes,live,base,error),error);auto r=std::dynamic_pointer_cast<fx::CharacterAuthoredCompositeFxResourceV4>(base);check(bool(r),"actual composite owner");
  math::Matrix4f outer{};const float position[]{113,227,331},rotation[]{heading%2?.15f:0.f,heading%3?.2f:0.f,float(heading)*.785398185f},scale[]{1.2f,.9f,1.1f};fx::source_fx_trs_matrix_v4(outer,position,rotation,scale);
  for(int ms=0;ms<=256;ms+=32){check(r->sample_animation(std::min(ms,r->end_ms()),error),error);check(r->source_scene_frame_v4(ms,32,outer,error),error);
   std::vector<fx::CharacterFxMeshDrawSourceV4> before;check(r->mesh_draw_sources_v4(before,error),error);
   for(const auto& s:before){math::Matrix4f node{},expected{};check(fx::source_fx_node_world_matrix_v4(*s.scene,s.node,node,error),error);fx::source_fx_matrix_multiply_v4(expected,outer,node);
    // Source skin palette already owns graph matrices. Rigid receivers carry
    // outer*node; skinned receivers use outer after deformation.
    bool rigid=true;for(unsigned i=0;i<16;++i)rigid&=expected.m[i]==s.part.world[i];if(!rigid)exact(outer,s.part.world,"source skin owner-only transform");else exact(expected,s.part.world,"source node transform once");++mesh_frames;
   }
   std::vector<fx::CharacterParticleDrawSourceV3> particle_before;check(r->draw_sources_v3(particle_before,error),error);
   // Change camera orientation only, keeping same owner pose and time. Mesh
   // and weapon-style trail matrices must not follow camera orientation.
   math::Matrix4f cam{};const float zero[]{0,0,0},camrot[]{.1f,0,float(heading)*.31f+float(ms)*.001f},one[]{1,1,1};fx::source_fx_trs_matrix_v4(cam,zero,camrot,one);std::copy_n(cam.m,16,c.view);
   check(r->source_scene_frame_v4(ms,0,outer,error),error);std::vector<fx::CharacterFxMeshDrawSourceV4> after;check(r->mesh_draw_sources_v4(after,error)&&after.size()==before.size(),error);
   for(unsigned i=0;i<after.size();++i){check(after[i].part.world==before[i].part.world,"camera mutated source mesh/trail matrix");check(after[i].part.positions==before[i].part.positions,"camera mutated authored mesh/trail geometry");}
   std::vector<fx::CharacterParticleDrawSourceV3> p;check(r->draw_sources_v3(p,error)&&p.size()==particle_before.size(),error);for(unsigned cloud=0;cloud<p.size();++cloud){const auto& s=p[cloud];math::Matrix4f id{};check(fx::authored_fx_draw_world_v39(id,fx::AuthoredPositionSpaceV39::particle_world,nullptr,nullptr,error),error);exact(id,s.part.world,"particle world transform applied twice");check(s.part.positions.size()%4==0,"source billboard4 vertices");const auto& before_positions=particle_before[cloud].part.positions;check(s.part.positions.size()==before_positions.size(),"same source time changed particle count");
    for(unsigned v=0;v<s.part.positions.size();v+=4)for(unsigned k=0;k<3;++k){float center=0,old_center=0;for(unsigned corner=0;corner<4;++corner){center+=s.part.positions[v+corner][k]*.25f;old_center+=before_positions[v+corner][k]*.25f;}check(std::abs(center-old_center)<.002f,"camera displaced source world-space particle center");}++cloud_frames;}
  }
 }
 // The common contract rejects accidental outer application to world-space
 // particles and repeated socket/node multiplication after source skinning.
 math::Matrix4f id{},out{},old{};check(fx::authored_fx_draw_world_v39(id,fx::AuthoredPositionSpaceV39::particle_world,nullptr,nullptr,error),error);old=out;
 check(!fx::authored_fx_draw_world_v39(out,fx::AuthoredPositionSpaceV39::particle_world,&id,nullptr,error)&&!std::memcmp(&old,&out,sizeof(out)),"particle double-transform guard/atomic output");
 check(!fx::authored_fx_draw_world_v39(out,fx::AuthoredPositionSpaceV39::owner_local_skinned,&id,&id,error),"skin node applied twice");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_resources\":3,\"headings\":8,\"mesh_frames\":"<<mesh_frames<<",\"cloud_frames\":"<<cloud_frames<<",\"camera_and_actor_poses\":\"fixtures\",\"gpu_or_live_visual_proof\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
