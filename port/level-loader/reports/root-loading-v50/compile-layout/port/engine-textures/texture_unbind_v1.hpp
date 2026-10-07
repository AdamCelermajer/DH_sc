#pragma once
#include "texture_binding_owner_v1.hpp"
namespace dh2::textures {
struct TextureUnbindServicesV1 {
 std::function<bool(std::uint32_t,std::string&)> delete_texture;
};
// Whole 5b28dc for retained 2D data. Same pending storage as updateData.
bool texture_unbind_2d_v1(TextureBindingOwnerV1&,const TextureBindingReceiverV1&,
 TextureSamplerFieldsV1&,std::uint32_t& name54,const TextureBindingDriverBorrowV1&,
 const TextureBindingServicesV1&,std::uint32_t* pending,std::size_t pending_words,
 const TextureUnbindServicesV1&,bool upload_error,std::string&);
}
