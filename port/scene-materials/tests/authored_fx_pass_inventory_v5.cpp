#include "../effect_render_pass_v4.hpp"
#include "../particle_scene_v1.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
int main(int argc,char** argv){if(argc!=2)return 1;const std::string dir=argv[1];std::ifstream list(dir+"/resources.txt");std::string name;unsigned transparent=0,opaque=0,required=0;
 while(std::getline(list,name)){if(!name.empty()&&name.back()=='\r')name.pop_back();std::ifstream f(dir+"/"+name,std::ios::binary);if(!f)return 1;std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(f),{}};dh2::resources::BresView image{};if(dh2_bres_open(&image,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)return 1;dh2::scene::Scene scene;std::string e;if(!dh2::scene::load_particle_scene_v1(image,scene,e)){++required;std::cout<<name<<" | scene REQUIRED | "<<e<<'\n';continue;}
  for(const auto& material:scene.materials){dh2::scene::EffectRenderPassV4 pass;if(!dh2::scene::effect_render_pass_v4(image,material.id.c_str(),"default",pass,e)){++required;std::cout<<name<<" | "<<material.id<<" | REQUIRED | "<<e<<'\n';continue;}std::uint32_t flags;std::memcpy(&flags,pass.pass.data()+4,4);(flags&0x10000)?++transparent:++opaque;std::cout<<name<<" | "<<material.id<<" | "<<((flags&0x10000)?"transparent":"opaque")<<" | "<<pass.vertex_file<<" | "<<pass.vertex_defines.size()<<" | "<<pass.fragment_defines.size()<<'\n';}
 }std::cout<<"{\"transparent_passes\":"<<transparent<<",\"opaque_passes\":"<<opaque<<",\"required\":"<<required<<"}"<<std::endl;return 0;}
