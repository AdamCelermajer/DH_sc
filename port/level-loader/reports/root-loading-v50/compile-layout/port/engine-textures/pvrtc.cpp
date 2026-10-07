// Reconstructed from DH2 1.0.2 ARM instructions, not the newer SDK decoder.
// The legacy endpoint expansion and 2bpp modulation behavior are deliberate.
#include "textures.hpp"
#include <algorithm>
#include <array>

namespace {
using Color=std::array<int,4>;
struct Block { std::uint32_t bits,color; Color a,b; };
std::uint32_t le32(const std::uint8_t* p) {
    return p[0] | std::uint32_t(p[1])<<8 | std::uint32_t(p[2])<<16 | std::uint32_t(p[3])<<24;
}
unsigned twiddle(unsigned height,unsigned width,unsigned y,unsigned x) {
    const auto minimum=std::min(height,width);
    unsigned result=0,source=1,destination=1,shift=0;
    while(source<minimum) {
        if(y&source) result|=destination;
        if(x&source) result|=destination<<1;
        source<<=1;destination<<=2;++shift;
    }
    return result | ((height<width?x:y)>>shift)<<(2*shift);
}
Block block(const std::uint8_t* p) {
    Block b{}; b.bits=le32(p);b.color=le32(p+4);
    const unsigned a=b.color&0xfffe, c=b.color>>16;
    if(a&0x8000) b.a={int((a>>10)&31),int((a>>5)&31),int((a&31)|((a&31)>>4)),15};
    else {
        const auto red=(a>>7)&30,green=(a>>3)&30,blue=(a&15)<<1;
        b.a={int(red|(red>>4)),int(green|(green>>4)),int(blue|(blue>>3)),int((a>>11)&14)};
    }
    if(c&0x8000) b.b={int((c>>10)&31),int((c>>5)&31),int(c&31),15};
    else {
        const auto red=(c>>7)&30,green=(c>>3)&30;
        b.b={int(red|(red>>4)),int(green|(green>>4)),int((c&15)<<1),int((c>>11)&14)};
        // 0x69fb70..0x69fb84 updates the A endpoint's blue, even while
        // decoding transparent B. Keep this observable legacy quirk.
        b.a[2]|=b.a[2]>>4;
    }
    return b;
}
Color interpolate(const Color& p,const Color& q,const Color& r,const Color& s,
                  unsigned dx,unsigned dy,unsigned word_width) {
    Color result{};
    for(unsigned c=0;c<4;++c) {
        const int top=p[c]*int(word_width)+(q[c]-p[c])*int(dx);
        const int bottom=r[c]*int(word_width)+(s[c]-r[c])*int(dx);
        int value=top*4+(bottom-top)*int(dy);
        if(c<3) {value>>=word_width==8?2:1;value+=value>>5;}
        else {if(word_width==8)value>>=1;value+=value>>4;}
        result[c]=value;
    }
    return result;
}
}

extern "C" bool dh2_pvrtc_decompress(const void* encoded,std::size_t size,bool two,
                                    unsigned width,unsigned height,std::uint8_t* output,std::size_t capacity) {
    if(!encoded || !output || !width || !height || width>4096 || height>4096 ||
       (width&(width-1)) || (height&(height-1)) || capacity<std::size_t(width)*height*4) return false;
    const unsigned word_width=two?8:4,real_width=std::max(width,word_width*2),real_height=std::max(height,8u);
    const unsigned columns=real_width/word_width,rows=real_height/4;
    if(size<std::size_t(columns)*rows*8) return false;
    const auto* bytes=static_cast<const std::uint8_t*>(encoded);
    std::array<Block,4> blocks{};
    unsigned previous_x=~0u,previous_y=~0u;
    int values[8][16]{},modes[8][16]{};
    constexpr int normal[4]={0,3,5,8},punch[4]={0,4,4,8};
    for(unsigned y=0;y<height;++y) for(unsigned x=0;x<width;++x) {
        const unsigned bx=((x+real_width-word_width/2)&(real_width-1))/word_width;
        const unsigned by=((y+real_height-2)&(real_height-1))/4;
        if(bx!=previous_x || by!=previous_y) {
            previous_x=bx;previous_y=by;
            for(unsigned row=0;row<2;++row) for(unsigned column=0;column<2;++column) {
                const auto index=twiddle(rows,columns,(by+row)&(rows-1),(bx+column)&(columns-1));
                auto& b=blocks[row*2+column];b=block(bytes+index*8);
                auto bits=b.bits;const int mode=b.color&1;
                for(unsigned yy=0;yy<4;++yy) for(unsigned xx=0;xx<word_width;++xx) {
                    auto& value=values[row*4+yy][column*word_width+xx];
                    modes[row*4+yy][column*word_width+xx]=mode;
                    if(two && mode) {if(!((xx^yy)&1)){value=bits&3;bits>>=2;}}
                    else if(two) {value=(bits&1)?3:0;bits>>=1;}
                    else {value=bits&3;bits>>=2;}
                }
            }
        }
        const unsigned dx=(x+word_width/2)&(word_width-1),dy=(y+2)&3;
        const auto a=interpolate(blocks[0].a,blocks[1].a,blocks[2].a,blocks[3].a,dx,dy,word_width);
        const auto b=interpolate(blocks[0].b,blocks[1].b,blocks[2].b,blocks[3].b,dx,dy,word_width);
        const unsigned mx=dx+word_width/2,my=dy+2;
        const auto raw=values[my][mx];const auto mode=modes[my][mx];
        int modulation=normal[raw];bool transparent=false;
        if(!two && mode) {modulation=punch[raw];transparent=raw==2;}
        else if(two && mode && ((mx^my)&1)) {
            modulation=(normal[values[my-1][mx]]+normal[values[my+1][mx]]+
                        normal[values[my][mx-1]]+normal[values[my][mx+1]]+2)/4;
        }
        auto* target=output+(std::size_t(y)*width+x)*4;
        for(unsigned c=0;c<4;++c) target[c]=std::uint8_t((a[c]*8+(b[c]-a[c])*modulation)>>3);
        if(transparent) target[3]=0;
    }
    return true;
}
