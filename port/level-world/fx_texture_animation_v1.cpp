#include "fx_texture_animation_v1.hpp"
#include <cstring>
namespace dh2::fx {
bool texture_sample_v1(TextureTransform20V1& out,const assets::Animation& a,std::int32_t ms,bool interpolate,std::string& error){
 const auto* defaults=dh2_animation_default(&a);
 const auto n=dh2_animation_channels(&a);
 if(!defaults||n!=dh2_animation_samplers(&a)||!n||n>5){error="Unsupported source texture accessor/default";return false;}
 TextureTransform20V1 value;std::memcpy(&value,defaults,sizeof(value));
 // Source getValueEx initializes each channel cursor to zero and ANDs the
 // interpolation enable through the channel loop. Its public cursor is unused.
 for(std::uint32_t i=0;i<n;++i){
  const auto type=dh2_animation_type(&a,static_cast<std::int32_t>(i));
  assets::Vector values{},times{};
  if(type<87||type>91||!dh2_animation_vector(&a,i,true,&values)||!dh2_animation_vector(&a,i,false,&times)||
     values.type!=6||values.components!=1||!values.count||times.count!=values.count||times.components!=1||
     (times.type!=1&&times.type!=3&&times.type!=4)||dh2_animation_offsets(&a)||dh2_animation_scales(&a)){
   error="Unsupported source texture key layout";return false;
  }
  std::int32_t key=0;float t=0;
  interpolate=dh2_animation_find(&a,i,ms,&key,&t)&&interpolate;
  if(key<0||static_cast<std::uint32_t>(key)>=values.count){error="Source texture selected invalid key";return false;}
  float first{},next{},sample{};if(!dh2_vector_read(&values,key,&first)){error="Source texture key read";return false;}
  if(interpolate){
   if(static_cast<std::uint32_t>(key)+1>=values.count||!dh2_vector_read(&values,key+1,&next)){error="Source texture next key read";return false;}
   const float pair[2]{first,next};if(dh2_fx_texture_between_v1(&sample,pair,2,0,1,t)){error="Source texture interpolation";return false;}
  }else std::memcpy(&sample,&first,4);
  std::memcpy(reinterpret_cast<std::uint8_t*>(&value)+(type-87)*4,&sample,4);
 }
 out=value;return true;
}
}
extern "C" int dh2_fx_texture_sample_test_v1(void* output,const void* bytes,std::uint32_t size,std::int32_t animation,std::int32_t segment,std::int32_t ms,std::uint32_t interpolate){
 if(!output||!bytes||interpolate>1)return -1;
 dh2::resources::BresView image{};dh2::assets::Animation accessor{};dh2::fx::TextureTransform20V1 out;std::string error;
 if(dh2_bres_open(&image,bytes,size)!=dh2::resources::BresError::ok||dh2_animation_open(&accessor,&image,animation,segment)!=dh2::assets::Error::ok||!dh2::fx::texture_sample_v1(out,accessor,ms,interpolate!=0,error))return -1;
 std::memcpy(output,&out,20);return 0;
}
