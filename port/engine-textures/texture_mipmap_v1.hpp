#pragma once
#include "texture_owner_v1.hpp"
namespace dh2::textures {
struct TextureMipmapDriverBorrowV1 {
 const std::uint32_t* texture_units4c{};
 std::uint32_t* active_unit268{};
};
struct TextureMipmapServicesV1 {
 // Source bindTexture5b26f0: same actual texture receiver, last source unit.
 std::function<bool(std::uint32_t,const void*,std::uint32_t,std::string&)> bind;
 std::function<bool(std::uint32_t,std::string&)> active_texture;
 std::function<bool(std::uint32_t,std::string&)> generate;
 std::function<bool(std::uint32_t,std::uint32_t,std::int32_t,std::string&)> integer;
};
// Whole CTexture::generateMipmapsImpl5b280c supported defined-index body.
bool texture_generate_mipmaps_v1(TextureSamplerFieldsV1&,const void* same_texture,
 const TextureMipmapDriverBorrowV1&,const TextureMipmapServicesV1&,std::string&);
// CommonGL's retained cache member C2 at6dcd90, owner offset254:
// +14 = active unit268 =0; +18=unpack26c=4.
struct TextureGlCacheFieldsV1 {std::uint32_t active_unit268{},unpack_alignment26c{4};};
}
