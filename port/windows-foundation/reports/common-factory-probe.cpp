#include "../../scene-materials/scene.hpp"
#include "../../scene-materials/effect_render_pass_v4.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <stdexcept>
int main(int argc,char**argv){try{for(int a=1;a<argc;++a){
 std::ifstream f(argv[a],std::ios::binary|std::ios::ate);auto n=f.tellg();if(n<=0)return 1;std::vector<unsigned char>b(static_cast<std::size_t>(n));f.seekg(0);f.read(reinterpret_cast<char*>(b.data()),n);
 auto w=[&](std::size_t p){if(p+4>b.size())throw std::runtime_error("field bounds");std::uint32_t v;std::memcpy(&v,b.data()+p,4);return v;};
 auto t=[&](std::uint32_t p){if(!p)return std::string();if(p>=b.size())throw std::runtime_error("text bounds");auto e=std::memchr(b.data()+p,0,b.size()-p);if(!e)throw std::runtime_error("text terminator");return std::string(reinterpret_cast<char*>(b.data()+p),static_cast<unsigned char*>(e)-(b.data()+p));};
 dh2::resources::BresView v{};if(dh2_bres_open(&v,b.data(),b.size())!=dh2::resources::BresError::ok)return 2;
 std::cout<<"FILE "<<argv[a]<<"\n";
 for(unsigned m=0;m<dh2_bres_library_count(&v,dh2::resources::Library::material);++m){auto q=dh2_bres_library_item(&v,dh2::resources::Library::material,m)-v.bytes;
  const auto uri=t(w(q+12));std::cout<<"MATERIAL "<<m<<" id="<<t(w(q))<<" file="<<t(w(q+8))<<" effect="<<uri<<"\n";
  if(t(w(q+8)).empty()){dh2::scene::EffectRenderPassV4 pass;std::string error;if(dh2::scene::effect_render_pass_v4(v,t(w(q)).c_str(),"default",pass,error))std::cout<<" DEFAULT_RENDER blend="<<pass.blend<<" src="<<pass.blend_src<<" dst="<<pass.blend_dst<<" depth="<<pass.depth<<" write="<<pass.depth_write<<" cull="<<pass.cull<<"\n";else std::cout<<" DEFAULT_RENDER_ERROR "<<error<<"\n";}
  for(unsigned j=0;j<w(q+16);++j){auto p=w(q+20)+24*j;std::cout<<" PARAM "<<t(w(p))<<" type="<<w(p+8);if(w(p+8)==20)std::cout<<" technique="<<t(w(w(p+20)+4));std::cout<<"\n";}
  for(unsigned e=0;e<dh2_bres_library_count(&v,dh2::resources::Library::effect);++e){auto p=dh2_bres_library_item(&v,dh2::resources::Library::effect,e)-v.bytes;if("#"+t(w(p))!=uri)continue;
   for(unsigned j=0;j<w(p+32);++j){auto row=w(p+36)+12*j;std::cout<<" TECHNIQUE index="<<j<<" name="<<t(w(row))<<" passes="<<w(row+4)<<"\n";
    if(w(row+4)!=1)continue;auto pass=w(row+8);std::cout<<" VS="<<t(w(pass+4))<<" PREAMBLE="<<t(w(pass+12))<<" FS="<<t(w(pass+16))<<" FPREAMBLE="<<t(w(pass+24))<<"\n";
   }
  }
 }
}return 0;}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 3;}}
