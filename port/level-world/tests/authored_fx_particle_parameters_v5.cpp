#include "../../asset-payloads/payloads.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
int main(int argc,char** argv){if(argc!=2)return 1;std::ifstream f(std::string(argv[1])+"/asset_27.bdae",std::ios::binary);std::vector<std::uint8_t> bytes{std::istreambuf_iterator<char>(f),{}};dh2::resources::BresView image{};if(dh2_bres_open(&image,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)return 1;
 for(unsigned i=0;i<dh2_bres_library_count(&image,dh2::resources::Library::animation);++i){dh2::assets::Animation a{};if(dh2_animation_open(&a,&image,i,0)!=dh2::assets::Error::ok)return 1;unsigned type=dh2_animation_type(&a,0);if(type!=37&&type!=38)continue;const auto* ch=dh2_animation_channel(&a,0);std::uint32_t prop;std::memcpy(&prop,ch+12,4);dh2::assets::Vector v{};bool vector=dh2_animation_vector(&a,0,true,&v);std::cout<<"type "<<type<<" target "<<dh2_animation_target(&a)<<" propertyWord "<<prop<<" vectorAvailable "<<vector<<" vector "<<v.type<<'x'<<v.components<<" count "<<v.count<<" channels "<<dh2_animation_channels(&a)<<" samplers "<<dh2_animation_samplers(&a)<<"\n";}
 return 0;}
