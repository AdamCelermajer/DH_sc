#include "canonical_light_point_v53.hpp"
#include "level_lightset_stage9_v53.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace dh2;
std::string bytes(const void* p,std::size_t n){std::ostringstream s;auto* b=static_cast<const unsigned char*>(p);for(std::size_t i=0;i<n;++i)s<<std::hex<<std::setw(2)<<std::setfill('0')<<unsigned(b[i]);return s.str();}
bool assertion(void*,std::string& e){e.clear();return true;}
int main(){std::string e;
 std::array<float,3> zero{};world::CanonicalPropertySourceServicesV1 ps{};ps.position_rotation_default=&zero;ps.load_template_assertion=assertion;
 world::CanonicalPropertyMapV1 map(ps);auto runtime=std::make_shared<int>(19);
 struct Scalar {std::array<float,3> attenuation,ambient,diffuse,specular;const char *a,*b,*c,*d;};
 Scalar scalars[]={{{0.265625f,0.0f,0.132813f},{255.0f,255.0f,255.0f},{255.0f,255.0f,255.0f},{255.0f,255.0f,255.0f},"0000883e000000005c9b0e34","0000803f0000803f0000803f","0000803f0000803f0000803f","0000803f0000803f0000803f"},
{{1.0f,1000.0f,1000000.0f},{0.0f,128.0f,255.0f},{-1.0f,12.0f,60.0f},{512.0f,32.0f,100.0f},"0000803f0000803f0000803f","000000008180003f0000803f","818080bbc1c0403df1f0703e","818000408180003ec9c8c83e"},
{{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},"000000000000000000000000","000000000000000000000000","000000000000000000000000","000000000000000000000000"},
{{3.0f,-2000.0f,-800000.0f},{1.0f,3.0f,7.0f},{17.0f,49.0f,99.0f},{100.0f,125.0f,250.0f},"00004040000000c0cdcc4cbf","8180803bc1c0403ce1e0e03c","8988883dc5c4443ec7c6c63e","c9c8c83efbfafa3efbfa7a3f"}};
 for(const auto& row:scalars){auto p=std::make_shared<world::CanonicalLightPointV53>(runtime,world::LightPointInitServicesV53{});auto canonical=p->canonical(p);assert(*canonical.type_f4==19);*canonical.class_name20="LightPoint";auto properties=p->properties();assert(map.init_properties(properties,e));assert(map.load_defaults(properties,e));auto& fields=properties.fields;
  assert(fields.write_vector3(fields.context,0x134,row.attenuation,e));assert(fields.write_vector3(fields.context,0x140,row.ambient,e));assert(fields.write_vector3(fields.context,0x14c,row.diffuse,e));assert(fields.write_vector3(fields.context,0x158,row.specular,e));
  assert(!p->init_post(e)&&e=="Required actual LightPoint renderer owner"&&!p->actual_node().identity&&!p->init_complete());
  assert(bytes(p->vector(0x134)->data(),12)==row.a);assert(bytes(p->vector(0x140)->data(),12)==row.b);assert(bytes(p->vector(0x14c)->data(),12)==row.c);assert(bytes(p->vector(0x158)->data(),12)==row.d);
  assert(!p->init_post(e));assert(bytes(p->vector(0x140)->data(),12)==row.b); // no repeated /255 after failed prefix
 }
 {auto p=std::make_shared<world::CanonicalLightPointV53>(runtime,world::LightPointInitServicesV53{});auto c=p->canonical(p);*c.class_name20="LightPoint";auto a=p->properties();assert(!p->floating(0x130)&&!p->byte(0x164));assert(map.init_properties(a,e)&&map.load_defaults(a,e));assert(*p->floating(0x130)==2&&*p->byte(0x164)==0);assert(map.set_property(a,"radius","3.5",e)&&map.set_property(a,"automatic","1",e)&&map.set_property(a,"attachedTo","Player",e)&&map.set_property(a,"attachedOffset","0,0,200",e));assert(*p->floating(0x130)==3.5&&*p->byte(0x164)==1&&*p->string(0x180)=="Player"&&(*p->vector(0x1a4))[2]==200);}
 struct Stage{bool fixed;const char *fixed_name,*regular_name;int polls,queries;};Stage stages[]={{false,"fixed","regular",1,1},
{true,"fixed","regular",1,1},
{true,"","regular",1,0},
{false,"fixed","",1,0},
{true,"fixed","",1,1},
{false,"","",1,0},
{false,"fixed","regular",3,3},
{true,"fixed","regular",3,3}};
 for(const auto& row:stages){auto owner=std::make_shared<int>(53);std::uintptr_t id=53;std::string fixed=row.fixed_name,regular=row.regular_name;int count=0;loader::Stage9ConfigBorrowV53 f{owner,owner,&id,53,&fixed,&regular};loader::Stage9ServicesV53 s;s.actual_device_owner=owner;s.is_fixed_pipeline=[&](bool& o,std::string& err){o=row.fixed;err.clear();return true;};s.actual_file_loader_owner=owner;s.validate_current=[](std::string& err){err.clear();return true;};s.load_module_file_step=[&](const std::string& name,const char* tag,std::string& err){assert(name==(row.fixed?fixed:regular)&&std::string(tag)=="Module");++count;err.clear();return count<row.polls?loader::LifecycleStepV36::pending:loader::LifecycleStepV36::complete;};loader::Stage9BodyV53 body(f,s);loader::LifecycleStepV36 result;do{result=body.step(e);}while(result==loader::LifecycleStepV36::pending);assert(result==loader::LifecycleStepV36::complete&&count==row.queries&&e.empty());assert(body.step(e)==loader::LifecycleStepV36::complete&&count==row.queries);id=54;assert(body.step(e)==loader::LifecycleStepV36::failed);}
 struct Sync {float radius;const char* expected;};Sync syncs[]={{-1.0f,"0000803f00000040000040400000803f000080400000a0400000c0400000803f0000e04000000041000010410000803f0000803e0000003f0000403f00088745"},
{0.0f,"0000803f00000040000040400000803f000080400000a0400000c0400000803f0000e04000000041000010410000803f0000803e0000003f0000403f00088745"},
{0.005f,"0000803f00000040000040400000803f000080400000a0400000c0400000803f0000e04000000041000010410000803f0000803e0000003f0000403f00088745"},
{0.01f,"0000803f00000040000040400000803f000080400000a0400000c0400000803f0000e04000000041000010410000803f0000803e0000003f0000403f00088745"},
{0.0101f,"0000803f00000040000040400000803f000080400000a0400000c0400000803f0000e04000000041000010410000803f0000803e0000003f0000403f787a253c"},
{2.0f,"0000803f00000040000040400000803f000080400000a0400000c0400000803f0000e04000000041000010410000803f0000803e0000003f0000403f00000040"},
{3.5f,"0000803f00000040000040400000803f000080400000a0400000c0400000803f0000e04000000041000010410000803f0000803e0000003f0000403f00006040"}};
 for(const auto& row:syncs){world::CanonicalLightPointV53 p(runtime,{});auto c=p.canonical(runtime);*c.class_name20="LightPoint";auto a=p.properties();assert(map.init_properties(a,e)&&map.load_defaults(a,e));auto& f=a.fields;assert(f.write_float(f.context,0x130,row.radius,e));assert(f.write_vector3(f.context,0x134,{.25f,.5f,.75f},e));assert(f.write_vector3(f.context,0x140,{1,2,3},e));assert(f.write_vector3(f.context,0x14c,{4,5,6},e));assert(f.write_vector3(f.context,0x158,{7,8,9},e));std::array<float,4> amb{},diff{},spec{};std::array<float,3> att{};float radius=4321;world::LightParameterFieldsV53 out{runtime,0x1234,&amb,&diff,&spec,&att,&radius,[](std::string& err){err.clear();return true;}};assert(p.sync_data(out,e));std::string joined=bytes(amb.data(),16)+bytes(diff.data(),16)+bytes(spec.data(),16)+bytes(att.data(),12)+bytes(&radius,4);assert(joined==row.expected&&e.empty());out.owner.reset();auto before=amb;assert(!p.sync_data(out,e)&&amb==before);}
 std::cout<<"PASS original8 stage9 alternatives/polls,4 bit-exact normalization cases,GO19 actual property map/defaults/overrides,7 original SyncData writes,missing/stale/replay guards; renderer/GPU activation0\n";
}
