#include "../../animation.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
using namespace dh2;
static std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error(path);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char**argv){try{
 if(argc!=4)return 2;auto raw=read(argv[1]);resources::BresView image{};scene::Scene scene;std::string error;
 if(dh2_bres_open(&image,raw.data(),raw.size())!=resources::BresError::ok||!scene::load(image,scene,error))throw std::runtime_error(error);
 std::ifstream list(argv[2]);int id;unsigned count=0,failures=0,skipped=0,unbound=0;std::cout<<"{\"resources\":[";
 while(list>>id){auto bytes=read(std::string(argv[3])+"/"+std::to_string(id)+".bdae");animation::Player player;
  const bool ok=player.load(bytes.data(),bytes.size(),scene,error,animation::MissingTargets::ignore);
  if(count++)std::cout<<',';std::cout<<"{\"clip_id\":"<<id<<",\"loaded\":"<<(ok?"true":"false")<<",\"tracks\":"<<player.track_count()<<",\"segments\":"<<player.segment_count()<<",\"skipped\":"<<player.skipped<<",\"unbound\":"<<player.unbound<<",\"error\":\""<<error<<"\"}";
  failures+=!ok;skipped+=player.skipped;unbound+=player.unbound;
 }
 std::cout<<"],\"count\":"<<count<<",\"failures\":"<<failures<<",\"skipped\":"<<skipped<<",\"unbound\":"<<unbound<<"}\n";return failures||skipped?1:0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
