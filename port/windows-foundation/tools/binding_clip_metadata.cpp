#include "../animation_markers.hpp"
#include "../../engine-animation/animation.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
std::vector<std::uint8_t> read(const char* name){std::ifstream f(name,std::ios::binary);if(!f)throw std::runtime_error("Missing metadata input");return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char**argv){try{
 if(argc!=3)return 2;auto model=read(argv[1]),clip=read(argv[2]);dh2::resources::BresView view{};
 if(dh2_bres_open(&view,model.data(),model.size())!=dh2::resources::BresError::ok)throw std::runtime_error("Model BRES rejected");
 dh2::scene::Scene scene;std::string error;if(!dh2::scene::load(view,scene,error))throw std::runtime_error(error);
 dh2::animation::Player player;if(!player.load(clip.data(),clip.size(),scene,error,dh2::animation::MissingTargets::ignore))throw std::runtime_error(error);
 dh::foundation::AnimationMarkers markers;
 if(player.events.view().count&&!markers.load(clip.data(),clip.size(),player.start,player.end,error))throw std::runtime_error(error);
 std::cout<<"{\"startMs\":"<<player.start<<",\"endMs\":"<<player.end<<",\"markers\":[";bool first=true;
 for(const auto&m:markers.markers()){if(!first)std::cout<<',';first=false;std::cout<<"{\"name\":"<<std::quoted(m.name)<<",\"timeMs\":"<<m.time_ms<<",\"authoredTimeMs\":"<<m.authored_time_ms<<'}';}
 std::cout<<"]}\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
