#include "texture_unbind_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::textures;
int main(){unsigned checks=0;
 for(bool automatic:{false,true})for(unsigned mips:{1u,10u,32u,33u})for(unsigned dirty:{0u,0x1ffdu}){
  TextureBindingOwnerV1 grid;TextureSamplerFieldsV1 f;f.parameters38=14<<4;f.mip_count3e=mips;f.flags3f=24|(automatic?2:0);f.dirty40=dirty;
  std::uint32_t name=77,units=4,active=0,pending[2]{};int identity=0;
  TextureBindingReceiverV1 r{&identity,&f.flags3f,&f.dirty40,&name};TextureBindingDriverBorrowV1 driver{&units,&active};TextureBindingServicesV1 binding;
  binding.active_texture=[](auto,std::string&){return true;};binding.bind_texture=[](auto,auto,std::string&){return true;};binding.update=[](auto,bool,std::string&){return true;};std::string error;
  assert(grid.set_texture(0,r,0,driver,binding,error));assert(grid.set_texture(3,r,0,driver,binding,error));const auto changes=grid.texture_changes84();
  TextureUnbindServicesV1 s;s.delete_texture=[&](auto value,std::string&){assert(value==77);assert(!grid.slot(0,0)->identity&&!grid.slot(0,3)->identity);++checks;return true;};
  assert(texture_unbind_2d_v1(grid,r,f,name,driver,binding,pending,2,s,true,error));
  assert(!name&&f.flags3f==((automatic?2:0)|16)&&f.dirty40==((dirty&~2)|0x1ffd)&&grid.texture_changes84()==changes);
  assert(pending[0]==(automatic?1u:0xffffffffu));assert(pending[1]==(mips>32&&!automatic?0xffffffffu:0u));++checks;
 }std::cout<<"PASS "<<checks<<" native unbind checks\n";
}
