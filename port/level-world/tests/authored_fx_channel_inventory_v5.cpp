#include "../../asset-payloads/payloads.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
using namespace dh2;
int main(int argc,char** argv){if(argc!=2)return 1;const std::string dir=argv[1];std::ifstream list(dir+"/resources.txt");std::string name;
 while(std::getline(list,name)){if(!name.empty()&&name.back()=='\r')name.pop_back();std::ifstream f(dir+"/"+name,std::ios::binary);if(!f)return 1;std::vector<std::uint8_t> raw{std::istreambuf_iterator<char>(f),{}};resources::BresView image{};if(dh2_bres_open(&image,raw.data(),raw.size())!=resources::BresError::ok)return 1;
  std::cout<<"asset "<<name<<" | "<<std::flush;for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::animation);++i){assets::Animation a{};if(dh2_animation_open(&a,&image,i,0)!=assets::Error::ok)return 1;const auto type=dh2_animation_type(&a,0);if(type==1||type==5||type==10||type==87||type==89||type==91)continue;assets::Vector v{};bool vec=type==86&&dh2_animation_samplers(&a)==1&&dh2_animation_vector(&a,0,true,&v);const auto* target=dh2_animation_target(&a);std::cout<<"type "<<type<<" target "<<(target?target:"<none>")<<" channels "<<dh2_animation_channels(&a)<<" samplers "<<dh2_animation_samplers(&a)<<" animator "<<dh2_animation_animator(&a)<<" offsets "<<(dh2_animation_offsets(&a)!=nullptr);if(vec)std::cout<<" vector "<<v.type<<"x"<<v.components<<" count "<<v.count;std::cout<<"; "<<std::flush;}
  std::cout<<" | mesh ";for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::geometry);++i){assets::Mesh m{};auto err=dh2_mesh_open(&m,&image,i);std::cout<<i<<':'<<unsigned(err)<<" vertices "<<m.vertices<<" primitives "<<m.primitives<<"; ";}std::cout<<'\n';
 }return 0;
}
