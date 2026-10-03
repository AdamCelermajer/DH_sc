#include "../visual_motion.hpp"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
namespace fs=std::filesystem;
std::vector<std::uint8_t> read(const fs::path& p){std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
template<class T>void write(std::ofstream& f,const T& v){f.write(reinterpret_cast<const char*>(&v),sizeof(v));}
struct Row {std::uint32_t clip,milliseconds,timestamp,previous_timestamp;float previous[3],point[3];std::uint32_t reset;};
static_assert(sizeof(Row)==44);
int main(int argc,char** argv){
 if(argc!=4)return 2;std::string error;fs::path assets=argv[1];std::vector<fs::path> clips;
 for(const auto& directory:{assets/"animations",assets/"actors"})for(const auto& entry:fs::directory_iterator(directory))if(entry.path().extension()==".bdae")clips.push_back(entry.path());std::sort(clips.begin(),clips.end());
 std::vector<Row> rows;std::ofstream report(argv[3]);report<<"{\n  \"clips\": [\n";unsigned count=0,totalnodes=0;
 for(const auto& file:clips){const auto name=file.filename().string();fs::path model;
  if(name=="skeleton.bdae"||name=="ghost.bdae"||name=="slime_green_v2.bdae")continue;
  if(name.rfind("prince_",0)==0)model=assets/"models/prince_modular.bdae";
  else if(name.rfind("skeleton_",0)==0)model=assets/"actors/skeleton.bdae";
  else if(name.rfind("slime_",0)==0)model=assets/"actors/slime_green_v2.bdae";
  else if(name.rfind("ghost_",0)==0)model=assets/"actors/ghost.bdae";else continue;
  auto modelbytes=read(model),clipbytes=read(file);dh2::resources::BresView view{};dh2::scene::Scene scene;
  if(dh2_bres_open(&view,modelbytes.data(),modelbytes.size())!=dh2::resources::BresError::ok||!dh2::scene::load(view,scene,error)){std::cerr<<model<<": "<<error;return 3;}
  dh2::animation::Player player;if(!player.load(clipbytes.data(),clipbytes.size(),scene,error,dh2::animation::MissingTargets::ignore)){std::cerr<<name<<": "<<error;return 4;}
  dh2::visual::SceneBinding binding;if(!binding.bind(scene,error)){std::cerr<<name<<": "<<error;return 5;}auto animated=binding.animated_node();const auto rootname=scene.graph[animated].name;
  float previous[3]{};std::uint32_t previous_timestamp=0;unsigned cliprows=0;float first[3]{},last[3]{};
  std::vector<std::int32_t> times;for(auto ms=player.start;ms<player.end;ms+=16)times.push_back(ms);times.push_back(player.end);times.push_back(player.end);times.push_back(player.start);
  for(unsigned i=0;i<times.size();++i){auto ms=times[i];bool reset=i==0||i+1==times.size();std::uint32_t timestamp=i+1==times.size()?0:std::uint32_t(ms-player.start+1);
   if(!player.sample(scene,ms,error)){std::cerr<<error;return 6;}Row row{count,std::uint32_t(ms),timestamp,previous_timestamp,{previous[0],previous[1],previous[2]},{scene.graph[animated].translation[0],scene.graph[animated].translation[1],scene.graph[animated].translation[2]},std::uint32_t(reset)};rows.push_back(row);
   if(!binding.sample(scene,player,ms,timestamp,reset,error)){std::cerr<<error;return 7;}
   for(const auto& node:scene.graph)for(float f:node.world)if(!std::isfinite(f))return 8;
   for(unsigned k=0;k<3;++k){previous[k]=row.point[k];last[k]=binding.root.position[k];if(i==0)first[k]=row.point[k];}previous_timestamp=timestamp;++cliprows;
  }
  totalnodes+=scene.graph.size();if(count)report<<",\n";report<<"    {\"name\":\""<<name<<"\",\"model\":\""<<model.filename().string()<<"\",\"root_name\":\""<<rootname<<"\",\"root_node\":"<<animated<<",\"nodes\":"<<scene.graph.size()<<",\"tracks\":"<<player.track_count()<<",\"unbound\":"<<player.unbound<<",\"start\":"<<player.start<<",\"end\":"<<player.end<<",\"samples\":"<<cliprows<<",\"accumulated_xyz\":["<<last[0]<<','<<last[1]<<','<<last[2]<<"]}";++count;
 }
 report<<"\n  ],\n  \"clip_count\": "<<count<<",\n  \"sample_count\": "<<rows.size()<<",\n  \"graph_node_count\": "<<totalnodes<<",\n  \"errors\":0\n}\n";
 std::ofstream output(argv[2],std::ios::binary);output.write("VRSA",4);std::uint32_t n=rows.size();write(output,n);for(const auto& row:rows)write(output,row);
 std::cout<<"Visual motion assets: clips="<<count<<" samples="<<rows.size()<<" nodes="<<totalnodes<<" errors=0\n";return count<4?9:0;
}
