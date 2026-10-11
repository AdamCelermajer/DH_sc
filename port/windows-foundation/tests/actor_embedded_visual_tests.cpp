#include "../original_character.hpp"
#include "../asset_catalog.hpp"
#include "../camera.hpp"
#include "../../level-world/character_authored_resource_v32.hpp"
#include <cstring>
#include <set>
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
bool same(const Mesh& a,const Mesh& b,float tolerance=0.002f){if(a.vertices.size()!=b.vertices.size())return false;for(unsigned i=0;i<a.vertices.size();++i){const auto& x=a.vertices[i].position;const auto& y=b.vertices[i].position;if(std::abs(x.x-y.x)>tolerance||std::abs(x.y-y.y)>tolerance||std::abs(x.z-y.z)>tolerance)return false;}return true;}
bool fx_camera(void*,float* view,float* eye,std::string&){CameraPose camera{{700,-900,1400},{0,0,270},{0,0,1},45};const auto m=sourceCameraViewMatrix(camera);std::copy(m.begin(),m.end(),view);eye[0]=700;eye[1]=-900;eye[2]=1400;return true;}
bool fx_driver(void*,std::uint32_t& d,std::string&){d=8;return true;}
void actor_particles(const AssetCatalog& assets,const CharacterVisual& visual){
 const auto b=std::make_shared<const std::vector<std::uint8_t>>(assets.read("original-cache/data/3d/characters/faeries/faeries_02_celeste.bdae"));
 dh2::fx::CharacterAuthoredResourceFactoryV32 factory({nullptr,fx_camera,fx_driver},{});auto create=factory.factory();
 std::shared_ptr<dh2::fx::CharacterParticleFxResourceV2> resource;std::string error;const auto* pose=visual.retained_scene_borrow();check(pose,"live faery pose");
 check(create.create(create.context,b,*pose,resource,error),error.c_str());auto* composite=dynamic_cast<dh2::fx::CharacterAuthoredCompositeFxResourceV4*>(resource.get());check(composite,"composite actor emitter");
 auto moved=*pose;for(auto& node:moved.graph)node.world[12]+=900;
 auto mismatch=moved;mismatch.graph[0].id+="-wrong";check(!composite->source_actor_pose_v1(mismatch,error),"foreign graph admitted");
 mismatch=moved;mismatch.graph[1].parent=-2;check(!composite->source_actor_pose_v1(mismatch,error),"foreign parent admitted");
 mismatch=moved;mismatch.graph.back().world[12]=NAN;check(!composite->source_actor_pose_v1(mismatch,error),"nonfinite pose admitted");
 dh2::math::Matrix4f outer{};outer.m[0]=outer.m[5]=outer.m[10]=outer.m[15]=1;outer.m[12]=320;
 std::set<unsigned> emitters;unsigned packets=0;bool gold=false,shifted=false,trail=false;
 for(int step=1;step<=400;++step){
  check(resource->sample_animation((step*16)%2000,error),error.c_str());
  check(composite->source_actor_pose_v1(moved,error),error.c_str());check(composite->source_actor_pose_v1(moved,error),error.c_str());
  check(composite->source_scene_frame_v4(step*16,16,outer,error),error.c_str());
  std::vector<dh2::fx::CharacterParticleDrawSourceV3> sources;check(resource->draw_sources_v3(sources,error),error.c_str());
  for(const auto& source:sources){++packets;emitters.insert(source.emitter_node);
   check(std::abs(source.emitter_world[12]-(moved.graph[source.emitter_node].world[12]+320))<.002f,"emitter rebuilt actor pose from model rest graph");
   const auto& colors=source.part.geometry->attributes[source.part.geometry->primitives[0].attributes[2]].values;
   gold|=colors[0]==218&&colors[1]==197&&colors[2]==101;
   float center=0;for(unsigned j=0;j<4;++j)center+=source.part.positions[j][0]/4;
   if(shifted&&source.emitter_world[12]-center>900)trail=true;
  }
  if(!sources.empty()&&!shifted){for(auto& node:moved.graph)node.world[12]+=1000;shifted=true;}
 }
 check(emitters.size()==3&&packets>=10&&gold,"actual sparse faery emitters did not produce gold RGBA stars");
 check(trail,"already-born world-space particles followed moved actor");
}
int main(int argc,char** argv){try{
 check(argc==2,"asset root argument");AssetCatalog assets(argv[1]);CharacterVisual visual;CharacterVisualConfig c;
 c.model_path="original-cache/data/3d/characters/faeries/faeries_02_celeste.bdae";
 c.template_clip_path="data/3d/characters/faeries/animations/faeries_template_anim.bdae";
 c.clips={{"idle","data/3d/characters/faeries/animations/faeries_celeste_idle.bdae"},{"walk","data/3d/characters/faeries/animations/faeries_celeste_walk.bdae"}};
 c.include_static_instances=true;c.allow_missing_animation_targets=true;c.motion_node_id="auto";c.consume_root_motion=true;
 std::string error;check(visual.load(assets,c,error),error.c_str());check(visual.select("idle",true,error),error.c_str());check(visual.update(.35,error),error.c_str());
 check(visual.meshes().size()==3,"actual faery body/additive/glow parts");const auto plain=visual.meshes();const auto clock=visual.animation_elapsed_seconds();
 CameraPose camera{{700,-900,1400},{0,0,270},{0,0,1},45};Mat4 world{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};world[12]=320;world[13]=450;
 check(visual.prepare_camera_billboards(world,sourceCameraViewMatrix(camera),{camera.position.x,camera.position.y,camera.position.z},error),error.c_str());
 const auto faced=visual.meshes();check(same(plain[0],faced[0])&&same(plain[1],faced[1]),"billboard preparation changed skinned body");
 check(!same(plain[2],faced[2]),"authored glow ignored camera");check(visual.animation_elapsed_seconds()==clock,"billboard advanced clip time");
 check(visual.prepare_camera_billboards(world,sourceCameraViewMatrix(camera),{camera.position.x,camera.position.y,camera.position.z},error),error.c_str());
 check(same(faced[2],visual.meshes()[2]),"repeated camera preparation accumulates transform");
 camera.position={-900,700,1100};check(visual.prepare_camera_billboards(world,sourceCameraViewMatrix(camera),{camera.position.x,camera.position.y,camera.position.z},error),error.c_str());
 check(!same(faced[2],visual.meshes()[2]),"camera move did not reface glow");
 check(visual.select("walk",true,error)&&visual.update(.3,error),error.c_str());check(visual.prepare_camera_billboards(world,sourceCameraViewMatrix(camera),{camera.position.x,camera.position.y,camera.position.z},error),error.c_str());
 check(visual.update(.1,error),error.c_str());check(visual.prepare_camera_billboards(world,sourceCameraViewMatrix(camera),{camera.position.x,camera.position.y,camera.position.z},error),error.c_str());
 auto invalid=world;invalid[0]=NAN;check(!visual.prepare_camera_billboards(invalid,sourceCameraViewMatrix(camera),{camera.position.x,camera.position.y,camera.position.z},error),"invalid placement admitted");
 CharacterVisual ordinary;CharacterVisualConfig p;p.model_path="models/prince_modular.bdae";p.template_clip_path="animations/prince_template_anim.bdae";p.clips={{"idle","animations/prince_idle_shield.bdae"}};p.skin_id_contains="_default_warrior-mesh-skin";p.allow_missing_animation_targets=true;
 check(ordinary.load(assets,p,error)&&ordinary.update(.2,error),error.c_str());const auto normal=ordinary.meshes();
 check(ordinary.prepare_camera_billboards(world,sourceCameraViewMatrix(camera),{camera.position.x,camera.position.y,camera.position.z},error),error.c_str());
 check(normal.size()==ordinary.meshes().size(),"ordinary actor parts lost");for(unsigned i=0;i<normal.size();++i)check(same(normal[i],ordinary.meshes()[i]),"ordinary actor pose changed");
 actor_particles(assets,visual);
 std::cout<<"actor embedded visual PASS: real faery billboard/body/camera/clock/idempotence/motion, ordinary actor pose\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
