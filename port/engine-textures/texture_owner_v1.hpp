#pragma once
#include "textures.hpp"
#include <functional>
#include <string>
namespace dh2::textures {
struct TextureMipmapDriverBorrowV1;
struct TextureMipmapServicesV1;
// Logical source CImage metadata, borrowed from its actual decoder/retained
// payload. This is not an ARM object overlay or an inferred GPU descriptor.
struct ImageMetadataV1 {std::uint32_t format20,width10,height14;std::uint8_t mipmapped28;};
struct TextureManagerFieldsV1 {std::uint32_t flags74{0x43};}; // source C1/C2
struct TextureDescriptorV1 {
 std::uint32_t kind{},format{},layout{},usage{},width{1},height{1},depth{1};
 std::uint8_t mipmapped{},render_target{},reserved{};
};
// Whole createTextureFromImage descriptor prefix for source layout0. Texture
// creation/conversion/upload continuation is deliberately a separate owner.
bool texture_image_descriptor_v1(TextureDescriptorV1&,const ImageMetadataV1&,
 const TextureManagerFieldsV1&,const std::uint32_t* actual_driver_flags88,std::string&);
struct TextureSamplerFieldsV1 {
 std::uint32_t parameters38{};std::uint16_t dirty40{};
 std::uint8_t flags3f{},mip_count3e{};float anisotropy44{},lod_bias48{},max_lod50{};
};
// Actual driver fields remain borrows and are re-read after ordered delivery.
struct TextureDriverBorrowV1 {
 const std::uint32_t* capabilities9c{};const std::uint32_t* flags7ec{};
 const float* maximum_anisotropy4a8{};
};
struct TextureParameterServicesV1 {
 // All GL values are the exact source tables/enums. Callback must deliver to
 // this owner's actual bound texture; failure leaves dirty-prefix effects.
 std::function<bool(std::uint32_t,std::uint32_t,std::int32_t,std::string&)> integer;
 std::function<bool(std::uint32_t,std::uint32_t,float,std::string&)> scalar;
 // Source format-property flags table for the compressed/no-mips restriction.
 std::function<bool(std::uint32_t,std::uint32_t&,std::string&)> format_flags;
 std::function<bool(std::uint32_t,std::string&)> compressed_filter_diagnostic;
};
bool texture_sampler_constructor_v1(TextureSamplerFieldsV1&,const TextureDescriptorV1&,std::string&);
// Whole supported source updateParameters body. Undefined table indexes and
// unsafe float→int conversions reject explicitly; all normal source branches
// including the original duplicate wrap-T write are preserved.
bool texture_update_parameters_v1(TextureSamplerFieldsV1&,const TextureDriverBorrowV1&,
 const TextureParameterServicesV1&,std::string&);
struct TextureUpload2dBorrowV1 {
 const std::uint8_t* bytes{};std::size_t size{};
 // Actual source mip offsets and actual GL format mapping from the driver
 // format table. Neither pixel format nor compressed payload is inferred.
 const std::uint32_t* offsets{};std::size_t offset_count{};
 std::uint32_t* pending_bits{};std::size_t pending_word_count{};
 std::uint32_t internal_format{},pixel_format{},pixel_type{},format_flags{};
 std::uint32_t row_bytes{};std::uint32_t* driver_unpack_alignment26c{};
 const std::uint32_t* driver_capabilities9c{};
};
struct TextureUpload2dServicesV1 {
 std::function<bool(std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,
  std::uint32_t,const std::uint8_t*,std::size_t,bool,std::uint32_t,std::uint32_t,std::string&)> image;
 std::function<bool(std::uint32_t&,std::string&)> gl_error;
 std::function<bool(std::uint32_t,std::uint32_t,std::string&)> pixel_store;
 std::function<bool(std::string&)> compressed_mipmap_diagnostic;
 // Source virtual20; required only actual uncompressed auto-mips + cap4.
 std::function<bool(std::string&)> generate_mipmaps;
};
bool texture_upload_2d_v1(TextureSamplerFieldsV1&,const TextureDescriptorV1&,
 const TextureUpload2dBorrowV1&,const TextureUpload2dServicesV1&,std::string&);
class TextureParameterOwnerV1 {
 TextureDescriptorV1 descriptor_{};TextureSamplerFieldsV1 fields_{};
 bool constructed_{},uploaded_{},parameters_delivered_{},generation_delivery_{};
public:
 bool construct(const TextureDescriptorV1&,std::string&);
 // Source setData/invalidate base-data domain. Exact offsets/retained payload
 // are supplied at upload, and no readiness boolean may be injected.
 bool set_data(bool generate_mips,std::string&);
 bool update(const TextureDriverBorrowV1&,const TextureParameterServicesV1&,std::string&);
 bool upload(const TextureUpload2dBorrowV1&,const TextureUpload2dServicesV1&,std::string&);
 bool generate_mipmaps(const void* same_texture,const TextureMipmapDriverBorrowV1&,
  const TextureMipmapServicesV1&,std::string&);
 void context_lost()noexcept{uploaded_=parameters_delivered_=false;fields_.dirty40|=0x1ffd;}
 bool ready()const noexcept{return constructed_&&uploaded_&&parameters_delivered_&&!(fields_.dirty40&0x1ffc);}
 const TextureDescriptorV1& descriptor()const noexcept{return descriptor_;}
 TextureSamplerFieldsV1& fields()noexcept{parameters_delivered_=false;return fields_;}
 const TextureSamplerFieldsV1& fields()const noexcept{return fields_;}
 float lod_bias()const noexcept{return fields_.lod_bias48;}
};
}
