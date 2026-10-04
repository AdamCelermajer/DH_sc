#include "gfnt.hpp"
#include <cstring>
#include <limits>
#include <stdexcept>
namespace {
using namespace dh2::ui;
constexpr std::uint32_t max_bytes=64*1024*1024,max_pixels=1024*1024;
std::uint32_t be(const std::uint8_t* p){return (std::uint32_t(p[0])<<24)|(std::uint32_t(p[1])<<16)|(std::uint32_t(p[2])<<8)|p[3];}
std::int32_t signed_word(std::uint32_t v){std::int32_t out;std::memcpy(&out,&v,4);return out;}
bool range(const void* p,std::size_t n){auto v=reinterpret_cast<std::uintptr_t>(p);return p&&n<=std::numeric_limits<std::uintptr_t>::max()-v;}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<y+m&&y<x+n;}
bool header(const GfntInput16& in){if(in.reserved||in.size<44||in.size>max_bytes||!range(in.bytes,in.size))return false;const auto* b=in.bytes;auto count=be(b+12),w=be(b+16),h=be(b+20);return be(b)==0x47464e54&&be(b+4)==0&&be(b+8)==0&&count&&count<=65536&&w&&h&&w<=4096&&h<=4096&&std::uint64_t(w)*h<=max_pixels&&40ull+4ull*(count+1)<=in.size;}
bool record(const GfntInput16& in,unsigned i,std::uint32_t& begin,std::uint32_t& end){auto count=be(in.bytes+12),table=40u+4u*(count+1);begin=be(in.bytes+40+4*i);end=be(in.bytes+44+4*i);if(begin<table||end<begin||end>in.size)return false;if(begin==end)return true;if(end-begin<5)return false;const auto target=be(in.bytes+16)*be(in.bytes+20);std::uint32_t done=0,p=begin+4;while(done<target){if(p>=end)return false;auto token=in.bytes[p++];unsigned n=(token&127)+1;if(n>target-done)return false;unsigned bytes=token&128?4:n*4;if(bytes>end-p)return false;p+=bytes;done+=n;}return p==end;}
std::int32_t f2iz(float v){if(!(v==v))return 0;if(v>=2147483648.0f)return 2147483647;if(v<=-2147483648.0f)return (-2147483647-1);return static_cast<std::int32_t>(v);}
float source_scale(std::int32_t value,std::int32_t height){float a=static_cast<float>(height);a=a*20.0f;const float denominator=1024.0f/a;return static_cast<float>(value)*denominator;}
}
extern "C" int dh2_gfnt_raster(GfntGlyph32* out,std::uint8_t* rgba,std::size_t capacity,const GfntInput16* input,std::uint32_t code,std::int32_t font_height){
 if(!range(out,sizeof(*out))||reinterpret_cast<std::uintptr_t>(out)%alignof(GfntGlyph32)||!range(input,sizeof(*input))||reinterpret_cast<std::uintptr_t>(input)%alignof(GfntInput16)||!header(*input))return -1;auto in=*input;
 if(overlap(out,sizeof(*out),in.bytes,in.size)||overlap(out,sizeof(*out),input,sizeof(*input)))return -1;
 auto first=be(in.bytes+36),count=be(in.bytes+12);auto i=code-first;if(i>=count)return 0;std::uint32_t start,end;if(!record(in,i,start,end))return -1;if(start==end)return 0;
 auto w=be(in.bytes+16),h=be(in.bytes+20),n=w*h*4;if(capacity<n||!range(rgba,capacity)||overlap(rgba,capacity,in.bytes,in.size)||overlap(rgba,capacity,input,sizeof(*input))||overlap(rgba,capacity,out,sizeof(*out)))return -1;
 GfntGlyph32 result{};result.metrics={unsigned(in.bytes[start])*256+in.bytes[start+1],be(in.bytes+24),w,h,0};auto right=unsigned(in.bytes[start+2])*256+in.bytes[start+3];auto advance=be(in.bytes+32)+1u+right-result.metrics.bearing_x;result.metrics.advance=f2iz(source_scale(signed_word(advance),font_height));result.pitch=w*4;const float base_height=static_cast<float>(be(in.bytes+28));const float base_pixels=base_height*20.0f;const float base_denominator=1024.0f/base_pixels;result.source_font_scale=base_height*base_denominator;
 std::uint32_t p=start+4,done=0;while(done<n){auto token=in.bytes[p++];unsigned run=(token&127)+1;if(token&128){for(unsigned j=0;j<run;++j)std::memcpy(rgba+done+j*4,in.bytes+p,4);p+=4;}else {std::memcpy(rgba+done,in.bytes+p,run*4);p+=run*4;}done+=run*4;}*out=result;return 1;
}
#ifndef DH2_GFNT_KERNEL_ONLY
namespace dh2::ui {
struct GfntFont::Snapshot {std::vector<std::uint8_t> bytes;};
bool GfntFont::load(const std::uint8_t* b,std::size_t n,std::string& error){error.clear();if(snapshot_&&snapshot_.use_count()>1){error="GFNT snapshot is borrowed";return false;}if(n>max_bytes){error="GFNT payload exceeds native bound";return false;}GfntInput16 in{b,static_cast<unsigned>(n),0};if(!header(in)){error="Malformed GFNT header";return false;}auto count=be(b+12);if(be(b+40)!=40+4*(count+1)||be(b+40+4*count)!=n){error="Malformed GFNT table extent";return false;}for(unsigned i=0;i<count;++i){std::uint32_t begin,end;if(!record(in,i,begin,end)){error="Malformed GFNT glyph record";return false;}}auto next=std::make_shared<Snapshot>();next->bytes.assign(b,b+n);snapshot_=std::move(next);return true;}
std::uint32_t GfntFont::Borrow::first_codepoint()const {if(!snapshot_)throw std::logic_error("Empty GFNT borrow");return be(snapshot_->bytes.data()+36);}
std::uint32_t GfntFont::Borrow::glyph_slots()const {if(!snapshot_)throw std::logic_error("Empty GFNT borrow");return be(snapshot_->bytes.data()+12);}
int GfntFont::Borrow::raster(GfntRaster& out,std::uint32_t code,std::int32_t height,std::string& error)const {error.clear();if(!snapshot_){error="Empty GFNT borrow";return -1;}const auto& b=snapshot_->bytes;GfntInput16 in{b.data(),static_cast<unsigned>(b.size()),0};GfntRaster next;next.rgba.resize(std::size_t(be(b.data()+16))*be(b.data()+20)*4);int result=dh2_gfnt_raster(&next.source,next.rgba.data(),next.rgba.size(),&in,code,height);if(result<0)error="Invalid GFNT raster request";if(result==1){next.advance_twips=static_cast<float>(next.source.metrics.advance)*20.0f;out=std::move(next);}return result;}
}
#endif
