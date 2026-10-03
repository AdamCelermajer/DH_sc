#include "objects.hpp"
#include "animation_tables.hpp"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;const std::string root=argv[1];std::string error;auto a=read(root+"/data/animations_pyarray.bin"),b=read(root+"/data/animations_pyarraynames.bin"),c=read(root+"/data/animations_pystructnames.bin"),d=read(root+"/data/animations_dictionary_pyarraynames.bin"),e=read(root+"/data/animations_dictionary_pyarray.bin");dh2::data::Dictionary clips;dh2::data::AnimationTables tables;
 if(!dh2::data::load_dictionary(bytes(d),bytes(e),clips,error)||!dh2::data::load_animation_tables(bytes(a),bytes(b),bytes(c),clips,tables,error))throw std::runtime_error(error);
 std::cout<<"{\"clips\":[";unsigned count=0,total=0,unbound=0,unsupported=0;
 for(auto actor:{std::pair<int,const char*>{62,"skeleton.bdae"},{64,"slime_green_v2.bdae"},{24,"ghost.bdae"}}){std::set<int> ids;std::function<void(int,unsigned)> visit=[&](int id,unsigned depth){if(depth>=3)throw std::runtime_error("Clip redirect depth");for(const auto& step:tables.sequences.at(id).steps){if(step.redir)visit(step.anim,depth+1);else ids.insert(step.anim);}};
  for(const char* state_name:{"Idle","Walk","Attack","Died"}){auto* state=dh2::data::animation_state(tables,actor.first,state_name);if(!state)throw std::runtime_error("Missing state");visit(state-tables.sequences.data(),0);}
  for(int id:ids){auto path=clips.values.at(id);auto slash=path.find_last_of("/\\");auto name=path.substr(slash==std::string::npos?0:slash+1);auto model=read(root+"/actors/"+actor.second),clip=read(root+"/actors/"+name);dh2::objects::Resource resource;
   if(!dh2::objects::load_resource(model.data(),model.size(),clip.data(),clip.size(),resource,error))throw std::runtime_error(error);if(resource.animation.end<=resource.animation.start)throw std::runtime_error("Empty actor clip");unsigned poses=0;
   for(int ms=resource.animation.start;ms<=resource.animation.end;++ms){if(!dh2::objects::sample(resource,ms,error))throw std::runtime_error(error);for(const auto& p:resource.primitives)for(const auto& vertex:p.vertices)for(float value:vertex.p)if(!std::isfinite(value))throw std::runtime_error("Nonfinite actor vertex");++poses;}
   if(count++)std::cout<<',';std::cout<<"{\"model\":\""<<actor.second<<"\",\"clip\":\""<<name<<"\",\"clip_id\":"<<id<<",\"start\":"<<resource.animation.start<<",\"end\":"<<resource.animation.end<<",\"tracks\":"<<resource.animation.track_count()<<",\"unbound\":"<<resource.animation.unbound<<",\"unsupported\":"<<resource.animation.skipped<<",\"poses\":"<<poses<<'}';total+=poses;unbound+=resource.animation.unbound;unsupported+=resource.animation.skipped;
  }
 }
 if(count!=26)throw std::runtime_error("Actor clip count differs");std::cout<<"],\"clip_count\":26,\"every_millisecond_poses\":"<<total<<",\"unbound_tracks\":"<<unbound<<",\"unsupported_tracks\":"<<unsupported<<",\"finite_skinned_vertices\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
