#include "decor_scene.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
int main(int argc,char** argv){try{
 if(argc!=2)throw std::runtime_error("requires exact extracted chest cache directory");
 for(const char* name:{"go_chest_swamp.bdae","go_chest_swamp_big.bdae","go_chest_swamp_rotten.bdae"}){
  std::ifstream file(std::string(argv[1])+"/"+name,std::ios::binary);
  if(!file)throw std::runtime_error(std::string("actual chest resource absent: ")+name);
  std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
  dh2::resources::BresView view{};
  if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)throw std::runtime_error("actual chest BRES rejected");
  dh2::scene::Scene scene;std::string error;
  if(!dh2::scene::load(view,scene,error))throw std::runtime_error(error);
  dh2::physical::DecorSceneMarker marker;
  if(!dh2::physical::decor_scene_marker(view,scene,marker,error))throw std::runtime_error(error);
  std::cout<<name<<" nodes="<<scene.graph.size()<<" meshes="<<scene.instances.size()<<" colbox="<<marker.found;
  if(marker.found){std::cout<<" bounds=";for(float x:marker.bounds)std::cout<<x<<',';std::cout<<" parent_scale=";for(float x:marker.parent_scale)std::cout<<x<<',';}
  std::cout<<'\n';
 }
 std::cout<<"Actual three chest resources and complete-scene marker resolution PASS\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
