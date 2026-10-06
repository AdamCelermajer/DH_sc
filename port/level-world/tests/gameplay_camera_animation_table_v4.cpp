#include "../../game-data/animation_tables.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <stdexcept>
using namespace dh2;
int main(int argc,char** argv){try{if(argc!=2)return 2;std::vector<std::vector<std::uint8_t>> owned;owned.reserve(5);auto bytes=[&](const char* name){std::ifstream f(std::string(argv[1])+"/"+name,std::ios::binary);if(!f)throw std::runtime_error(std::string("Required actual table ")+name);owned.push_back({std::istreambuf_iterator<char>(f),{}});auto& b=owned.back();return data::Bytes{b.data(),b.size()};};std::string e;data::Dictionary dict;if(!data::load_dictionary(bytes("animations_dictionary_pyarraynames.bin"),bytes("animations_dictionary_pyarray.bin"),dict,e))throw std::runtime_error(e);data::AnimationTables table;if(!data::load_animation_tables(bytes("animations_pyarray.bin"),bytes("animations_pyarraynames.bin"),bytes("animations_pystructnames.bin"),dict,table,e))throw std::runtime_error(e);
 for(unsigned i=0;i<table.cameras.size();++i){auto& c=table.cameras[i];std::cout<<"CAM "<<i<<" "<<table.camera_names[i]<<" tpl "<<c.template_id<<" idle "<<c.idle<<" shake "<<c.shake<<" crit "<<c.crit<<" extras "<<c.cam_anims.size()<<'\n';for(auto id:{c.template_id,c.idle,c.shake,c.crit}){if(id<0)continue;std::cout<<"RESOURCE "<<id<<" "<<dict.values.at(id)<<'\n';}for(auto id:c.cam_anims)std::cout<<"EXTRA "<<id<<" "<<dict.values.at(id)<<'\n';}
 if(table.cameras.empty())throw std::runtime_error("Actual source camera table absent");std::cout<<"Actual whole camera table PASS rows "<<table.cameras.size()<<'\n';
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
