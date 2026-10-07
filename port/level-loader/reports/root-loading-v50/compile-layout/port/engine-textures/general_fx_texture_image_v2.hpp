#pragma once
#include "blood_texture_image_v1.hpp"
namespace dh2::textures {
// Modern GLES image storage from actual View/Open/Decode. Keeps source image
// identity/metadata and the existing original sampler/upload lifecycle; it
// does not emulate the old GPU compressed-format choice.
class GeneralFxTextureImageOwnerV2 {
 std::shared_ptr<const std::vector<std::uint8_t>> source_;
 std::vector<std::uint8_t> decoded_;
 std::vector<std::uint32_t> offsets_;
 std::uint32_t pending_{};bool initialized_{};
 std::uint32_t source_format_{};Format decoded_source_kind_{};
 TextureDescriptorV1 descriptor_{};Gles2TextureSamplerFeaturesV1 features_{};
 TextureParameterOwnerV1 parameters_;
public:
 bool initialize(std::shared_ptr<const std::vector<std::uint8_t>>,
  const Gles2TextureSamplerFeaturesV1&,const TextureManagerFieldsV1&,
  const std::uint32_t* same_driver_options88,
  const std::function<bool(std::uint32_t,std::uint32_t,std::string&)>& conversion_diagnostic,
  std::string&);
 TextureParameterOwnerV1& parameters()noexcept{return parameters_;}
 const TextureDescriptorV1& descriptor()const noexcept{return descriptor_;}
 std::uint32_t source_format()const noexcept{return source_format_;}
 Format source_kind()const noexcept{return decoded_source_kind_;}
 const std::vector<std::uint8_t>& rgba()const noexcept{return decoded_;}
 TextureDriverBorrowV1 driver()const noexcept{return {&features_.capabilities9c,&features_.parameter_flags7ec,&features_.maximum_anisotropy4a8};}
 bool upload_borrow(std::uint32_t* same_driver_unpack_alignment26c,TextureUpload2dBorrowV1&,std::string&);
};
bool general_fx_texture_format_flags_v2(std::uint32_t,std::uint32_t&,std::string&);
}
