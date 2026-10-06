#include "../texture_owner_v1.hpp"
#include "../texture_driver_fields_v1.hpp"
#include <cassert>
#include <cstdio>
#include <vector>
#include <array>
#include "../../../.local-inputs/blood_atlas_native_fixture_v1.inc"
using namespace dh2::textures;
int main(){
 std::string e;unsigned checks=0;
 TextureDriverOptionsOwnerV1 options;assert(options.flags88()==0x10);++checks;
 assert(options.set_option(0x400,true,{},e)&&options.flags88()==0x410);++checks;
 assert(!options.set_option(0x100,false,{},e)&&options.flags88()==0x410);++checks;
 ShaderAdditionalConfigOwnerV1 config;bool tracing=false;unsigned reads=0;
 ShaderConfigFileServicesV1 cfg;cfg.trace_missing=&tracing;cfg.read=[&](const char* name,bool& found,std::string& out,std::string&){assert(std::string(name)=="glsl.config");found=++reads>=2;out.clear();return true;};
 assert(config.initialize(cfg,e)&&config.config()==nullptr);++checks;
 assert(config.initialize(cfg,e)&&config.config()!=nullptr&&config.config()->empty()&&config.length80()==0);++checks;
 assert(config.initialize(cfg,e)&&reads==2);++checks;
 const std::uint32_t caps=0x80,ext=0;TextureDriverBorrowV1 driver{&caps,&ext,nullptr};
 static constexpr std::uint32_t targets[]{0xde1,0x806f,0x8513,0x84f5},filters[]{0x2600,0x2601,0x2700,0x2701,0x2702,0x2703},wraps[]{0x2901,0x812f,0x812f,0x812f,0x2901,0x1401,0x1403,0};
 for(unsigned target=0;target<4;++target)for(unsigned fi=0;fi<6;++fi)for(unsigned wrap=0;wrap<8;++wrap){
  TextureSamplerFieldsV1 f{};f.parameters38=target|(fi<<12)|(1<<15)|(wrap<<18)|(wrap<<21);f.dirty40=0x1ffd;
  std::vector<std::array<std::uint32_t,3>> calls;TextureParameterServicesV1 cb;cb.integer=[&](auto t,auto p,auto v,std::string&){calls.push_back({t,p,std::uint32_t(v)});return true;};
  assert(texture_update_parameters_v1(f,driver,cb,e));assert(f.dirty40==1);
  const std::vector<std::array<std::uint32_t,3>> expected{{targets[target],0x2801,filters[fi]},{targets[target],0x2800,0x2601},{targets[target],0x2802,wraps[wrap]},{targets[target],0x2803,wraps[wrap]},{targets[target],0x2803,wraps[wrap]}};
  assert(calls==expected);++checks;
 }
 Description image{};assert(dh2_pvr_describe(actual_blood_atlas,sizeof actual_blood_atlas,&image));assert(image.engine_format==27&&image.width==512&&image.height==512&&!image.mipmapped);++checks;
 View view{};assert(dh2_texture_open(actual_blood_atlas,sizeof actual_blood_atlas,&view)==Error::ok);std::vector<std::uint8_t> rgba(512*512*4);assert(dh2_texture_decode(&view,rgba.data(),rgba.size())==Error::ok);++checks;
 TextureDescriptorV1 d{};TextureManagerFieldsV1 manager;assert(texture_image_descriptor_v1(d,{image.engine_format,image.width,image.height,0},manager,&options.flags88(),e));assert(d.mipmapped&&d.usage==0&&d.layout==0);++checks;
 TextureParameterOwnerV1 owner;assert(owner.construct(d,e));assert(owner.fields().mip_count3e==10);assert(!owner.ready());assert(owner.set_data(true,e));++checks;
 TextureParameterServicesV1 params;unsigned param_calls=0;params.integer=[&](auto,auto,auto,std::string&){++param_calls;return true;};params.format_flags=[](auto format,std::uint32_t& flags,std::string&){assert(format==27);flags=8;return true;};params.compressed_filter_diagnostic=[](auto,std::string&){return true;};
 assert(owner.update(driver,params,e));assert(param_calls==5&&!owner.ready());assert(((owner.fields().parameters38>>12)&7)==0);++checks;
 std::array<std::uint32_t,11> offsets{};offsets[1]=std::uint32_t(view.payload_size);for(unsigned i=2;i<11;++i)offsets[i]=offsets[1];std::uint32_t pending=1,alignment=4;
 TextureUpload2dBorrowV1 data;data.bytes=view.payload;data.size=view.payload_size;data.offsets=offsets.data();data.offset_count=offsets.size();data.pending_bits=&pending;data.pending_word_count=1;data.internal_format=0x8c02;data.format_flags=8;data.row_bytes=256;data.driver_unpack_alignment26c=&alignment;data.driver_capabilities9c=&caps;
 TextureUpload2dServicesV1 upload;unsigned images=0,errors=0,diagnostics=0;upload.image=[&](auto target,auto level,auto w,auto h,auto,const auto* bytes,auto size,bool compressed,auto,auto,std::string&){assert(target==0xde1&&!level&&w==512&&h==512&&bytes==view.payload&&size==131072&&compressed);++images;return true;};upload.gl_error=[&](std::uint32_t& v,std::string&){v=0;++errors;return true;};upload.compressed_mipmap_diagnostic=[&](std::string&){++diagnostics;return true;};
 // fields() intentionally invalidates parameter receipt: perform source update
 // again before source updateData, rather than inject an upload-ready flag.
 assert(owner.update(driver,params,e));assert(owner.upload(data,upload,e));assert(owner.ready()&&images==1&&errors==2&&diagnostics==1&&!pending);++checks;
 owner.context_lost();assert(!owner.ready());++checks;
 std::printf("PASS %u texture-owner checks; 192 ARM oracle table counterparts + actual blood cache decode + ordered callback receipts (GPU not executed)\n",checks);
}
