#include "../blood_texture_image_v1.hpp"
#include <cassert>
#include <cstdio>
#include <array>
#include "../../../.local-inputs/blood_atlas_native_fixture_v1.inc"
using namespace dh2::textures;
int main(){
 std::string e;unsigned checks=0;Gles2TextureSamplerFeaturesV1 f{};
 assert(texture_gles2_sampler_features_v1("",{},f,e));assert(f.capabilities9c==4&&!f.pvrtc414);++checks;
 assert(texture_gles2_sampler_features_v1("GL_IMG_texture_compression_pvrtc",{},f,e));assert(!f.pvrtc414);++checks;
 assert(texture_gles2_sampler_features_v1("GL_IMG_texture_compression_pvrtc ",{},f,e)&&f.pvrtc414);++checks;
 assert(!texture_gles2_sampler_features_v1("GL_EXT_texture_filter_anisotropic ",{},f,e));++checks;
 assert(texture_gles2_sampler_features_v1("GL_EXT_texture_filter_anisotropic GL_OES_texture_3D GL_APPLE_texture_max_level ",[](auto key,float& value,std::string&){assert(key==0x84ff);value=16;return true;},f,e));assert(f.capabilities9c==0x20084&&f.parameter_flags7ec==0x80000&&f.maximum_anisotropy4a8==16);++checks;
 auto image=std::make_shared<const std::vector<std::uint8_t>>(std::begin(actual_blood_atlas),std::end(actual_blood_atlas));
 const std::uint32_t options=0x10;
 for(bool pvrtc:{false,true}){
  assert(texture_gles2_sampler_features_v1(pvrtc?"GL_IMG_texture_compression_pvrtc ":"",{},f,e));
  BloodTextureImageOwnerV1 owner;unsigned conversions=0;
  assert(owner.initialize(image,f,{},&options,[&](auto from,auto to,std::string&){assert(from==27&&to==14);++conversions;return true;},e));assert(conversions==unsigned(!pvrtc));++checks;
  assert(owner.descriptor().format==(pvrtc?27u:14u)&&owner.descriptor().mipmapped);++checks;
  std::uint32_t alignment=4;TextureUpload2dBorrowV1 data;assert(owner.upload_borrow(&alignment,data,e));++checks;
  const std::array<std::uint32_t,11> rgba_offsets{0,1048576,1310720,1376256,1392640,1396736,1397760,1398016,1398080,1398096,1398100},pvrtc_offsets{0,131072,163840,172032,174080,174592,174720,174752,174784,174816,174848};
  const auto& expected=pvrtc?pvrtc_offsets:rgba_offsets;assert(data.offset_count==expected.size());for(unsigned i=0;i<expected.size();++i)assert(data.offsets[i]==expected[i]);++checks;
  assert(data.internal_format==(pvrtc?0x8c02u:0x1908u));assert(data.size==(pvrtc?131072u:1048576u));assert(!owner.parameters().ready());++checks;
  TextureParameterServicesV1 params;params.integer=[](auto,auto,auto,std::string&){return true;};params.format_flags=blood_texture_format_flags_v1;params.compressed_filter_diagnostic=[](auto,std::string&){return true;};
  assert(owner.parameters().update(owner.driver(),params,e));++checks;
  TextureUpload2dServicesV1 upload;unsigned generated=0,diagnosed=0,images=0;
  upload.image=[&](auto target,auto level,auto w,auto h,auto internal,const auto* bytes,auto size,bool compressed,auto format,auto type,std::string&){assert(target==0xde1&&!level&&w==512&&h==512&&internal==data.internal_format&&bytes==data.bytes&&size==data.size&&compressed==pvrtc);if(!pvrtc)assert(format==0x1908&&type==0x1401);++images;return true;};
  upload.gl_error=[](std::uint32_t& value,std::string&){value=0;return true;};upload.generate_mipmaps=[&](std::string&){++generated;return true;};upload.compressed_mipmap_diagnostic=[&](std::string&){++diagnosed;return true;};
  assert(owner.parameters().upload(data,upload,e));assert(owner.parameters().ready()&&images==1&&generated==unsigned(!pvrtc)&&diagnosed==unsigned(pvrtc));++checks;
 }
 std::printf("PASS %u actual blood image preparation checks; both source-selected27/14, exact original mip offsets, callback upload domain (GPU not executed)\n",checks);
}
