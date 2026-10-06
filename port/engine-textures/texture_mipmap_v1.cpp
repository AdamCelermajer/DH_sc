#include "texture_mipmap_v1.hpp"
namespace dh2::textures {
bool texture_generate_mipmaps_v1(TextureSamplerFieldsV1& s,const void* texture,const TextureMipmapDriverBorrowV1& d,const TextureMipmapServicesV1& cb,std::string& e){
 if(!texture||!d.texture_units4c||!*d.texture_units4c||!d.active_unit268||!cb.bind){e="Required source texture receiver/driver units/cache/bind";return false;}
 const auto unit=*d.texture_units4c-1,kind=s.parameters38&3;
 if(!cb.bind(unit,texture,kind,e))return false;
 if(unit!=*d.active_unit268){if(!cb.active_texture){e="Required actual glActiveTexture delivery";return false;}if(!cb.active_texture(0x84c0+unit,e))return false;*d.active_unit268=unit;}
 static constexpr std::uint32_t targets[]{0xde1,0x806f,0x8513,0x84f5},filters[]{0x2600,0x2601,0x2700,0x2701,0x2702,0x2703};
 const auto target=targets[kind],before=(s.parameters38>>12)&7;
 if(before>5){e="Unsupported source mipmap filter table index";return false;}
 if(before<=1){if(!cb.integer){e="Required source temporary mip filter delivery";return false;}if(!cb.integer(target,0x2801,0x2700,e))return false;}
 if(!cb.generate){e="Required actual glGenerateMipmap delivery";return false;}if(!cb.generate(target,e))return false;
 if(before<=1){const auto after=(s.parameters38>>12)&7;if(after>5){e="Unsupported changed source mipmap filter index";return false;}if(!cb.integer(target,0x2801,std::int32_t(filters[after]),e))return false;}
 if(!(s.flags3f&2))s.dirty40|=2;
 return true;
}
}
