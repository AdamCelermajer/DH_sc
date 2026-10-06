#include "blood_texture_image_v1.hpp"
#include <algorithm>
namespace dh2::textures {
namespace {
bool source_extension(const std::string& s,const char* name){
 // initExtensions6dd734 only commits tokens terminated by space. Its final
 // unterminated token is discarded; preserve this original policy.
 std::size_t begin=0;
 while(true){const auto end=s.find(' ',begin);if(end==std::string::npos)return false;if(s.compare(begin,end-begin,name)==0)return true;begin=end+1;}
}
}
bool texture_gles2_sampler_features_v1(const std::string& extensions,const std::function<bool(std::uint32_t,float&,std::string&)>& query,Gles2TextureSamplerFeaturesV1& out,std::string& e){
 if(extensions.find('\0')!=std::string::npos){e="Unsupported embedded NUL in actual GL extension string";return false;}
 Gles2TextureSamplerFeaturesV1 f{};
 // genericDriverInit5b3c1c sets source cap4 unconditionally after the actual
 // max-texture-unit query. Remaining bits here are the exact consumed subset.
 f.capabilities9c=4;
 if(source_extension(extensions,"GL_EXT_texture3D")||source_extension(extensions,"GL_OES_texture_3D"))f.capabilities9c|=0x80;
 if(source_extension(extensions,"GL_EXT_texture_filter_anisotropic")){
  f.capabilities9c|=0x20000;
  if(!query){e="Required actual GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT query";return false;}
  if(!query(0x84ff,f.maximum_anisotropy4a8,e))return false;
 }
 if(source_extension(extensions,"GL_APPLE_texture_max_level"))f.parameter_flags7ec|=0x80000;
 f.pvrtc414=source_extension(extensions,"GL_IMG_texture_compression_pvrtc");
 f.rgba8_392_or_437=source_extension(extensions,"GL_OES_rgb8_rgba8")||source_extension(extensions,"GL_ARM_rgba8");
 out=f;return true;
}
bool blood_texture_format_flags_v1(std::uint32_t format,std::uint32_t& out,std::string& e){
 if(format==14){out=1;return true;}if(format==27){out=9;return true;}
 e="Unsupported blood texture format-property row";return false;
}
bool BloodTextureImageOwnerV1::initialize(std::shared_ptr<const std::vector<std::uint8_t>> image,const Gles2TextureSamplerFeaturesV1& features,const TextureManagerFieldsV1& manager,const std::uint32_t* options,const std::function<bool(std::uint32_t,std::uint32_t,std::string&)>& diagnostic,std::string& e){
 if(initialized_){e="Blood texture image owner already initialized";return false;}
 if(!image){e="Required actual retained material texture image";return false;}
 Description original{};View view{};
 if(!dh2_pvr_describe(image->data(),image->size(),&original)||original.kind||original.engine_format!=27||original.mipmapped||dh2_texture_open(image->data(),image->size(),&view)!=Error::ok){e="Unsupported blood texture source image domain (requires actual non-mipped PVRTC4 RGBA)";return false;}
 TextureDescriptorV1 descriptor{};
 if(!texture_image_descriptor_v1(descriptor,{original.engine_format,original.width,original.height,0},manager,options,e))return false;
 if(!features.pvrtc414){
  // Original createTextureImpl selects native14 without extension414.
  if(!diagnostic){e="Required source texture format27→14 conversion diagnostic";return false;}
  if(!diagnostic(27,14,e))return false;
  descriptor.format=14;converted_.resize(std::size_t(view.width)*view.height*4);
  const auto result=dh2_texture_decode(&view,converted_.data(),converted_.size());
  if(result!=Error::ok){e=dh2_texture_error(result);return false;}
 }
 if(!parameters_.construct(descriptor,e))return false;
 const auto count=static_cast<const TextureParameterOwnerV1&>(parameters_).fields().mip_count3e;
 offsets_.reserve(count+1);offsets_.push_back(0);
 for(unsigned mip=0;mip<count;++mip){
  const auto w=std::max(1u,view.width>>mip),h=std::max(1u,view.height>>mip);
  // Original format14/27 rows, getImageDataSize5edbec→5edb48. Format27:
  // block4×4,8 bytes,minimumtotal32 (not independent axis minimums).
  const auto bytes=descriptor.format==14?w*h*4:std::max(32u,((w+3)/4)*((h+3)/4)*8);
  offsets_.push_back(offsets_.back()+bytes);
 }
 if(!parameters_.set_data(true,e))return false;
 source_=std::move(image);features_=features;descriptor_=descriptor;pending_=1;initialized_=true;return true;
}
bool BloodTextureImageOwnerV1::upload_borrow(std::uint32_t* alignment,TextureUpload2dBorrowV1& out,std::string& e){
 if(!initialized_||!alignment){e="Required initialized blood texture and SAME driver unpack alignment";return false;}
 TextureUpload2dBorrowV1 d{};d.offsets=offsets_.data();d.offset_count=offsets_.size();d.pending_bits=&pending_;d.pending_word_count=1;d.driver_unpack_alignment26c=alignment;d.driver_capabilities9c=&features_.capabilities9c;
 if(descriptor_.format==14){d.bytes=converted_.data();d.size=converted_.size();d.format_flags=1;d.row_bytes=descriptor_.width*4;d.internal_format=features_.rgba8_392_or_437?0x8058:0x1908;d.pixel_format=0x1908;d.pixel_type=0x1401;}
 else{View view{};if(dh2_texture_open(source_->data(),source_->size(),&view)!=Error::ok){e="Retained blood texture image became invalid";return false;}d.bytes=view.payload;d.size=view.payload_size;d.format_flags=9;d.row_bytes=((descriptor_.width+3)/4)*8;d.internal_format=0x8c02;}
 out=d;return true;
}
}
