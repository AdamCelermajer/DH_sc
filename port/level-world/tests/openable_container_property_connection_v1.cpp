#include "openable_container_property_connection_v1.hpp"
#include <cassert>
#include <iostream>
int main(){
 using namespace dh2::world;OpenableContainerFieldsV1 fields;
 struct Base {std::array<float,3> position{};std::int32_t probability{};std::uint8_t visible{};} base;
 CanonicalPropertyFieldServicesV1 inherited;inherited.context=&base;
 inherited.write_bool=[](void* c,auto offset,auto value,auto&){assert(offset==0x81);static_cast<Base*>(c)->visible=value;return true;};
 inherited.write_int=[](void* c,auto offset,auto value,auto&){assert(offset==0x274);static_cast<Base*>(c)->probability=value;return true;};
 inherited.write_vector3=[](void* c,auto offset,auto const& value,auto&){assert(offset==0x160);static_cast<Base*>(c)->position=value;return true;};
 OpenableContainerPropertyConnectionV1 connection(fields,inherited,std::make_shared<int>(1));auto s=connection.services();std::string e;
 assert(s.write_int(s.context,0x374,-1,e));assert(s.write_string(s.context,0x378,"Swamp_Normal_Chest",e));
 assert(s.write_string(s.context,0x6f0,"actual_key",e));assert(s.write_int(s.context,0x708,4,e));
 assert(s.write_bool(s.context,0x390,1,e));assert(s.write_bool(s.context,0x70c,0,e));
 assert(fields.data374==-1&&fields.data_desc=="Swamp_Normal_Chest"&&fields.key_name=="actual_key"&&fields.key_qty==4&&fields.death_reset&&!fields.key_consume);
 assert(s.write_int(s.context,0x274,100,e));assert(s.write_bool(s.context,0x81,1,e));
 std::array<float,3> p{-2198.67f,935.187f,250.f};assert(s.write_vector3(s.context,0x160,p,e));
 assert(base.probability==100&&base.visible==1&&base.position==p);
 std::uint8_t b=0;assert(s.read_bool(s.context,0x390,b,e)&&b==1);assert(s.read_bool(s.context,0x70c,b,e)&&b==0);
 assert(!s.write_string(s.context,8,"missing_actual_base",e));assert(e.find("SAME GameObject")!=std::string::npos);
 std::cout<<"Chest class-specific properties and SAME inherited storage delegation PASS (declared base fixture)\n";
}
