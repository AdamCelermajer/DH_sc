#include "visual_motion.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
unsigned checks{};void check(bool v){++checks;if(!v)std::exit(1);}
int main(){
 // Isolated transform-owner regression. The live renderer additionally uses
 // real Gear/BRES skinning; this test does not substitute a synthetic actor.
 dh2::scene::Scene scene;dh2::scene::Node n;n.id="root";n.name="Bip01";scene.graph.push_back(n);
 scene.instances.push_back({"root",0,0,{},{},-1});dh2::visual::SceneBinding binding;std::string error;
 check(binding.bind(scene,error));binding.root.position[0]=1250;binding.root.position[1]=-730;binding.root.position[2]=20;
 binding.root.scale[0]=binding.root.scale[1]=binding.root.scale[2]=2;const float euler[]{0,0,.8f};check(binding.set_rotation(euler));
 const auto root=binding.root;check(scene.instances[0].world[12]!=root.position[0]);
 check(binding.update_world(scene,error));check(scene.instances[0].world[12]==root.position[0]);
 auto first=scene.instances[0].world;for(unsigned i=0;i<20;++i){check(binding.update_world(scene,error));check(scene.instances[0].world==first);check(!std::memcmp(&root,&binding.root,sizeof root));}
 // Equipment mutations consume the same current scene: changed authored node
 // TRS is republished without replaying a clip or changing root ownership.
 scene.graph[0].translation[2]=17;check(binding.update_world(scene,error));check(scene.instances[0].world[14]==54);
 first=scene.instances[0].world;scene.graph[0].id="wrong";check(!binding.update_world(scene,error));check(scene.instances[0].world==first);
 std::printf("Inventory pane same-root pose publication PASS %u checks; no timeline/events/AI advancement\n",checks);
}
