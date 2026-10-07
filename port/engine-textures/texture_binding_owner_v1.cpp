#include "texture_binding_owner_v1.hpp"
namespace dh2::textures {
bool TextureBindingOwnerV1::initial_bind_impl_prefix(const TextureBindingReceiverV1& r,TextureSamplerFieldsV1& f,std::uint32_t& name,const TextureBindingDriverBorrowV1& d,const TextureInitialBindServicesV1& cb,std::string& e){
 if(!r.identity||r.flags3f!=&f.flags3f||r.dirty40!=&f.dirty40||r.gl_name54!=&name||name||!d.texture_units4c||!*d.texture_units4c||*d.texture_units4c>8||!d.active_unit268||*d.active_unit268>=*d.texture_units4c){e="Required fresh SAME CTexture bindImpl fields/driver/grid";return false;}
 const auto kind=f.parameters38&3;
 // Original5b5728/2c executes this prefix BEFORE actual GL generation.
 f.flags3f&=~0x10;
 if(!cb.generate_texture){e="Required actual glGenTextures";return false;}
 if(!cb.generate_texture(name,e))return false;
 if(!name){f.flags3f|=0x10;e="Source glGenTextures returned name0/error10";return false;}
 auto unit=*d.active_unit268;
 if(slots_[kind*8+unit].identity!=r.identity){
  unit=*d.texture_units4c-1;
  if(unit!=*d.active_unit268){if(!cb.active_texture){e="Required actual initial glActiveTexture";return false;}if(!cb.active_texture(0x84c0+unit,e))return false;*d.active_unit268=unit;}
  slots_[kind*8+unit]=r;
 }
 static constexpr std::uint32_t targets[]{0xde1,0x806f,0x8513,0x84f5};
 if(!cb.bind_texture){e="Required actual initial glBindTexture";return false;}
 if(!cb.bind_texture(targets[kind],name,e))return false;
 const auto format=(f.parameters38>>4)&63,min=(f.parameters38>>12)&7;
 if(f.mip_count3e>1){
  if(!(f.flags3f&2)){e="Required source initial explicit-mips bindImpl filter continuation";return false;}
  if(format!=27&&(format!=14||min<=1)){e="Required source initial bindImpl format/filter continuation";return false;}
 }
 // Source5b5820 compressed/one-mip branch or5b57c4 autoRGBA branch. Source
 // online flag is visible to SAME update(true)/mipmap callbacks immediately.
 f.flags3f|=8;return true;
}
bool texture_clean_update_false_v1(const TextureBindingReceiverV1& r,bool creation,std::string& e){
 if(creation||!r.identity||!r.dirty40){e="Required SAME texture for source CTexture.update(false)";return false;}
 if(*r.dirty40&0x1ffd){e="Required dirty source texture updateParameters/updateData continuation";return false;}
 return true;
}
bool TextureBindingOwnerV1::set_texture(std::uint32_t unit,const TextureBindingReceiverV1& r,std::uint32_t kind,const TextureBindingDriverBorrowV1& d,const TextureBindingServicesV1& cb,std::string& e){
 if(!d.texture_units4c){e="Required actual source driver texture_units4c";return false;}
 if(unit>=*d.texture_units4c){e="Source setTexture unit outside actual driver texture units";return false;}
 if(kind>=4||unit>=8){e="Unsupported source setTexture grid index";return false;}
 auto& slot=slots_[kind*8+unit];
 auto activate=[&](){if(!d.active_unit268){e="Required SAME source driver active_unit268";return false;}if(unit!=*d.active_unit268){if(!cb.active_texture){e="Required actual glActiveTexture";return false;}if(!cb.active_texture(0x84c0+unit,e))return false;*d.active_unit268=unit;}return true;};
 auto update=[&](const TextureBindingReceiverV1& actor){if(!cb.update){e="Required SAME source CTexture.update(false)";return false;}return cb.update(actor,false,e);};
 if(slot.identity==r.identity){
  if(!slot.identity)return true;
  if(slot.flags3f!=r.flags3f||slot.dirty40!=r.dirty40||slot.gl_name54!=r.gl_name54){e="Changed source texture field authority for same identity";return false;}
  if(!slot.dirty40){e="Required SAME source texture dirty40";return false;}
  if(!(*slot.dirty40&0x1ffd))return true;
  if(!activate())return false;
  return update(slot);
 }
 // Source publishes slot BEFORE online check and retains this prefix on a
 // missing reached receiver. The old receiver is not released/refcounted.
 slot=r;
 if(!r.identity)return true;
 ++changes84_;
 if(!activate())return false;
 if(!r.flags3f){e="Required SAME source texture flags3f";return false;}
 if(!(*r.flags3f&8)){e="Required original ITexture.bind(false)/online creation continuation";return false;}
 if(!r.gl_name54){e="Required source online texture GL name54";return false;}
 static constexpr std::uint32_t targets[]{0xde1,0x806f,0x8513,0x84f5};
 if(!cb.bind_texture){e="Required actual glBindTexture delivery";return false;}
 if(!cb.bind_texture(targets[kind],*r.gl_name54,e))return false;
 return update(r);
}
}
