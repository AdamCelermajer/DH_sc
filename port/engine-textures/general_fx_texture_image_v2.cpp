#include "general_fx_texture_image_v2.hpp"
#include <algorithm>
#include <limits>
namespace dh2::textures {
bool general_fx_texture_format_flags_v2(std::uint32_t format,std::uint32_t& out,std::string& error){if(format==14){out=1;return true;}error="Required decoded RGBA8 texture format-property row";return false;}
bool GeneralFxTextureImageOwnerV2::initialize(std::shared_ptr<const std::vector<std::uint8_t>> image,
 const Gles2TextureSamplerFeaturesV1& features,const TextureManagerFieldsV1& manager,const std::uint32_t* options,
 const std::function<bool(std::uint32_t,std::uint32_t,std::string&)>& diagnostic,std::string& error){
 if(initialized_){error="General FX image already initialized";return false;}
 if(!image){error="Required actual retained FX texture bytes";return false;}
 View view{};const auto opened=dh2_texture_open(image->data(),image->size(),&view);
 if(opened!=Error::ok){error="Actual FX image decoder: "+std::string(dh2_texture_error(opened));return false;}
 Description original{};const bool pvr=dh2_pvr_describe(image->data(),image->size(),&original);
 // Open validates its supported complete two-dimensional image domain.
 // Existing mipped/cube PVR decoders remain explicit required boundaries.
 if(pvr&&(original.kind||original.mipmapped)){error="Required authored cube/mip image decoder";return false;}
 const auto source_format=pvr?original.engine_format:(view.format==Format::tga_bgr24?13u:14u);
 const std::uint64_t size=std::uint64_t(view.width)*view.height*4;
 if(size>std::numeric_limits<std::uint32_t>::max()){error="Actual FX RGBA byte range";return false;}
 std::vector<std::uint8_t> decoded(std::size_t(size),0);
 const auto status=dh2_texture_decode(&view,decoded.data(),decoded.size());
 if(status!=Error::ok){error="Actual FX pixel decode: "+std::string(dh2_texture_error(status));return false;}
 TextureDescriptorV1 descriptor{};
 // The native decoded image IS RGBA8, including actual255 alpha for authored
 // RGB sources. Source mip manager/options continue to choose sampler/mips.
 if(!texture_image_descriptor_v1(descriptor,{14,view.width,view.height,0},manager,options,error))return false;
 if(source_format!=14&&(!diagnostic||!diagnostic(source_format,14,error))){if(error.empty())error="Required actual FX image conversion diagnostic";return false;}
 if(!parameters_.construct(descriptor,error))return false;
 const auto count=static_cast<const TextureParameterOwnerV1&>(parameters_).fields().mip_count3e;
 offsets_.reserve(count+1);offsets_.push_back(0);
 for(unsigned mip=0;mip<count;++mip){const auto w=std::max(1u,view.width>>mip),h=std::max(1u,view.height>>mip);offsets_.push_back(offsets_.back()+w*h*4);}
 // Actual assets opened here have base data only. If source driver requests
 // mips, the existing real generateMipmaps continuation produces the chain.
 if(!parameters_.set_data(true,error))return false;
 source_=std::move(image);decoded_=std::move(decoded);source_format_=source_format;decoded_source_kind_=view.format;
 features_=features;descriptor_=descriptor;pending_=1;initialized_=true;return true;
}
bool GeneralFxTextureImageOwnerV2::upload_borrow(std::uint32_t* alignment,TextureUpload2dBorrowV1& out,std::string& error){
 if(!initialized_||!alignment){error="Required initialized FX image/SAME driver alignment";return false;}
 TextureUpload2dBorrowV1 payload{};payload.bytes=decoded_.data();payload.size=decoded_.size();payload.offsets=offsets_.data();payload.offset_count=offsets_.size();payload.pending_bits=&pending_;payload.pending_word_count=1;payload.driver_unpack_alignment26c=alignment;payload.driver_capabilities9c=&features_.capabilities9c;
 payload.format_flags=1;payload.row_bytes=descriptor_.width*4;payload.internal_format=0x1908;payload.pixel_format=0x1908;payload.pixel_type=0x1401;out=payload;return true;
}
}
