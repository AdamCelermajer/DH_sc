#include "texture_unbind_v1.hpp"
namespace dh2::textures {
bool texture_unbind_2d_v1(TextureBindingOwnerV1& grid,const TextureBindingReceiverV1& r,
 TextureSamplerFieldsV1& f,std::uint32_t& name,const TextureBindingDriverBorrowV1& d,
 const TextureBindingServicesV1& binding,std::uint32_t* pending,std::size_t words,
 const TextureUnbindServicesV1& cb,bool upload_error,std::string& e){
 if(!r.identity||r.flags3f!=&f.flags3f||r.dirty40!=&f.dirty40||r.gl_name54!=&name||
    (f.parameters38&3)||!d.texture_units4c||*d.texture_units4c>8){e="Required SAME 2D texture unbind fields/grid";return false;}
 for(std::uint32_t unit=0;unit<*d.texture_units4c;++unit)
  if(grid.slot(0,unit)->identity==r.identity&&!grid.set_texture(unit,{},0,d,binding,e))return false;
 if(!cb.delete_texture){e="Required actual glDeleteTextures";return false;}
 if(!cb.delete_texture(name,e))return false;
 f.dirty40=std::uint16_t((f.dirty40&~2u)|0x1ffc);f.flags3f&=0xe7;name=0;
 f.dirty40|=1;
 const auto needed=(std::size_t(f.mip_count3e)+31)/32;
 if(!pending||words<needed){e="Required SAME retained texture pending mip words";return false;}
 if(f.flags3f&2)pending[0]|=1;
 else for(std::size_t i=0;i<needed;++i)pending[i]=0xffffffffu;
 // bindImpl5b5864 invokes virtual10, then re-publishes error10.
 if(upload_error)f.flags3f|=0x10;
 return true;
}
}
