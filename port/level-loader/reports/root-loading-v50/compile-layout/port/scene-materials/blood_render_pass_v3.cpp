#include "blood_render_pass_v3.hpp"
#include <cstring>
#include <stdexcept>
namespace {std::uint32_t w(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}void put(std::uint8_t* p,std::uint32_t v){std::memcpy(p,&v,4);}}
extern "C" int dh2_render_pass_convert_v3(dh2::scene::RenderPassState32V3* out,const dh2::scene::RenderStateSource76V3* in){
 if(!out||!in)return -1;
 const auto& s=*in;const auto a=w(s.data()+8),b=w(s.data()+12),c=w(s.data()+16);dh2::scene::RenderPassState32V3 r{};
 put(r.data(),s[0]|(std::uint32_t(s[2])<<8)|(std::uint32_t(s[3])<<16)|(((a>>12)&7)<<24)|(((b>>12)&7)<<27)|(a&0xc0000000));
 std::uint32_t flags=((a>>18)&7)|(((a>>21)&7)<<3)|(((a>>24)&7)<<6)|(((a>>27)&7)<<9)|(((b>>15)&3)<<12)|(((b>>17)&3)<<14);
 for(unsigned bit=19;bit<=23;++bit)flags|=((b>>bit)&1)<<(bit-3);
 for(unsigned bit=25;bit<=30;++bit)flags|=((b>>bit)&1)<<(bit-4);
 flags|=(c&1)<<27;put(r.data()+4,flags);std::memcpy(r.data()+8,s.data()+20,4);std::memcpy(r.data()+12,s.data()+40,8);std::memcpy(r.data()+20,s.data()+48,8);std::memcpy(r.data()+28,s.data()+56,4);*out=r;return 0;
}
#ifndef DH2_RENDER_PASS_KERNEL_ONLY
namespace dh2::scene {
namespace {
struct Reader {const resources::BresView& v;
 const std::uint8_t* at(std::uint64_t p,std::uint64_t n)const{if(p>v.size||n>v.size-p)throw std::runtime_error("Blood render pass range");return v.bytes+p;}
 std::uint32_t word(std::uint64_t p)const{return w(at(p,4));}
 std::string text(std::uint32_t p)const{if(!p)throw std::runtime_error("Blood render pass missing string");auto b=at(p,1);auto e=static_cast<const std::uint8_t*>(std::memchr(b,0,v.size-p));if(!e)throw std::runtime_error("Blood render pass string range");return {reinterpret_cast<const char*>(b),reinterpret_cast<const char*>(e)};}
};
}
bool blood_render_pass_v3(const resources::BresView& image,const char* material,const char* technique,BloodRenderPassV3& out,std::string& error){try{
 if(!material||!technique)throw std::runtime_error("Required actual blood material/technique selection");
 Reader r{image};std::string effect_uri;unsigned matches=0;
 for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::material);++i){auto p=dh2_bres_library_item(&image,resources::Library::material,i)-image.bytes;if(r.text(r.word(p))!=material)continue;if(r.word(p+8))throw std::runtime_error("Required external blood material effect");effect_uri=r.text(r.word(p+12));++matches;}
 if(matches!=1||effect_uri.empty()||effect_uri[0]!='#')throw std::runtime_error("Blood render material lookup");
 matches=0;BloodRenderPassV3 result;
 for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::effect);++i){auto p=dh2_bres_library_item(&image,resources::Library::effect,i)-image.bytes;if(r.text(r.word(p))!=effect_uri.substr(1))continue;const auto n=r.word(p+32),base=r.word(p+36);r.at(base,std::uint64_t(n)*12);for(unsigned j=0;j<n;++j){auto t=base+12*j;if(r.text(r.word(t))!=technique)continue;++matches;if(r.word(t+4)!=1)throw std::runtime_error("Required multipass blood render continuation");auto pass=r.word(t+8);r.at(pass,116);result.vertex_file=r.text(r.word(pass+4));result.vertex_defines=r.text(r.word(pass+12));result.fragment_file=r.text(r.word(pass+16));result.fragment_defines=r.text(r.word(pass+24));std::memcpy(result.source.data(),r.at(pass+28,76),76);}}
 if(matches!=1)throw std::runtime_error("Blood actual GLES2 technique lookup");
 if(result.vertex_file!="ProfileCOMMON_emul_VS.glsl"||result.fragment_file!="ProfileCOMMON_emul_FS.glsl"||result.vertex_defines!="#define TEXTURED\n"||result.fragment_defines!="#define TEXTURED\n")throw std::runtime_error("Required nondefault blood shader technique");
 dh2_render_pass_convert_v3(&result.pass,&result.source);auto a=w(result.pass.data()),b=w(result.pass.data()+4);static const std::uint32_t blend[]{0,1,0x300,0x301,0x302,0x303,0x306,0x307,0x304,0x305,0x8001,0x8002,0x8003,0x8004,0x308};static const std::uint32_t equation[]{0x8006,0x800a,0x800b,0x8007,0x8008};static const std::uint32_t cull[]{0x405,0x404,0x408};
 const auto src=a&15,dst=(a>>4)&15,eq=(a>>24)&7,cf=a>>30;if(src>=15||dst>=15||eq>=5||cf>=3)throw std::runtime_error("Blood render state enum");result.blend_src=blend[src];result.blend_dst=blend[dst];result.blend_equation=equation[eq];result.depth_function=0x200+((a>>27)&7);result.cull_face=cull[cf];result.front_face=(b&(1<<18))?0x900:0x901;result.blend=b&(1<<16);result.cull=b&(1<<17);result.depth=b&(1<<19);result.depth_write=b&(1<<20);result.stencil=b&(1<<27);result.sample_coverage=b&(1<<25);result.polygon_offset=b&((1<<21)|(1<<22)|(1<<23));
 if(result.stencil||result.sample_coverage||result.polygon_offset)throw std::runtime_error("Required blood auxiliary render states");
 out=std::move(result);error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}}
}
#endif
