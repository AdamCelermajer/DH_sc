#include "texture_owner_v1.hpp"
#include "texture_mipmap_v1.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
namespace dh2::textures {
bool texture_image_descriptor_v1(TextureDescriptorV1& out,const ImageMetadataV1& image,const TextureManagerFieldsV1& manager,const std::uint32_t* flags,std::string& error){
 if(!flags||!image.width10||!image.height14||image.width10>0x7fffffffu||image.height14>0x7fffffffu||image.mipmapped28>1){error="Required actual image metadata/driver flags88";return false;}
 TextureDescriptorV1 d{};d.format=image.format20;d.width=image.width10;d.height=image.height14;
 d.mipmapped=std::uint8_t((*flags&0x10)!=0||(image.mipmapped28&&(manager.flags74&0x40)));
 d.usage=manager.flags74&0x20?3:manager.flags74&0x10?1:0;out=d;return true;
}
bool texture_sampler_constructor_v1(TextureSamplerFieldsV1& out,const TextureDescriptorV1& d,std::string& error){
 if(d.kind>3||d.format>63||d.layout>3||d.usage>3||!d.width||!d.height||!d.depth||d.mipmapped>1||d.render_target>1||d.reserved){error="Unsupported source texture descriptor";return false;}
 TextureSamplerFieldsV1 s{};s.dirty40=0x1ffd;s.flags3f=d.render_target?4:0;s.anisotropy44=1;s.lod_bias48=0;
 auto log=[](std::uint32_t x){unsigned n=0;while(x>>=1)++n;return n;};
 s.mip_count3e=std::uint8_t(d.mipmapped?std::max({log(d.width),log(d.height),log(d.depth)})+1:1);
 s.max_lod50=float(s.mip_count3e-1);
 s.parameters38=(d.kind&3)|((d.layout&3)<<2)|((d.usage&3)<<10)|((d.format&63)<<4)|(d.mipmapped?0x3000:0x1000)|0x8000;out=s;return true;
}
bool texture_update_parameters_v1(TextureSamplerFieldsV1& s,const TextureDriverBorrowV1& d,const TextureParameterServicesV1& cb,std::string& e){
 static constexpr std::uint32_t targets[]{0xde1,0x806f,0x8513,0x84f5},filters[]{0x2600,0x2601,0x2700,0x2701,0x2702,0x2703},wraps[]{0x2901,0x812f,0x812f,0x812f,0x2901,0x1401,0x1403,0};
 const auto target=targets[s.parameters38&3];
 auto integer=[&](std::uint32_t name,std::uint32_t value){if(!cb.integer){e="Required source glTexParameteri delivery";return false;}return cb.integer(target,name,std::int32_t(value),e);};
 auto filter=[&](unsigned shift,std::uint32_t name){const auto i=(s.parameters38>>shift)&7;if(i>=6){e="Unsupported original filter table index";return false;}return integer(name,filters[i]);};
 if(s.dirty40&4){
  if(s.flags3f&2){std::uint32_t flags{};if(!cb.format_flags){e="Required source texture format-property table";return false;}if(!cb.format_flags((s.parameters38>>4)&63,flags,e))return false;
   if(flags&8){if(!cb.compressed_filter_diagnostic){e="Required source compressed-filter diagnostic";return false;}if(!cb.compressed_filter_diagnostic((s.parameters38>>4)&63,e))return false;
    if((s.parameters38>>12)&7){s.parameters38&=~0x7000u;s.dirty40|=4;}
   }
  }
  if(!filter(12,0x2801))return false;
 }
 if((s.dirty40&8)&&!filter(15,0x2800))return false;
 if((s.dirty40&0x10)&&!integer(0x2802,wraps[(s.parameters38>>18)&7]))return false;
 if((s.dirty40&0x20)&&!integer(0x2803,wraps[(s.parameters38>>21)&7]))return false;
 if(!d.capabilities9c||!d.flags7ec){e="Required actual texture driver capabilities9c/flags7ec";return false;}
 if((s.dirty40&0x40)&&(*d.capabilities9c&0x80)&&!integer(0x2803,wraps[(s.parameters38>>21)&7]))return false;
 if((s.dirty40&0x80)&&(*d.capabilities9c&0x20000)){
  if(!d.maximum_anisotropy4a8||!cb.scalar){e="Required actual maximum anisotropy/glTexParameterf";return false;}
  const auto value=*d.maximum_anisotropy4a8<s.anisotropy44?*d.maximum_anisotropy4a8:s.anisotropy44;
  if(!cb.scalar(target,0x84fe,value,e))return false;
 }
 if((*d.flags7ec&0x80000)&&(s.dirty40&0x400)){
  const float value=((s.parameters38>>12)&7)<=3?s.max_lod50+.5f:std::ceil(s.max_lod50);
  if(!std::isfinite(value)||double(value)<-2147483648.||double(value)>=2147483648.){e="Unsupported source max-LOD integer conversion";return false;}
  if(!integer(0x813d,std::uint32_t(std::int32_t(value))))return false;
 }
 s.dirty40&=0xe003;return true;
}
bool TextureParameterOwnerV1::construct(const TextureDescriptorV1& d,std::string& e){
 if(constructed_){e="Texture parameter owner already constructed";return false;}if(!texture_sampler_constructor_v1(fields_,d,e))return false;descriptor_=d;constructed_=true;return true;
}
bool TextureParameterOwnerV1::update(const TextureDriverBorrowV1& d,const TextureParameterServicesV1& cb,std::string& e){
 if(!constructed_){e="Required actual source texture construction";return false;}parameters_delivered_=false;
 if(!texture_update_parameters_v1(fields_,d,cb,e))return false;
 parameters_delivered_=true;return true;
}
bool TextureParameterOwnerV1::set_data(bool generate,std::string& e){
 if(!constructed_){e="Required actual source texture construction before setData";return false;}
 if(fields_.mip_count3e>1&&generate)fields_.flags3f|=2;else fields_.flags3f&=~2;
 fields_.dirty40|=1;uploaded_=parameters_delivered_=false;return true;
}
bool texture_upload_2d_v1(TextureSamplerFieldsV1& s,const TextureDescriptorV1& d,const TextureUpload2dBorrowV1& data,const TextureUpload2dServicesV1& cb,std::string& e){
 if(d.kind!=0||!data.bytes||!data.offsets||data.offset_count<std::size_t(s.mip_count3e)+1||!data.driver_capabilities9c||!data.pending_bits||data.pending_word_count<(unsigned(s.mip_count3e)+31)/32||!data.driver_unpack_alignment26c||!cb.image||!cb.gl_error){e="Required actual 2D texture payload/offsets/pending bits/driver upload services";return false;}
 const unsigned count=s.flags3f&2?1:s.mip_count3e;
 if(!count){e="Unsupported zero source mip count";return false;}
 const auto alignment=data.row_bytes&1?1u:4u-(data.row_bytes&3);
 if(*data.driver_unpack_alignment26c!=alignment){if(!cb.pixel_store){e="Required actual glPixelStorei unpack alignment";return false;}if(!cb.pixel_store(0xcf5,alignment,e))return false;*data.driver_unpack_alignment26c=alignment;}
 std::uint32_t discarded_error{};if(!cb.gl_error(discarded_error,e))return false;
 for(unsigned i=0;i<count;++i){
  if(!(data.pending_bits[i/32]&(1u<<(i%32))))continue;
  const auto begin=data.offsets[i],end=data.offsets[i+1];
  if(begin>end||end>data.size){e="Invalid source texture mip payload range";return false;}
  const auto width=std::max(1u,d.width>>i),height=std::max(1u,d.height>>i);
  if(!cb.image(0xde1,i,width,height,data.internal_format,data.bytes+begin,end-begin,(data.format_flags&8)!=0,data.pixel_format,data.pixel_type,e))return false;
  std::uint32_t gl_error{};if(!cb.gl_error(gl_error,e))return false;
  if(gl_error)s.flags3f|=0x10;
 }
 for(std::size_t i=0;i<data.pending_word_count;++i)data.pending_bits[i]=0;
 // Whole source supported 2D updateData tail, 5b036c..5b0438.
 s.dirty40&=~3;
 if(s.flags3f&0x10){e="Source texture GL upload error flag10";return false;}
 if(s.mip_count3e>1&&(s.flags3f&2)){
  if(data.format_flags&8){if(!cb.compressed_mipmap_diagnostic){e="Required source compressed mipmap diagnostic";return false;}return cb.compressed_mipmap_diagnostic(e);}
  if(*data.driver_capabilities9c&4){if(!cb.generate_mipmaps){e="Required actual texture virtual20 generateMipmaps";return false;}return cb.generate_mipmaps(e);}
 }
 return true;
}
bool TextureParameterOwnerV1::upload(const TextureUpload2dBorrowV1& d,const TextureUpload2dServicesV1& cb,std::string& e){
 if(!constructed_||!parameters_delivered_){e="Required ordered source sampler update before texture upload";return false;}
 auto actual=cb;
 if(cb.generate_mipmaps)actual.generate_mipmaps=[&](std::string& error){generation_delivery_=true;struct Reset{bool& value;~Reset(){value=false;}} reset{generation_delivery_};return cb.generate_mipmaps(error);};
 uploaded_=false;if(!texture_upload_2d_v1(fields_,descriptor_,d,actual,e))return false;uploaded_=true;return true;
}
bool TextureParameterOwnerV1::generate_mipmaps(const void* texture,const TextureMipmapDriverBorrowV1& d,const TextureMipmapServicesV1& cb,std::string& e){
 if(!constructed_||!generation_delivery_){e="Required reached source upload mipmap continuation";return false;}
 return texture_generate_mipmaps_v1(fields_,texture,d,cb,e);
}
}
