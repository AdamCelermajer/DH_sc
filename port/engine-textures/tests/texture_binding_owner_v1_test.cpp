#include "../texture_binding_owner_v1.hpp"
#include "../texture_mipmap_v1.hpp"
#include <cassert>
#include <cstdio>
using namespace dh2::textures;
int main(){
 std::string e;unsigned checks=0;
 for(unsigned kind=0;kind<4;++kind)for(unsigned format:{14u,27u}){
  TextureBindingOwnerV1 binding;std::uint32_t units=4;TextureGlCacheFieldsV1 cache;
  TextureSamplerFieldsV1 fields{};fields.parameters38=kind|(format<<4)|(3<<12)|(1<<15);fields.flags3f=18;fields.mip_count3e=10;fields.dirty40=0x1ffd;std::uint32_t name=0;
  TextureBindingReceiverV1 receiver{&fields,&fields.flags3f,&fields.dirty40,&name};TextureBindingDriverBorrowV1 driver{&units,&cache.active_unit268};
  unsigned generated=0,active=0,bound=0;
  TextureInitialBindServicesV1 initial;
  initial.generate_texture=[&](std::uint32_t& out,std::string&){assert(!(fields.flags3f&16));out=77;++generated;return true;};
  initial.active_texture=[&](auto unit,std::string&){assert(unit==0x84c3);++active;return true;};
  initial.bind_texture=[&](auto target,auto value,std::string&){assert(target==std::uint32_t(kind==0?0xde1:kind==1?0x806f:kind==2?0x8513:0x84f5)&&value==77&&binding.slot(kind,3)->identity==&fields&&!binding.texture_changes84()&&!(fields.flags3f&8));++bound;return true;};
  assert(binding.initial_bind_impl_prefix(receiver,fields,name,driver,initial,e));assert(generated==1&&active==1&&bound==1&&fields.flags3f==10&&fields.dirty40==0x1ffd&&cache.active_unit268==3&&!binding.texture_changes84());++checks;
  fields.dirty40=0;TextureBindingServicesV1 services;services.update=texture_clean_update_false_v1;
  assert(binding.set_texture(3,receiver,kind,driver,services,e));assert(!binding.texture_changes84());++checks;
  TextureMipmapServicesV1 mip;unsigned generated_mips=0;
  mip.bind=[&](auto unit,const void* actor,auto type,std::string& error){assert(actor==&fields);return binding.set_texture(unit,receiver,type,driver,services,error);};
  mip.generate=[&](auto,std::string&){++generated_mips;return true;};
  assert(texture_generate_mipmaps_v1(fields,&fields,{&units,&cache.active_unit268},mip,e));assert(generated_mips==1&&!fields.dirty40&&!binding.texture_changes84());++checks;
  TextureBindingReceiverV1 empty;assert(binding.set_texture(3,empty,kind,driver,services,e));assert(!binding.slot(kind,3)->identity&&!binding.texture_changes84());++checks;
 }
 TextureBindingOwnerV1 failure;TextureSamplerFieldsV1 fields{};fields.flags3f=18;fields.parameters38=(14<<4)|(3<<12);fields.mip_count3e=10;std::uint32_t name=0,units=4;TextureGlCacheFieldsV1 cache;
 TextureBindingReceiverV1 r{&fields,&fields.flags3f,&fields.dirty40,&name};TextureInitialBindServicesV1 initial;initial.generate_texture=[](std::uint32_t& out,std::string&){out=0;return true;};
 assert(!failure.initial_bind_impl_prefix(r,fields,name,{&units,&cache.active_unit268},initial,e));assert(fields.flags3f==18&&!name&&!failure.slot(0,3)->identity&&!failure.texture_changes84());++checks;
 fields.flags3f=0;name=77;TextureBindingServicesV1 services;services.active_texture=[](auto,std::string&){return true;};services.update=texture_clean_update_false_v1;
 assert(!failure.set_texture(3,r,0,{&units,&cache.active_unit268},services,e));assert(failure.slot(0,3)->identity==&fields&&failure.texture_changes84()==1&&cache.active_unit268==3);++checks;
 std::printf("PASS %u source texture binding/initial-prefix/mipmap composition checks; required offline failure preserves source prefix (GPU not executed)\n",checks);
}
