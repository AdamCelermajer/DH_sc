#include "../condition_data_init_v3.hpp"
#include <cassert>
#include <iostream>
int main(){using namespace dh2;actor::RuntimeState runtime{};auto pin=std::make_shared<int>(1);world::CanonicalGameObjectBaseOwnerV1 base(reinterpret_cast<std::uintptr_t>(&runtime),11,pin,runtime);std::string e;unsigned checks=0;
for(auto offset:{0x8cu,0xb0u}){
 assert(base.pointer(offset+0x1c)&&*base.pointer(offset+0x1c)==0&&base.byte(offset+0x20)&&*base.byte(offset+0x20)==0);++checks;
 for(auto name:{"","Invalid"}){*base.string(offset+4)=name;*base.pointer(offset+0x1c)=0x11223344;*base.byte(offset+0x20)=85;assert(world::condition_data_init_v3(base,offset,{},e));assert(*base.pointer(offset+0x1c)==0x11223344&&*base.byte(offset+0x20)==85);++checks;}
 *base.string(offset+4)="Known";assert(!world::condition_data_init_v3(base,offset,{},e)&&!e.empty());assert(*base.pointer(offset+0x1c)==0x11223344);++checks;
 std::vector<world::ConditionDataRowV3> rows{{"Known",4,8}};world::ConditionDataInitServicesV3 services;services.owner=pin;services.conditions=&rows;unsigned constructed=0,initialized=0,destroyed=0;
 services.construct_condition=[&](std::uintptr_t& v,std::string&){++constructed;v=123;return true;};
 assert(!world::condition_data_init_v3(base,offset,services,e));assert(*base.pointer(offset+0x1c)==123&&constructed==1);++checks;
 services.initialize_condition=[&](std::uintptr_t v,std::uintptr_t a8,std::uintptr_t a4,std::string&){assert(v==123&&a8==8&&a4==4&&*base.pointer(offset+0x1c)==123);++initialized;return true;};
 assert(world::condition_data_init_v3(base,offset,services,e)&&initialized==1&&constructed==2);++checks;
 *base.string(offset+4)="Missing";assert(world::condition_data_init_v3(base,offset,services,e)&&*base.pointer(offset+0x1c)==123&&initialized==1);++checks;
 assert(!world::condition_data_clear_v3(base,offset,{},e)&&*base.pointer(offset+0x1c)==123);++checks;
 services.destroy_condition=[&](std::uintptr_t v,std::string&){assert(v==123&&*base.pointer(offset+0x1c)==123);++destroyed;return true;};assert(world::condition_data_clear_v3(base,offset,services,e)&&*base.pointer(offset+0x1c)==0&&destroyed==1&&*base.byte(offset+0x20)==85);++checks;
 assert(world::condition_data_clear_v3(base,offset,{},e));++checks;
}std::cout<<"ConditionData original ctor/Init/Clear same-field and failure-prefix PASS "<<checks<<" checks\n";}
