#include "animation.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
std::vector<std::uint8_t> read(const char* name){std::ifstream f(name,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){if(argc!=3)return 2;auto model=read(argv[1]),clip=read(argv[2]);dh2::resources::BresView view{};if(dh2_bres_open(&view,model.data(),model.size())!=dh2::resources::BresError::ok)return 3;
 dh2::scene::Scene scene;std::string error;if(!dh2::scene::load(view,scene,error)){std::cerr<<error;return 4;}
 dh2::animation::Player player;if(!player.load(clip.data(),clip.size(),scene,error,dh2::animation::MissingTargets::ignore)){std::cerr<<error;return 5;}
 for(int ms=player.start;ms<=player.end;++ms)if(!player.sample(scene,ms,error)){std::cerr<<error;return 6;}
 std::cout<<"tracks="<<player.track_count()<<" skipped="<<player.skipped<<" unbound="<<player.unbound<<" segments="<<player.segment_count()<<" start="<<player.start<<" end="<<player.end<<'\n';return 0;
}
