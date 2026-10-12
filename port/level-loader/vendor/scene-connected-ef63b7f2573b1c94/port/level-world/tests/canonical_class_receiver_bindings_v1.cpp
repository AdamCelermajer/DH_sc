#include "../canonical_class_receiver_bindings_v1.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
// Explicit receiver fixtures test dispatch/lifetime, not production constructors.
struct Receiver {
 const char* name{};std::string templ;
 dh2::target_providers::Handle16 handle{0,UINT32_MAX,0};
 std::map<std::uint32_t,CanonicalPropertyValueV1> fields;
 static bool read(void*,std::uint32_t o,std::uint8_t& v,std::string&){if(o!=0x84)return false;v=0;return true;}
 template<class T> static bool write(void* p,std::uint32_t o,T v,std::string&){static_cast<Receiver*>(p)->fields[o]=v;return true;}
 CanonicalPropertyActorV1 properties(){return {name,&templ,{this,read,write<std::uint8_t>,write<std::int32_t>,write<float>,write<const std::string&>,write<const std::array<float,3>&>,write<const std::array<std::int32_t,2>&>}};}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> p){CanonicalObjectBorrowV1 b;b.identity=reinterpret_cast<std::uintptr_t>(this);b.lease=p;b.shared_handle=&handle;b.class_name20=&name;return b;}
};
struct Construction {std::weak_ptr<Receiver> last;bool fail{};};
static bool make(void* p,const CanonicalSourceObjectRequestV1&,CanonicalClassReceiverV1& out,std::string& e){auto& c=*static_cast<Construction*>(p);auto r=std::make_shared<Receiver>();c.last=r;out=canonical_class_receiver_v1(r);if(c.fail){e="fixture constructor continuation failure";return false;}return true;}
int main(){
 CanonicalPropertyMapV1 map({nullptr,&canonical_vec3_origin_v1(),nullptr});Construction c;
 CanonicalClassReceiverBindingsV1 owner(map,{&c,make,make,nullptr,nullptr});auto s=owner.services();std::string e;
 CanonicalSourceObjectRequestV1 q;CanonicalObjectBorrowV1 a;
 assert(s.construct(s.context,{"Character",0},q,a,e));assert(owner.retained_count()==1&&!c.last.expired());
 *a.class_name20="Character";assert(s.init_properties(s.context,a,e));assert(s.load_defaults(s.context,a,e));
 auto actual=std::static_pointer_cast<Receiver>(a.lease);assert(std::get<std::uint8_t>(actual->fields.at(0x80))==1);
 assert(!s.init_post(s.context,a,e)&&e=="required actual class InitPost receiver");
 CanonicalObjectBorrowV1 old=a; // duplicate-name manager result dispatches same receiver
 assert(s.load_defaults(s.context,old,e)&&owner.retained_count()==1);
 c.fail=true;CanonicalObjectBorrowV1 failed;assert(!s.construct(s.context,{"OpenableContainer",0},q,failed,e));assert(failed.identity&&owner.retained_count()==2);
 assert(!s.construct(s.context,{"AnimatedDecor",0},q,failed,e));assert(e.find("required actual registered class construction")!=std::string::npos);
 owner.erased(a.identity);assert(owner.retained_count()==1);assert(!s.load_defaults(s.context,old,e));
 std::cout<<"canonical class receiver bindings PASS: explicit fixture lifetime, shared schema, prefix failure and missing lifecycle\n";
}
