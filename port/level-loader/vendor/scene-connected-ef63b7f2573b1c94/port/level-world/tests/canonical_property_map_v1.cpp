#include "../canonical_property_map_v1.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
struct Fields {std::map<std::uint32_t,CanonicalPropertyValueV1> values;std::vector<std::uint32_t> writes;std::uint32_t fail{};};
static bool read(void* p,std::uint32_t off,std::uint8_t& value,std::string&){auto& f=*static_cast<Fields*>(p);if(off!=0x84)return false;value=std::get<std::uint8_t>(f.values.at(off));return true;}
template<class T> static bool write(void* p,std::uint32_t off,T value,std::string& e){auto& f=*static_cast<Fields*>(p);f.writes.push_back(off);if(f.fail==off){e="fixture writer unavailable";return false;}f.values[off]=std::move(value);return true;}
static CanonicalPropertyFieldServicesV1 services(Fields& f){return {&f,read,write<std::uint8_t>,write<std::int32_t>,write<float>,write<const std::string&>,write<const std::array<float,3>&>,write<const std::array<std::int32_t,2>&>};}
static bool assertion(void* p,std::string&){++*static_cast<unsigned*>(p);return true;}
struct XML {std::map<std::string,std::string> attrs;};
static const char* attribute(void* p,std::uint32_t,const char* key){auto& attrs=static_cast<XML*>(p)->attrs;auto i=attrs.find(key);return i==attrs.end()?nullptr:i->second.c_str();}
int main(){
 unsigned asserts=0;CanonicalPropertyMapV1 map({&asserts,&canonical_vec3_origin_v1(),assertion});std::string e;
 Fields a;a.values[0x84]=std::uint8_t{0};std::string template_a;CanonicalPropertyActorV1 actor{"Character",&template_a,services(a)};
 assert(map.init_properties(actor,e)&&map.class_count()==1&&a.writes.empty());assert(map.load_defaults(actor,e));
 assert(std::get<std::uint8_t>(a.values.at(0x80))==1&&std::get<std::uint8_t>(a.values.at(0x87))==0);
 assert(std::get<std::int32_t>(a.values.at(0x274))==100&&std::get<float>(a.values.at(0x143c))==0.f);
 assert((std::get<std::array<std::int32_t,2>>(a.values.at(0x1434))==std::array<std::int32_t,2>{{0,0}}));
 assert(map.set_property(actor,"visible","true",e)&&std::get<std::uint8_t>(a.values.at(0x80))==0); // atoi semantics, not word true
 assert(map.set_property(actor,"visible","-2",e)&&std::get<std::uint8_t>(a.values.at(0x80))==1);
 assert(map.set_property(actor,"position",",1,,2,3,4",e));assert((std::get<std::array<float,3>>(a.values.at(0x160))==std::array<float,3>{{1,2,3}}));
 assert(map.set_property(actor,"spawn_delay","-8,,12,13",e));assert((std::get<std::array<std::int32_t,2>>(a.values.at(0x1434))==std::array<std::int32_t,2>{{-8,12}}));
 assert(map.set_property(actor,"name","skeleton01",e)&&std::get<std::string>(a.values.at(0x30))=="skeleton01");
 assert(map.set_property(actor,"name",nullptr,e)&&std::get<std::string>(a.values.at(0x30)).empty());
 assert(map.set_template(actor,"named",e)&&template_a=="named"&&asserts==1);assert(map.set_template(actor,"",e)&&template_a=="named"&&asserts==1);
 assert(map.set_template_parameter(actor,"visible","0",e));assert(map.load_defaults(actor,e)&&template_a=="named"&&std::get<std::uint8_t>(a.values.at(0x80))==0);
 auto xml=std::make_shared<XML>();xml->attrs={{"name","ghost02"},{"gametype","Character"},{"position","4,5,6"},{"spawn_delay","3,7"}};
 CanonicalSourceObjectRequestV1 request;request.source_lease=xml;request.source_context=xml.get();request.attribute=attribute;
 assert(map.load_overrides(actor,request,e)&&template_a.empty());assert(std::get<std::string>(a.values.at(0x30))=="ghost02"&&std::get<std::uint8_t>(a.values.at(0x80))==1); // cached named iteration, NULL _templateName switches lookup to base map
 Fields b;b.values[0x84]=std::uint8_t{1};std::string template_b;CanonicalPropertyActorV1 next{"Character",&template_b,services(b)};
 assert(map.init_properties(next,e)&&map.load_defaults(next,e)&&std::get<std::uint8_t>(b.values.at(0x84))==0); // same class descriptor defaults from FIRST genuine constructor
 Fields chest;chest.values[0x84]=std::uint8_t{0};std::string template_c;CanonicalPropertyActorV1 c{"OpenableContainer",&template_c,services(chest)};
 assert(map.init_properties(c,e)&&map.load_defaults(c,e));assert(std::get<std::int32_t>(chest.values.at(0x374))==-1&&std::get<std::int32_t>(chest.values.at(0x708))==1&&std::get<std::uint8_t>(chest.values.at(0x70c))==1);
 a.fail=0x143c;assert(!map.load_defaults(actor,e)&&!a.writes.empty());a.fail=0;e.clear();
 std::string oversized(256,'1');assert(!map.set_property(actor,"position",oversized.c_str(),e));assert((std::get<std::array<float,3>>(a.values.at(0x160))==std::array<float,3>{{0,0,0}}));
 std::cout<<"canonical PropertyMap PASS shared schema/defaults/XML/typed prefix fixtures; production receivers external\n";
}
