#include "textures.hpp"
#include <algorithm>
#include <cstring>
#include <limits>

namespace {
std::uint32_t word(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | std::uint32_t(p[1])<<8 | std::uint32_t(p[2])<<16 | std::uint32_t(p[3])<<24;
}
std::uint32_t half(const std::uint8_t* p) { return p[0] | std::uint32_t(p[1])<<8; }
std::uint32_t log2floor(std::uint32_t n) { std::uint32_t v=0; while(n >>= 1) ++v; return v; }
bool power2(std::uint32_t n) { return n && !(n & (n-1)); }
const std::uint8_t* pvr_header(const void* data, std::size_t size, std::size_t& offset) {
    if (!data) return nullptr;
    const auto* p=static_cast<const std::uint8_t*>(data);
    offset=size>=8 && !std::memcmp(p,"BTEXpvr\0",8) ? 8 : 0;
    if(size<offset+52) return nullptr;
    p+=offset;
    if(word(p)!=52 || std::memcmp(p+44,"PVR!",4)) return nullptr;
    return p;
}
std::uint32_t original_format(std::uint32_t type, bool alpha) {
    switch(type) {
    case 0: return 6; case 1:return 8; case 2:case 19:return 5;
    case 4:case 21:return 10; case 5:case 26:return 13; case 7:case 22:return 0;
    case 8:case 23:return 4; case 12:case 24:return alpha?25:24;
    case 13:case 25:return alpha?27:26; case 16:return 7; case 17:return 9;
    case 18:return 14; case 32:return alpha?18:17; case 33:case 34:return 19;
    case 35:case 36:return 20; case 42:return 16; case 57:return 2; case 59:return 1;
    case 80:return 31; case 83:return 30; case 86:return 29;
    default:return UINT32_MAX;
    }
}
}

extern "C" bool dh2_pvr_describe(const void* data, std::size_t size, dh2::textures::Description* out) {
    if(!out) return false;
    std::size_t offset=0;
    const auto* h=pvr_header(data,size,offset);
    if(!h) return false;
    const auto flags=word(h+16), width=word(h+8), height=word(h+4), mips=word(h+12), surfaces=word(h+48);
    if(!width || !height || width>16384 || height>16384) return false;
    if((flags&0x100) && !mips) return false;
    if((flags&0x1000) && surfaces!=6) return false;
    const std::uint32_t kind=flags&0x1000?2:flags&0x4000?1:0;
    const std::uint32_t depth=kind==1?surfaces:1;
    if((flags&0x100) && mips!=std::max({log2floor(width),log2floor(height),log2floor(depth)})) return false;
    const auto bytes=word(h+20);
    if(std::uint64_t(bytes)*(kind==2?6:1)!=size-offset-52) return false;
    const auto type=flags&255, format=original_format(type,(flags&0x8000)!=0);
    if(format==UINT32_MAX) return false;
    // The format-property table permits twiddling for compressed formats.
    // Its uncompressed branch is outside this first texture reconstruction.
    if((flags&0x200) && !(format>=17 && format<=27)) return false;
    *out={kind,format,0,0,width,height,depth,(flags>>8)&1};
    return true;
}

extern "C" dh2::textures::Error dh2_texture_open(const void* data, std::size_t size, dh2::textures::View* out) {
    using namespace dh2::textures;
    if(!data || !out) return Error::argument;
    const auto* p=static_cast<const std::uint8_t*>(data);
    View v{}; std::size_t offset=0;
    if(const auto* h=pvr_header(data,size,offset)) {
        Description desc{};
        if(!dh2_pvr_describe(data,size,&desc)) return Error::header;
        const auto type=word(h+16)&255;
        if(desc.kind || desc.mipmapped || (type!=24 && type!=25)) return Error::unsupported;
        v.width=desc.width; v.height=desc.height; v.format=type==24?Format::pvrtc2:Format::pvrtc4;
        v.alpha=(word(h+16)&0x8000)!=0; v.top_origin=1;
        if(!power2(v.width) || !power2(v.height) || v.width>4096 || v.height>4096) return Error::dimensions;
        const auto expected=std::size_t(std::max(v.width,type==24?16u:8u))*std::max(v.height,8u)*(type==24?2:4)/8;
        if(word(h+24)!=(type==24?2u:4u) || word(h+20)!=expected) return Error::header;
        v.payload=p+offset+52; v.payload_size=expected;
    } else {
        // Checked conventional uncompressed true-colour TGA. RLE/colour maps
        // are deliberately unsupported; all eight ordinary cache TGAs are type 2.
        if(size<18) return Error::truncated;
        if(p[1] || p[2]!=2 || (p[16]!=24 && p[16]!=32) || (p[17]&0xc0)) return Error::format;
        v.width=half(p+12); v.height=half(p+14);
        if(!v.width || !v.height || v.width>4096 || v.height>4096) return Error::dimensions;
        v.format=p[16]==24?Format::tga_bgr24:Format::tga_bgra32;
        v.alpha=p[16]==32 && (p[17]&15)!=0;
        v.top_origin=(p[17]>>5)&1; v.right_origin=(p[17]>>4)&1;
        const auto start=std::size_t(18)+p[0], count=std::size_t(v.width)*v.height*(p[16]/8);
        if(start>size || count>size-start) return Error::truncated;
        v.payload=p+start; v.payload_size=count;
    }
    *out=v; return Error::ok;
}

extern "C" dh2::textures::Error dh2_texture_decode(const dh2::textures::View* v, std::uint8_t* output, std::size_t capacity) {
    using namespace dh2::textures;
    if(!v || !output || !v->payload) return Error::argument;
    if(!v->width || !v->height || v->width>4096 || v->height>4096) return Error::dimensions;
    const auto bytes=std::size_t(v->width)*v->height*4;
    if(capacity<bytes) return Error::capacity;
    if(v->format==Format::pvrtc2 || v->format==Format::pvrtc4) {
        const bool two=v->format==Format::pvrtc2;
        const auto expected=std::size_t(std::max(v->width,two?16u:8u))*std::max(v->height,8u)*(two?2:4)/8;
        if(!power2(v->width)||!power2(v->height)||v->payload_size<expected) return Error::truncated;
        if(!dh2_pvrtc_decompress(v->payload,v->payload_size,two,v->width,v->height,output,capacity)) return Error::truncated;
        if(!v->alpha) for(std::size_t i=3;i<bytes;i+=4) output[i]=255;
    } else if(v->format==Format::tga_bgr24 || v->format==Format::tga_bgra32) {
        const std::size_t stride=v->format==Format::tga_bgr24?3:4;
        if(v->payload_size<std::size_t(v->width)*v->height*stride) return Error::truncated;
        for(std::uint32_t y=0;y<v->height;++y) for(std::uint32_t x=0;x<v->width;++x) {
            const auto sx=v->right_origin?v->width-1-x:x, sy=v->top_origin?y:v->height-1-y;
            const auto* s=v->payload+(std::size_t(sy)*v->width+sx)*stride;
            auto* d=output+(std::size_t(y)*v->width+x)*4;
            d[0]=s[2];d[1]=s[1];d[2]=s[0];d[3]=v->alpha && stride==4?s[3]:255;
        }
    } else return Error::format;
    return Error::ok;
}

extern "C" const char* dh2_texture_error(dh2::textures::Error error) {
    static const char* const names[]={"ok","invalid argument","invalid PVR header","invalid dimensions","unsupported format","truncated input","output capacity","unsupported texture feature"};
    const auto i=static_cast<unsigned>(error); return i<sizeof(names)/sizeof(*names)?names[i]:"unknown error";
}
