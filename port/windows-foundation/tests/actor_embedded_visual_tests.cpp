#include "../original_character.hpp"
#include "../asset_catalog.hpp"
#include "../camera.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
bool same(const Mesh& a,const Mesh& b,float tolerance=0.002f){if(a.vertices.size()!=b.vertices.size())return false;for(unsigned i=0;i<a.vertices.size();++i){const auto& x=a.vertices[i].position;const auto& y=b.vertices[i].position;if(std::abs(x.x-y.x)>tolerance||std::abs(x.y-y.y)>tolerance||std::abs(x.z-y.z)>tolerance)return false;}return true;}
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
 std::cout<<"actor embedded visual PASS: real faery billboard/body/camera/clock/idempotence/motion, ordinary actor pose\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
