#pragma once
#include "texture_owner_v1.hpp"
#include <memory>
#include <vector>
namespace dh2::textures {
// Source projection ONLY of the feature bits read by the bounded sampler/
// upload owner, not a fabricated full IVideoDriver or historical GPU state.
struct Gles2TextureSamplerFeaturesV1 {
 std::uint32_t capabilities9c{},parameter_flags7ec{};
 float maximum_anisotropy4a8{};
 bool pvrtc414{},rgba8_392_or_437{};
};
bool texture_gles2_sampler_features_v1(const std::string& actual_extensions,
 const std::function<bool(std::uint32_t,float&,std::string&)>& actual_float_query,
 Gles2TextureSamplerFeaturesV1&,std::string&);
bool blood_texture_format_flags_v1(std::uint32_t,std::uint32_t&,std::string&);
class BloodTextureImageOwnerV1 {
 std::shared_ptr<const std::vector<std::uint8_t>> source_;
 std::vector<std::uint8_t> converted_;
 std::vector<std::uint32_t> offsets_;
 std::uint32_t pending_{};bool initialized_{};
 TextureDescriptorV1 descriptor_{};Gles2TextureSamplerFeaturesV1 features_{};
 TextureParameterOwnerV1 parameters_;
public:
 bool initialize(std::shared_ptr<const std::vector<std::uint8_t>> actual_image,
  const Gles2TextureSamplerFeaturesV1&,const TextureManagerFieldsV1&,
  const std::uint32_t* same_driver_options88,
  const std::function<bool(std::uint32_t,std::uint32_t,std::string&)>& source_format_change_diagnostic,
  std::string&);
 TextureParameterOwnerV1& parameters()noexcept{return parameters_;}
 const TextureDescriptorV1& descriptor()const noexcept{return descriptor_;}
 TextureDriverBorrowV1 driver()const noexcept{return {&features_.capabilities9c,&features_.parameter_flags7ec,&features_.maximum_anisotropy4a8};}
 // SAME actual driver unpack-alignment cache, not a per-resource substitute.
 bool upload_borrow(std::uint32_t* same_driver_unpack_alignment26c,TextureUpload2dBorrowV1&,std::string&);
};
}
