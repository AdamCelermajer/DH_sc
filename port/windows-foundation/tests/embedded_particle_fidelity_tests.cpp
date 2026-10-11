#include "../../engine-animation/particle_color_tracks_v1.hpp"
#include "../../engine-animation/particle_box_v2.hpp"
#include "../../level-world/character_authored_resource_v32.hpp"
#include "../../engine-animation/particle_billboard_v1.hpp"
#include "../camera.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
using namespace dh2;
namespace {
void check(bool b,const char* s){if(!b)throw std::runtime_error(s);}
void cell(animation::ParticleSeed100& p,unsigned n,float v){std::memcpy(p.data()+n,&v,4);}
bool camera(void*,float* view,float* eye,std::string&){std::fill_n(view,16,0.f);view[0]=view[5]=view[10]=view[15]=1;std::fill_n(eye,3,0.f);return true;}
bool driver(void*,std::uint32_t& n,std::string&){n=8;return true;}
auto bytes(const std::string& name){std::ifstream in(name,std::ios::binary);check(bool(in),"fixture missing");return std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(in),std::istreambuf_iterator<char>());}
void clocks(){
 animation::ParticleColorTracksV1 c;c.mapping=1;c.offset=.1f;c.length=.8f;c.scale=2;
 animation::ParticleSeed100 p[2]{};cell(p[0],60,.25f);cell(p[1],60,.75f);cell(p[0],64,1);cell(p[1],64,1);
 int seed=123456789;check(c.initialize(p,2,&seed)==0&&seed==123456789,"unbound init consumes RNG");
 std::vector<int> times;c.sample=[&](auto&,int ms){times.push_back(ms);return 0;};
 check(c.initialize(p,2,&seed)==0&&seed==123456789,"zero color variation consumes RNG");
 check(c.apply(p,2,nullptr)==0&&times==std::vector<int>({500,1300}),"mapped particle ages are independent");
 c.mapping=0;times.clear();check(c.apply(p,2,nullptr)==0&&times==std::vector<int>({350,850}),"absolute age mapping ignores scale");
 c.offset_variation=1;c.length_variation=.5f;check(c.initialize(p,2,&seed)==0&&seed!=123456789,"nonzero variation consumes shared RNG");
 check(c.initialize(nullptr,1,&seed)!=0,"malformed initialization accepted");
 cell(p[0],60,NAN);check(c.apply(p,1,nullptr)!=0,"invalid age accepted");
 c.sample={};std::uint32_t white=0xffffffff;check(c.apply(p,1,&white)==0,"fallback unavailable");
}
void box(){
 animation::ParticleBoxV2 b{};float shape[]{1,2,862.5f};check(animation::particle_box_from_shape_v2(&b,shape)==0,"shape box");
 check(b.dimensions[0]==2&&b.dimensions[1]==862.5f&&b.dimensions[2]==1,"box axis mapping");
 float rotate[]{0,1,0,0,-1,0,0,0,0,0,1,0,10,20,30,1};check(animation::dh2_particle_box_transform_v2(&b,rotate)==0,"box world transform");
 check(b.edges[3]==-862.5f&&b.edges[1]==2&&b.edges[8]==1,"rotated box extents");
 check(animation::particle_box_from_shape_v2(&b,nullptr)!=0,"null box shape accepted");
}
void camera_boundary(){
 dh::foundation::CameraPose pose{{0,0,10},{0,0,0},{0,1,0},45};
 const auto view=dh::foundation::sourceCameraViewMatrix(pose);
 check(view[10]==-1&&view[14]==10,"source camera forward convention");
 animation::ParticleBillboardBasisV1 b{};check(animation::dh2_particle_billboard_basis_v1(&b,view.data())==0,"billboard basis");
 animation::ParticleSeed100 p{};float corners[12];check(animation::dh2_particle_billboard_corners_v1(corners,&b,&p)==0,"billboard corners");
 const float ux=corners[3]-corners[0],uy=corners[4]-corners[1],vx=corners[6]-corners[0],vy=corners[7]-corners[1];
 check(ux*vy-uy*vx>0,"source billboard is backface culled by OpenGL");
}
void resource(const std::string& path){
 fx::CharacterAuthoredResourceFactoryV32 factory({nullptr,camera,driver},{});auto create=factory.factory();
 std::shared_ptr<fx::CharacterParticleFxResourceV2> r;scene::Scene scene;std::string error;
 check(create.create(create.context,bytes(path),scene,r,error),error.c_str());
 auto* composite=dynamic_cast<fx::CharacterAuthoredCompositeFxResourceV4*>(r.get());check(composite,"composite required");
 math::Matrix4f world{};world.m[0]=world.m[5]=world.m[10]=world.m[15]=1;
 std::set<int> reds;float low=1e9f,high=-1e9f;unsigned peak=0;
 for(int step=1;step<=240;++step){
  // A clip loop/backward sample must not reset live particles on the absolute clock.
  check(r->sample_animation((step*16)%2000,error),error.c_str());
  check(composite->source_scene_frame_v4(step*16,16,world,error),error.c_str());
  std::vector<fx::CharacterParticleDrawSourceV3> sources;check(r->draw_sources_v3(sources,error),error.c_str());
  for(const auto& source:sources){
   check(source.part.material_table->at(source.material).color[0]==1.f,"animated material is not neutral");
   const auto& g=*source.part.geometry;const auto& color=g.attributes[g.primitives[0].attributes[2]].values;
   peak=std::max(peak,unsigned(g.positions.size()/4));
   for(unsigned i=0;i<g.positions.size();i+=4){
    float center=0;for(unsigned j=0;j<4;++j)center+=g.positions[i+j][1]/4;
    low=std::min(low,center);high=std::max(high,center);
    reds.insert(int(color[i*4]));check(color[i*4]>=color[i*4+1]&&color[i*4+1]>=color[i*4+2],"RGBA color channel order");
   }
  }
 }
 check(peak>=20,"particles reset at clip loops");check(high-low>600,"fire is not spread along authored Y line");check(reds.size()>10,"particle colors pulse together");
 // Fully malformed resource must fail cleanly, without retaining the old result.
 auto bad=std::make_shared<const std::vector<std::uint8_t>>(4,0);
 check(!create.create(create.context,bad,scene,r,error)&&!r,"malformed resource accepted");
}
}
int main(int argc,char** argv){try{check(argc==2,"fire model argument");clocks();box();camera_boundary();resource(argv[1]);std::cout<<"embedded particle fidelity PASS\n";return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
