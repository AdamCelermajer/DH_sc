#include "../swf_texture.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace dh2::scene;
extern "C" __attribute__((visibility("default"))) std::uint32_t dh2_swf_texture_test(std::uint32_t op,const std::uint8_t* in,std::uint32_t size,std::uint8_t* out){
    if(!in||!out)return 0;auto word=[&](std::uint32_t offset){std::uint32_t value;std::memcpy(&value,in+offset,4);return value;};
    if(op==0){if(size<8)return 0;const auto a=word(0),b=word(4);if(std::uint64_t(a)+b+8!=size)return 0;std::string result,error;if(!swf_texture_filename(std::string_view(reinterpret_cast<const char*>(in+8),a),std::string_view(reinterpret_cast<const char*>(in+8+a),b),result,error))return 0;std::memcpy(out,result.data(),result.size());return std::uint32_t(result.size());}
    if(op==1){if(size!=12)return 0;SwfTextureState8 state;std::memcpy(&state,in,8);swf_texture_set_wrap(state,word(8));std::memcpy(out,&state,8);return 8;}
    if(op==2){if(size!=12)return 0;auto value=swf_texture_diffuse(word(0)!=0,std::uint16_t(word(4)),word(8));std::uint32_t present=bool(value);std::memcpy(out,&present,4);if(value)std::memcpy(out+4,value->data(),16);return value?20:4;}
    if(op==3||op==4){if(size!=4)return 0;std::uint32_t value=0;bool ok=op==3?swf_texture_gl_wrap(word(0),value):swf_texture_gl_filter(word(0),value);if(!ok)return 0;std::memcpy(out,&value,4);return 4;}
    if(op==5){if(size!=76)return 0;SwfSourceRenderState76 source;std::memcpy(&source,in,76);auto value=swf_render_state(source);std::memcpy(out,&value,32);return 32;}
    if(op==6){if(size!=32)return 0;SwfRenderState32 source;std::memcpy(&source,in,32);SwfBlend16 value{};if(!swf_render_blend(source,value))return 0;std::memcpy(out,&value,16);return 16;}
    if(op==7){if(size<8)return 0;std::string result,error;if(!swf_texture_archive_key(std::string_view(reinterpret_cast<const char*>(in+8),size-8),word(0)!=0,word(4)!=0,result,error))return 0;std::memcpy(out,result.data(),result.size());return std::uint32_t(result.size());}
    if(op==8){if(size!=36)return 0;SwfCxform32 cx;std::array<std::uint8_t,4> rgba;std::memcpy(&cx,in,32);std::memcpy(rgba.data(),in+32,4);auto color=swf_solid_color(cx,rgba);std::memcpy(out,color.data(),4);return 4;}
    if(op==9){if(size!=32)return 0;SwfCxform32 cx;std::memcpy(&cx,in,32);auto color=swf_bitmap_color(cx);std::memcpy(out,&color,40);return 40;}
    return 0;
}
#ifndef DH2_SWF_TEXTURE_ORACLE
int main(int argc,char**argv){
    if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<std::uint8_t> data((std::istreambuf_iterator<char>(f)),{});std::size_t at=0;auto word=[&](){if(at+4>data.size())throw std::runtime_error("truncated gold");std::uint32_t v;std::memcpy(&v,data.data()+at,4);at+=4;return v;};if(word()!=0x31544653)return 3;auto count=word();std::vector<std::uint8_t> out(1024*1024+16);
    for(std::uint32_t i=0;i<count;++i){auto op=word(),n=word();if(at+n>data.size())return 4;auto input=data.data()+at;at+=n;auto expected=word();if(at+expected>data.size())return 4;auto actual=dh2_swf_texture_test(op,input,n,out.data());if(actual!=expected||std::memcmp(out.data(),data.data()+at,expected)!=0){std::cerr<<"Mismatch "<<i<<'\n';return 5;}at+=expected;}
    std::string out_string="preserved",error;unsigned guards=0;
    for(const auto& pair:std::vector<std::pair<std::string,std::string>>{{"",""},{"abc","abc"},{"",std::string("x\0y",3)},{std::string("x\0",2),"data/a.tga"}}){if(swf_texture_filename(pair.first,pair.second,out_string,error)||out_string!="preserved")return 6;++guards;}
    std::uint32_t result=0x12345678;for(auto bad:{5u,7u,0xffffffffu}){if(swf_texture_gl_wrap(bad,result)||result!=0x12345678)return 7;++guards;}
    for(auto bad:{6u,7u,0xffffffffu}){if(swf_texture_gl_filter(bad,result)||result!=0x12345678)return 7;++guards;}
    SwfRenderState32 invalid{};invalid.words[0]=0x07000000;SwfBlend16 blend{9,8,7,6};if(swf_render_blend(invalid,blend)||blend.enabled!=9||blend.equation!=8||blend.source!=7||blend.destination!=6)return 8;++guards;
    if(at!=data.size())return 9;std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"atomic_guards\":"<<guards<<",\"mismatches\":0}\n";return 0;
}
#endif
