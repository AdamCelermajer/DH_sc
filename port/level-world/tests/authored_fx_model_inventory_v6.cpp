#include "../../engine-animation/particle_factory.hpp"
#include "../../asset-payloads/payloads.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace dh2;
int main(int argc,char** argv){if(argc!=2)return 1;for(unsigned asset:{3u,4u,5u,6u,7u,8u,9u}){std::string path=std::string(argv[1])+"/asset_0"+std::to_string(asset)+".bdae";std::ifstream f(path,std::ios::binary);auto raw=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());resources::BresView image{};if(dh2_bres_open(&image,raw->data(),raw->size())!=resources::BresError::ok)return 1;
 for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::emitter);++i){animation::ParticleEmitterInput input;std::string e;if(!animation::decode_particle_emitter(raw,i,input,e)){std::cout<<asset<<" decode "<<e<<'\n';continue;}std::cout<<asset<<" emitter "<<input.name<<" directionType "<<input.record[0x48/4]<<" spinAxisType "<<input.record[0x88/4]<<" directionOffset "<<input.record[0x4c/4]<<'\n';}
 for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::geometry);++i){const auto* g=dh2_bres_library_item(&image,resources::Library::geometry,i);std::uint32_t kind;std::memcpy(&kind,g+8,4);std::cout<<asset<<" geometry "<<i<<" kind "<<kind<<'\n';}
 }return 0;}
