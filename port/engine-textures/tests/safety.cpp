#include "textures.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <random>
#include <vector>
#include <iostream>
using namespace dh2::textures;
int main() {
    // The six bytes encode a 2x1 BGR TGA with bottom-left origin.
    std::array<std::uint8_t,24> tga{};tga[2]=2;tga[12]=2;tga[14]=1;tga[16]=24;
    tga[18]=30;tga[19]=20;tga[20]=10;tga[21]=60;tga[22]=50;tga[23]=40;
    View view{};assert(dh2_texture_open(tga.data(),tga.size(),&view)==Error::ok);
    std::array<std::uint8_t,12> target{};target.fill(0xcd);
    assert(dh2_texture_decode(&view,target.data()+2,8)==Error::ok);
    const std::array<std::uint8_t,8> expected={10,20,30,255,40,50,60,255};
    assert(!std::memcmp(target.data()+2,expected.data(),8));
    assert(target.front()==0xcd && target.back()==0xcd);
    view.alpha=1;assert(dh2_texture_decode(&view,target.data()+2,8)==Error::ok);
    assert(target[5]==255 && target[9]==255);view.alpha=0;
    view.right_origin=1;assert(dh2_texture_decode(&view,target.data()+2,8)==Error::ok);
    assert(target[2]==40 && target[6]==10);
    for(std::size_t n=0;n<tga.size();++n) assert(dh2_texture_open(tga.data(),n,&view)!=Error::ok);
    assert(dh2_texture_open(nullptr,0,&view)==Error::argument);
    assert(dh2_texture_open(tga.data(),tga.size(),nullptr)==Error::argument);
    assert(dh2_texture_decode(nullptr,target.data(),target.size())==Error::argument);
    view={tga.data()+18,6,2,1,Format::tga_bgr24,0,0,0};
    assert(dh2_texture_decode(&view,target.data()+2,7)==Error::capacity);
    view.width=0;assert(dh2_texture_decode(&view,target.data(),target.size())==Error::dimensions);
    // Misaligned compressed input must not require uint32_t alignment.
    std::array<std::uint8_t,33> compressed{};std::array<std::uint8_t,256> rgba{};
    assert(dh2_pvrtc_decompress(compressed.data()+1,32,false,8,8,rgba.data(),rgba.size()));
    assert(!dh2_pvrtc_decompress(compressed.data()+1,31,false,8,8,rgba.data(),rgba.size()));
    assert(!dh2_pvrtc_decompress(compressed.data()+1,32,false,8,8,rgba.data(),rgba.size()-1));
    assert(!dh2_pvrtc_decompress(compressed.data()+1,32,false,7,8,rgba.data(),rgba.size()));
    std::mt19937 random(0xD22026);std::array<std::uint8_t,512> input{};
    std::size_t accepted=0;
    for(unsigned i=0;i<25000;++i) {
        const auto n=random()%input.size();for(auto& b:input)b=std::uint8_t(random());
        Description desc{};dh2_pvr_describe(input.data(),n,&desc);
        if(dh2_texture_open(input.data(),n,&view)==Error::ok) {
            ++accepted;std::vector<std::uint8_t> out(std::size_t(view.width)*view.height*4);
            assert(dh2_texture_decode(&view,out.data(),out.size())==Error::ok);
        }
    }
    std::cout<<"Safety/orientation checks passed; 25000 random buffers, "<<accepted<<" accepted.\n";
}
