#include "text_filter_v1.hpp"
#include <algorithm>
#include <cstring>
#include <exception>
#include <limits>

namespace dh2::ui::text_filter_v1 {
std::uint32_t Filter::word(std::size_t n) const{
    if(n>40)return 0;
    std::uint32_t v=0;for(unsigned j=0;j<4;++j)v|=std::uint32_t(bytes[n+j])<<(8*j);return v;
}
float Filter::real(std::size_t n) const{auto w=word(n);float f;std::memcpy(&f,&w,4);return f;}
namespace {
void put(Filter& f,std::size_t n,std::uint32_t v){for(unsigned j=0;j<4;++j){f.bytes[n+j]=(v>>(8*j))&255;f.defined[n+j]=1;}}
void putreal(Filter& f,std::size_t n,float v){std::uint32_t w;std::memcpy(&w,&v,4);put(f,n,w);}
void color(Filter& f,const std::array<std::uint8_t,4>& c){
    f.bytes[4]=c[2];f.bytes[5]=c[1];f.bytes[6]=c[0];f.bytes[7]=c[3];
    for(unsigned i=4;i<8;++i)f.defined[i]=1;
}
}
bool read(Effect& effect,const Reader& r,std::string& e){
    if(!r.byte||!r.half||!r.bits||!r.bit||!r.fixed||!r.rgba){e="source filter reader requires all stream primitives";return false;}
    std::uint8_t count;if(!r.byte(count,e))return false;
    // Source reserve, not resize. Allocation failure preserves the old prefix.
    try{effect.filters.reserve(std::max(effect.filters.size(),std::size_t(count)));}
    catch(const std::exception& x){e=x.what();return false;}
    for(unsigned i=0;i<count;++i){
        std::uint8_t kind;if(!r.byte(kind,e))return false;
        Filter f;put(f,0,kind);
        float x{},y{},angle{},distance{};std::uint16_t strength{};
        std::uint32_t quality{},reserved{};std::uint8_t extra{};
        std::array<std::uint8_t,4> c{};bool bit{};
        if(kind>3)continue;
        if(kind==3){
            if(!r.rgba(c,e)||!r.rgba(c,e))return false;
            for(unsigned n=0;n<4;++n)if(!r.fixed(x,e))return false;
            if(!r.half(strength,e))return false;
            for(unsigned n=0;n<4;++n)if(!r.bit(bit,e))return false;
            if(!r.bits(4,quality,e)||!r.byte(extra,e))return false;
            continue;
        }
        if(kind!=1){if(!r.rgba(c,e))return false;color(f,c);}
        if(!r.fixed(x,e)||!r.fixed(y,e))return false;
        if(kind==0){
            if(!r.fixed(angle,e)||!r.fixed(distance,e))return false;
            putreal(f,8,angle);putreal(f,12,distance);
        }
        if(kind==1){
            if(!r.bits(5,quality,e)||!r.bits(3,reserved,e))return false;
            put(f,40,quality);
        }else{
            if(!r.half(strength,e))return false;
            // Original read_u16 then sxtb: the high byte is consumed but unused.
            putreal(f,kind==0?16:8,float(static_cast<std::int8_t>(strength&255)));
            const unsigned flag=kind==0?20:12;
            for(unsigned n=0;n<3;++n){if(!r.bit(bit,e))return false;f.bytes[flag+n]=bit;f.defined[flag+n]=1;}
            if(!r.bits(5,quality,e)||!r.byte(extra,e))return false;
            if(kind==0){put(f,24,extra);put(f,28,quality);}
            else{put(f,16,quality);put(f,40,1);}
        }
        putreal(f,32,x);putreal(f,36,y);
        try{effect.filters.push_back(f);}catch(const std::exception& z){e=z.what();return false;}
    }
    e.clear();return true;
}
bool Binding::set(const Effect& source,std::string& e){
    try{auto copied=std::make_shared<Effect>(source);value_=std::move(copied);e.clear();return true;}
    catch(const std::exception& z){e=z.what();return false;}
}
}
