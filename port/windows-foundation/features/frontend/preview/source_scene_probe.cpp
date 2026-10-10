#include "../../../asset_catalog.hpp"
#include "../../../content_paths.hpp"
#include "../../../../scene-materials/scene.hpp"
#include <iostream>
#include <cstring>
int main(int argc,char**argv){if(argc!=2)return 2;try{dh::foundation::AssetCatalog a(argv[1]);auto b=dh::foundation::read_content(a,"models/class_selection.bdae");dh2::resources::BresView v{};std::string e;if(dh2_bres_open(&v,b.data(),b.size())!=dh2::resources::BresError::ok)return 1;dh2::scene::Scene s;if(!dh2::scene::load(v,s,e)){std::cerr<<e;return 1;}for(const auto& n:s.graph){std::cout<<"NODE "<<n.name<<" | "<<n.id<<" | pos "<<n.translation[0]<<' '<<n.translation[1]<<' '<<n.translation[2]<<" scale "<<n.scale[0]<<' '<<n.scale[1]<<' '<<n.scale[2]<<'\n';}auto word=[](const unsigned char*p){unsigned v;std::memcpy(&v,p,4);return v;};for(unsigned i=0;i<dh2_bres_library_count(&v,dh2::resources::Library::animation_clip);++i){auto p=dh2_bres_library_item(&v,dh2::resources::Library::animation_clip,i);std::cout<<"CLIP "<<reinterpret_cast<const char*>(b.data()+word(p))<<' '<<word(p+4)<<' '<<word(p+8)<<'\n';}return 0;}catch(const std::exception&e){std::cerr<<e.what();return 1;}}
