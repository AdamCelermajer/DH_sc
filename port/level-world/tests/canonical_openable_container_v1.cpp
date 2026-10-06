#include "canonical_openable_container_v1.hpp"
#include <cassert>
#include <iostream>
int main(){
 using namespace dh2;using namespace dh2::world;
 actor::RuntimeState runtime{};auto pin=std::make_shared<int>(1);
 auto chest=std::make_shared<CanonicalOpenableContainerV1>(pin,runtime,OpenableContainerServicesV1{});
 auto borrow=chest->canonical(chest);std::string error;
 assert(borrow.identity==reinterpret_cast<std::uintptr_t>(chest.get()));
 assert(borrow.shared_handle==&chest->base().shared_handle());
 assert(*borrow.type_f4==7&&*borrow.room64==-1);
 std::uintptr_t character=123;assert(borrow.as_character(borrow.context,character,error)&&character==0);
 assert(&chest->base().runtime()==&runtime);
 assert(*chest->base().byte(0x28)==1&&*chest->base().byte(0xf8)==2);
 assert(chest->base().lifecycle().static84==0);
 for(std::size_t n=0;n<2;++n){auto& net=chest->network(n);
  assert(*chest->base().pointer(n?0x104:0x100)==reinterpret_cast<std::uintptr_t>(&net));
  assert(net.count()==3);for(std::size_t i=0;i<3;++i){auto* m=net.member(i);
   assert(m&&m->mask==(i==0?1u:i==1?16u:32u));
   assert(m->raw138==0&&m->raw140==-1&&m->raw144==-1&&m->value150==0);
  }assert(net.member(3)==nullptr);
  assert(*net.pointer(0x114)==reinterpret_cast<std::uintptr_t>(net.byte(0x10c)));
  assert(*net.pointer(0x118)==*net.pointer(0x114));
 }
 *borrow.class_name20="OpenableContainer";
 auto properties=chest->properties();assert(std::string(properties.class_name)=="OpenableContainer");
 auto& s=properties.fields;
 assert(s.write_string(s.context,0x378,"Swamp_Normal_Chest",error));
 assert(s.write_int(s.context,0x274,100,error));
 assert(s.write_bool(s.context,0x87,1,error));
 std::uint8_t across=0;assert(borrow.read_across_rooms87(borrow.context,across,error)&&across==1);
 assert(chest->fields().data_desc=="Swamp_Normal_Chest");
 assert(*chest->base().integer(0x274)==100);
 assert(!chest->receiver().init_post(error));
 assert(!error.empty()); // required real runtime services remain absent
 std::cout<<"Canonical chest SAME base/runtime/property/net-member graph PASS; missing runtime service retained\n";
}
