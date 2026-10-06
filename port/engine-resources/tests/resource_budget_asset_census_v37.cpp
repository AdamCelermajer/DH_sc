// Descriptor census only. No pixel decompression, skinning, GL or emulator.
#include "../resource_budget_v37.hpp"
#include "../../asset-payloads/payloads.hpp"
#include "../../engine-textures/textures.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>
using namespace dh2;
static std::string quote(const std::string& s){std::string o="\"";for(char c:s){if(c=='\\'||c=='\"')o+='\\';o+=c;}return o+'\"';}
static std::uint32_t word(const std::uint8_t* p){return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
int main(int argc,char** argv){try{
 for(int arg=1;arg<argc;++arg){
  std::ifstream f(argv[arg],std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("Missing bounded asset input");
  const auto length=f.tellg();if(length<0||std::uint64_t(length)>64*resources::mib_v37)throw std::runtime_error("Asset census input exceeds 64MiB read cap");
  std::vector<std::uint8_t> bytes(std::size_t(length),0);f.seekg(0);if(!f.read(reinterpret_cast<char*>(bytes.data()),std::streamsize(bytes.size())))throw std::runtime_error("Short census input");
  std::string e;std::uint64_t vbo=0,ebo=0,particle_vbo=0,particle_ebo=0;unsigned primitives=0,emitters=0;
  resources::BresView image{};
  std::cout<<"{\"input\":"<<quote(argv[arg])<<",\"encoded_bytes\":"<<bytes.size();
  if(dh2_bres_open(&image,bytes.data(),bytes.size())==resources::BresError::ok){
   const auto geometries=dh2_bres_library_count(&image,resources::Library::geometry);
   for(unsigned i=0;i<geometries;++i){assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&image,i)!=assets::Error::ok)throw std::runtime_error("Actual census mesh rejected");
    for(unsigned j=0;j<mesh.primitives;++j){assets::Primitive p{};if(dh2_mesh_primitive(&mesh,j,&p)!=assets::Error::ok)throw std::runtime_error("Actual census primitive rejected");
     std::uint64_t vertex_bytes,index_bytes;if(!resources::checked_resource_bytes_v37(mesh.vertices,36,vertex_bytes,e)||!resources::checked_resource_bytes_v37(p.index_count,2,index_bytes,e))throw std::runtime_error(e);
     vbo+=vertex_bytes;ebo+=index_bytes;++primitives;
    }
   }
   emitters=dh2_bres_library_count(&image,resources::Library::emitter);
   for(unsigned i=0;i<emitters;++i){auto p=dh2_bres_library_item(&image,resources::Library::emitter,i);const auto maximum=word(p+0x18);particle_vbo+=std::uint64_t(maximum)*4*36;particle_ebo+=std::uint64_t(maximum)*6*2;}
   std::cout<<",\"kind\":\"BRES\",\"geometries\":"<<geometries<<",\"primitives\":"<<primitives<<",\"static_vbo_bytes\":"<<vbo<<",\"static_ebo_bytes\":"<<ebo
            <<",\"emitters\":"<<emitters<<",\"max_particle_vbo_bytes\":"<<particle_vbo<<",\"max_particle_ebo_bytes\":"<<particle_ebo<<",\"images\":[";
   for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::image);++i){const auto* p=dh2_bres_library_item(&image,resources::Library::image,i);auto offset=word(p+8);
    if(offset>=bytes.size())throw std::runtime_error("Actual image URI outside file");
    const auto* start=reinterpret_cast<const char*>(bytes.data()+offset);const auto* end=static_cast<const char*>(std::memchr(start,0,bytes.size()-offset));
    if(!end||end-start>4096)throw std::runtime_error("Actual image URI outside bounded string");
    if(i)std::cout<<',';
    std::cout<<quote(std::string(start,end));
   }
   std::cout<<']';
  }else{
   textures::View view{};const auto status=dh2_texture_open(bytes.data(),bytes.size(),&view);
   if(status!=textures::Error::ok)throw std::runtime_error(std::string("Actual texture descriptor unsupported: ")+dh2_texture_error(status));
   std::uint64_t base,mips;if(!resources::rgba_texture_bytes_v37(view.width,view.height,false,base,e)||!resources::rgba_texture_bytes_v37(view.width,view.height,true,mips,e))throw std::runtime_error(e);
   std::cout<<",\"kind\":\"texture\",\"width\":"<<view.width<<",\"height\":"<<view.height<<",\"decoded_rgba_bytes\":"<<base<<",\"full_mip_rgba_bytes\":"<<mips;
  }
  std::cout<<"}\n";
 }
 return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
