#include "world.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <cstdlib>
std::vector<std::uint8_t> read(const char* file){std::ifstream f(file,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
void point(const dh2::world::Point& p){std::cout<<'['<<p[0]<<','<<p[1]<<','<<p[2]<<']';}
int main(int argc,char** argv){
 if(argc!=3&&argc!=6)return 2;auto bytes=read(argv[1]),descriptor=read(argv[2]);dh2::resources::BresView view{};
 if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)return 3;
 dh2::world::Level level;std::string error;if(!dh2::world::load(view,descriptor.data(),descriptor.size(),level,error)){std::cerr<<error;return 4;}
 std::cout<<std::setprecision(9);
 if(argc==6){dh2::world::Point p{std::strtof(argv[3],nullptr),std::strtof(argv[4],nullptr),std::strtof(argv[5],nullptr)};float h=0;
  const bool supported=dh2::world::supported(level,p,36,h);std::cout<<"{\"supported\":"<<(supported?"true":"false")<<",\"height\":"<<h<<"}\n";return supported?0:5;}
 std::cout<<"{\"spawn\":";point(level.spawn);std::cout<<",\"floor\":[";bool first=true;
 for(const auto& t:level.floor){if(!first)std::cout<<',';first=false;std::cout<<"{\"room\":"<<t.room<<",\"corners\":[";point(t.a);std::cout<<',';point(t.b);std::cout<<',';point(t.c);std::cout<<"]}";}
 std::cout<<"]}\n";return 0;
}
