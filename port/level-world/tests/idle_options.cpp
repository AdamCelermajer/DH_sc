#include "objects.hpp"
#include "animation_tables.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;const std::string root=argv[1];std::string error;auto a=read(root+"/data/animations_pyarray.bin"),b=read(root+"/data/animations_pyarraynames.bin"),c=read(root+"/data/animations_pystructnames.bin"),d=read(root+"/data/animations_dictionary_pyarraynames.bin"),e=read(root+"/data/animations_dictionary_pyarray.bin");
 dh2::data::Dictionary clips;dh2::data::AnimationTables tables;
 if(!dh2::data::load_dictionary(bytes(d),bytes(e),clips,error)||!dh2::data::load_animation_tables(bytes(a),bytes(b),bytes(c),clips,tables,error))throw std::runtime_error(error);
 std::cout<<"{\"clips\":[";unsigned index=0,total=0;
 for(auto pair:{std::pair<int,const char*>{62,"skeleton.bdae"},{64,"slime_green_v2.bdae"},{24,"ghost.bdae"}}){
  const auto* state=dh2::data::animation_state(tables,pair.first,"Idle");if(!state)throw std::runtime_error("Missing idle");std::set<int> seen;
  for(const auto& step:state->steps){if(!seen.insert(step.anim).second)continue;const auto* path=dh2::data::animation_clip(step,clips);if(!path)throw std::runtime_error("Expected direct clip");auto slash=path->find_last_of("/\\");auto name=path->substr(slash==std::string::npos?0:slash+1);
   auto model=read(root+"/actors/"+pair.second),clip=read(root+"/actors/"+name);dh2::objects::Resource resource;
   if(!dh2::objects::load_resource(model.data(),model.size(),clip.data(),clip.size(),resource,error))throw std::runtime_error(error);
   unsigned poses=0;for(int ms=resource.animation.start;ms<=resource.animation.end;++ms){if(!dh2::objects::sample(resource,ms,error))throw std::runtime_error(error);for(const auto& primitive:resource.primitives)for(const auto& vertex:primitive.vertices)for(float value:vertex.p)if(!std::isfinite(value))throw std::runtime_error("Nonfinite idle vertex");++poses;}
   if(index++)std::cout<<',';std::cout<<"{\"model\":\""<<pair.second<<"\",\"clip\":\""<<name<<"\",\"clip_id\":"<<step.anim<<",\"tracks\":"<<resource.animation.track_count()<<",\"unbound\":"<<resource.animation.unbound<<",\"speed\":"<<step.speed<<",\"poses\":"<<poses<<'}';total+=poses;
  }
 }
 if(index!=5)throw std::runtime_error("Idle option count differs");std::cout<<"],\"every_millisecond_poses\":"<<total<<",\"original_idle_choices\":5,\"finite_skinned_vertices\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
