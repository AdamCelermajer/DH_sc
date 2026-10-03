#include "skinning.hpp"
#include "animation.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
std::vector<std::uint8_t> read(const char* name){std::ifstream f(name,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){
 if(argc!=4)return 2;auto model=read(argv[1]);dh2::resources::BresView view{};
 if(dh2_bres_open(&view,model.data(),model.size())!=dh2::resources::BresError::ok)return 3;
 dh2::scene::Scene rest;std::string error;if(!dh2::scene::load(view,rest,error)){std::cerr<<error;return 4;}
 std::vector<dh2::skinning::Skin> skins;
 std::vector<std::vector<std::array<float,3>>> positions;
 for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::controller);++i){dh2::skinning::Skin skin;
  if(!dh2::skinning::load(view,i,rest,skin,error)){std::cerr<<error;return 5;}if(skin.id.find("_default_warrior-mesh-skin")==std::string::npos)continue;
  dh2::assets::Mesh mesh{};dh2_mesh_open(&mesh,&view,skin.geometry);dh2::assets::Primitive primitive{};dh2_mesh_primitive(&mesh,0,&primitive);dh2::assets::Attribute attribute{};dh2_mesh_attribute(&mesh,primitive.attributes[0],&attribute);
  positions.emplace_back(mesh.vertices);for(unsigned j=0;j<mesh.vertices;++j){float value[4]{};dh2_attribute_read(&attribute,j,value);std::copy(value,value+3,positions.back()[j].begin());}skins.push_back(std::move(skin));
 }
 if(skins.size()!=4)return 6;std::cout<<"{\"skins\":4,\"clips\":[";
 for(unsigned clip_index=2;clip_index<4;++clip_index){auto clip=read(argv[clip_index]);auto scene=rest;dh2::animation::Player player;
  if(!player.load(clip.data(),clip.size(),scene,error)||player.skipped){std::cerr<<error;return 7;}
  unsigned changed=0;float lowest=INFINITY,highest=-INFINITY;std::vector<std::vector<std::array<float,3>>> first;
  for(int ms=player.start;ms<=player.end;++ms){if(!player.sample(scene,ms,error)){std::cerr<<error;return 8;}bool different=false;float foot=INFINITY;
   for(unsigned i=0;i<skins.size();++i){std::vector<dh2::skinning::Matrix> palette;std::vector<std::array<float,3>> output;
    if(!dh2::skinning::palette(skins[i],scene,palette,error)||!dh2::skinning::positions(skins[i],palette,positions[i],output,error)){std::cerr<<error;return 9;}
    for(const auto& vertex:output){for(float value:vertex)if(!std::isfinite(value))return 10;foot=std::min(foot,vertex[2]);}
    if(ms==player.start)first.push_back(output);else different=different||output!=first[i];
   }
   changed+=different;lowest=std::min(lowest,foot);highest=std::max(highest,foot);
  }
  if(changed<100)return 11;if(clip_index!=2)std::cout<<',';
  std::cout<<"{\"tracks\":"<<player.track_count()<<",\"skipped\":"<<player.skipped<<",\"segments\":"<<player.segment_count()<<",\"start_ms\":"<<player.start<<",\"end_ms\":"<<player.end<<",\"sampled_poses\":"<<player.end-player.start+1<<",\"changed_poses\":"<<changed<<",\"minimum_foot_z\":"<<lowest<<",\"maximum_foot_z\":"<<highest<<'}';
 }
 std::cout<<"]}\n";return 0;
}
