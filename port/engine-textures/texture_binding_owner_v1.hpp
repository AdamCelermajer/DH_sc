#pragma once
#include <array>
#include "texture_owner_v1.hpp"
namespace dh2::textures {
struct TextureBindingReceiverV1 {
 const void* identity{};
 const std::uint8_t* flags3f{};
 const std::uint16_t* dirty40{};
 const std::uint32_t* gl_name54{};
};
struct TextureBindingDriverBorrowV1 {
 const std::uint32_t* texture_units4c{};
 std::uint32_t* active_unit268{};
};
struct TextureBindingServicesV1 {
 std::function<bool(std::uint32_t,std::string&)> active_texture;
 std::function<bool(std::uint32_t,std::uint32_t,std::string&)> bind_texture;
 // Actual CTexture.update(false) successor. The proven clean leaf below
 // completes its genuine no-GL-call branch after reading SAME dirty40.
 std::function<bool(const TextureBindingReceiverV1&,bool,std::string&)> update;
};
struct TextureInitialBindServicesV1 {
 std::function<bool(std::uint32_t&,std::string&)> generate_texture;
 std::function<bool(std::uint32_t,std::string&)> active_texture;
 std::function<bool(std::uint32_t,std::uint32_t,std::string&)> bind_texture;
};
bool texture_clean_update_false_v1(const TextureBindingReceiverV1&,bool,std::string&);
// Source driver4×8 borrowed receiver grid ctor6de7f0 (memset42080), source
// texture-change counter84 ctor5ab044=0. No refcount is added by setTexture.
class TextureBindingOwnerV1 {
 std::array<TextureBindingReceiverV1,32> slots_{};
 std::uint32_t changes84_{};
public:
 // Genuine fresh CTexture.bindImpl prefix for source format14/27 and their
 // constructor sampler. Direct sourcegrid publication does NOT increment84.
 // Success reaches exactly BEFORE CTexture.update(true): caller must deliver
 // SAME parameters/updateData and the reached upload-error/unbind tail.
 bool initial_bind_impl_prefix(const TextureBindingReceiverV1&,
  TextureSamplerFieldsV1& same_fields,std::uint32_t& same_gl_name54,
  const TextureBindingDriverBorrowV1&,const TextureInitialBindServicesV1&,
  std::string&);
 bool set_texture(std::uint32_t unit,const TextureBindingReceiverV1&,
  std::uint32_t kind,const TextureBindingDriverBorrowV1&,
  const TextureBindingServicesV1&,std::string&);
 const TextureBindingReceiverV1* slot(std::uint32_t kind,std::uint32_t unit)const noexcept{return kind<4&&unit<8?&slots_[kind*8+unit]:nullptr;}
 const std::uint32_t& texture_changes84()const noexcept{return changes84_;}
 // Source grid is non-owning. Destroy consumers only after clearing their
 // source slots; whole ITexture unbind/destruction is a separate continuation.
};
}
