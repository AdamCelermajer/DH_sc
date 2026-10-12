#include "swf_texture.hpp"
#include <utility>
namespace dh2::scene {
bool swf_texture_filename(std::string_view cwd,std::string_view request,std::string& output,std::string& error){
    if(cwd.size()>1024*1024||request.size()>1024*1024||request.empty()||cwd.find('\0')!=cwd.npos||request.find('\0')!=request.npos){error="Invalid source filename";return false;}
    const auto found=request.find(cwd);std::size_t prefix=0;
    if(found!=request.npos){prefix=found+cwd.size()+1;if(prefix>request.size()){error="Source filename suffix outside input";return false;}}
    std::string value(request.substr(prefix));
    if(value.find(".tga")!=value.npos){const auto slash=value.find_last_of("/\\");std::string replacement="ata/3d/textures/";replacement+=value.substr(slash==value.npos?0:slash+1);value=std::move(replacement);}
    for(char& c:value)if(c>='A'&&c<='Z')c=char(c+('a'-'A'));
    std::string result(request.substr(0,prefix));result+=value;output=std::move(result);error.clear();return true;
}
void swf_texture_set_wrap(SwfTextureState8& state,std::uint32_t requested)noexcept{
    auto value=state.packed;const auto masked=requested&7u;
    if(((value>>18)&7u)!=requested){value=(value&~0x1c0000u)|(masked<<18);state.dirty|=0x10;state.packed=value;}
    if(((value>>21)&7u)!=requested){value=(value&~0xe00000u)|(masked<<21);state.dirty|=0x20;state.packed=value;
        if(requested!=((value>>21)&7u)){value=(value&~0x7000000u)|(masked<<24);state.dirty|=0x40;state.packed=value;}}
}
bool swf_texture_archive_key(std::string_view request,bool ignore_case,bool ignore_path,std::string& output,std::string& error){
    if(request.size()>1024*1024||request.find('\0')!=request.npos){error="Invalid archive filename";return false;}
    std::string result(request);if(ignore_case)for(char& c:result)if(c>='A'&&c<='Z')c=char(c+32);
    if(ignore_path){const auto slash=result.find_last_of("/\\");if(slash!=result.npos&&slash!=0)result=result.substr(slash+1);}
    output=std::move(result);error.clear();return true;
}
std::optional<std::array<float,4>> swf_texture_diffuse(bool present,std::uint16_t id,std::uint32_t flags)noexcept{
    if(!present||id==0xffff)return std::nullopt;return ((flags>>4)&63u)==2?std::array<float,4>{1,1,1,0}:std::array<float,4>{0,0,0,0};
}
bool swf_texture_gl_wrap(std::uint32_t code,std::uint32_t& out)noexcept {constexpr std::uint32_t table[]{0x2901,0x812f,0x812f,0x812f,0x2901};if(code>=5)return false;out=table[code];return true;}
bool swf_texture_gl_filter(std::uint32_t code,std::uint32_t& out)noexcept {constexpr std::uint32_t table[]{0x2600,0x2601,0x2700,0x2701,0x2702,0x2703};if(code>=6)return false;out=table[code];return true;}
SwfRenderState32 swf_render_state(const SwfSourceRenderState76& source)noexcept {
    const auto& s=source.words;SwfRenderState32 result{};auto& d=result.words;
    d[0]=((s[2]>>12)&7u)<<24|(s[0]&255u)|(s[2]&0xc0000000u)|((s[3]>>12)&7u)<<27|((s[0]>>16)&255u)<<8|((s[0]>>24)&255u)<<16;
    d[1]=((s[3]>>19)&1u)<<16|((s[3]>>20)&1u)<<17|((s[3]>>21)&1u)<<18|((s[3]>>22)&1u)<<19|((s[3]>>23)&1u)<<20;
    d[1]|=((s[3]>>15)&3u)<<12|((s[3]>>17)&3u)<<14;
    d[1]|=((s[3]>>25)&63u)<<21|(s[4]&1u)<<27;
    d[1]|=((s[2]>>18)&7u)|((s[2]>>21)&7u)<<3|((s[2]>>24)&7u)<<6|((s[2]>>27)&7u)<<9;
    d[2]=s[5];d[3]=s[10];d[4]=s[11];d[5]=s[12];d[6]=s[13];d[7]=s[14];return result;
}
bool swf_render_blend(const SwfRenderState32& state,SwfBlend16& out)noexcept {
    constexpr std::uint32_t factors[]{0,1,0x300,0x301,0x302,0x303,0x306,0x307,0x304,0x305,0x8001,0x8002,0x8003,0x8004,0x308};
    constexpr std::uint32_t equations[]{0x8006,0x800a,0x800b,0x8007,0x8008};
    const auto value=state.words[0],equation=(value>>24)&7u,source=value&15u,destination=(value>>4)&15u;
    if(equation>=5||source>=15||destination>=15)return false;
    out={((state.words[1]>>16)&1u),equations[equation],factors[source],factors[destination]};return true;
}
namespace {float color_bound(float value,float low,float high)noexcept {if(!(value<high))return high;if(!(value>low))return low;return value;}}
std::array<std::uint8_t,4> swf_solid_color(const SwfCxform32& cx,const std::array<std::uint8_t,4>& rgba)noexcept {
    std::array<std::uint8_t,4> out{};for(unsigned i=0;i<4;++i){const float product=float(rgba[i])*cx.terms[2*i];const float sum=product+cx.terms[2*i+1];out[i]=std::uint8_t(std::uint32_t(color_bound(sum,0,255)));}return out;
}
SwfBitmapColor40 swf_bitmap_color(const SwfCxform32& cx)noexcept {
    SwfBitmapColor40 out{};for(unsigned i=0;i<4;++i){out.clamped.terms[2*i]=color_bound(cx.terms[2*i],0,1);out.clamped.terms[2*i+1]=color_bound(cx.terms[2*i+1],-255,255);const float value=out.clamped.terms[2*i]*255;out.rgba[i]=std::uint8_t(std::uint32_t(value));if(out.clamped.terms[2*i+1]>1)out.additive=1;}return out;
}
}
