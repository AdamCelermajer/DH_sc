#include "canonical_gameobject_base_owner_v1.hpp"
#include <cstring>
namespace dh2::world {
CanonicalGameObjectBaseOwnerV1::CanonicalGameObjectBaseOwnerV1(std::uintptr_t identity,std::uint32_t type,std::shared_ptr<void> pin,actor::RuntimeState& runtime):identity_(identity),world_pin_(std::move(pin)),runtime_(runtime),type_f4_(type){
 handle_={0,UINT32_MAX,identity};
 for(auto offset:{0x28u,0x29u,0x60u,0x86u,0x88u,0x89u,0xf0u,0xf1u,0xf8u,0x10cu,0x118u,0x119u,0x15cu,0x1b4u,0x1b5u,0x1c4u,0x2ecu,0x2efu,0x2f0u,0x2f8u,0x2f9u,0x2fau,0x2fcu,0x372u,0x373u})bytes_[offset]=0;
 bytes_[0x2ee]=1;bytes_[0x2fb]=1;
 for(auto offset:{0x30u,0x48u,0x68u,0x90u,0xb4u,0xd4u,0x254u,0x278u,0x290u,0x2a8u,0x2c0u,0x358u})strings_[offset]={};
 for(auto offset:{0xecu,0x108u,0x110u,0x270u})integers_[offset]=-1;
 integers_[0x274]=100;integers_[0x370]=-1;
 for(auto offset:{0xfcu,0x100u,0x104u,0x114u,0x11cu,0x180u,0x1b8u,0x1bcu,0x26cu,0x2d8u,0x2dcu,0x2e0u,0x2e4u,0x2e8u,0x300u})pointers_[offset]=0;
 std::memset(runtime_.subobjects.position,0,12);std::memset(runtime_.rotation.rotation,0,12);runtime_.rotation.heading_angle=0;
 for(unsigned i=0;i<6;++i)runtime_.subobjects.local_bounds[i]=i<3?-100.f:100.f;
  update_absolute_aabb();dh2_nav_object_defaults(&runtime_.object);
 // SAME source destination+1a8 used by SetDestination393600. The port's
 // whole SetPosition successor already borrows controller.destination;
 // target_position is the separate LookAt cached-node projection.
 // GameObjectC2 38c1fc..204 and C1 38c464..46c initialize this XYZ to zero.
 std::memset(runtime_.controller.destination,0,12);
 // ObjectBaseC2 calls ConditionDataC1 at+8c/+b0 (33f424/33f42c).
 // ConditionDataC1 stores compiled+1c=NULL and tested+20=0 (33edc4/c8).
 pointers_[0xa8]=0;pointers_[0xcc]=0;bytes_[0xac]=0;bytes_[0xd0]=0;
}
std::uint8_t* CanonicalGameObjectBaseOwnerV1::byte(std::uint32_t o)noexcept{
 switch(o){case 0x80:return lifecycle_.visible_written?&lifecycle_.visible80:nullptr;case 0x81:return &lifecycle_.disabled81;case 0x84:return &lifecycle_.static84;case 0x85:return &lifecycle_.updating85;case 0x8a:return &lifecycle_.enabled8a;case 0x2ed:return &lifecycle_.non_zonable2ed;case 0x2ee:return &lifecycle_.zoning2ee;case 0x2ef:return &lifecycle_.assigned2ef;case 0x2f0:return &lifecycle_.entered2f0;case 0x373:return &lifecycle_.disabled373;default:break;}
 auto i=bytes_.find(o);return i==bytes_.end()?nullptr:&i->second;
}
std::int32_t* CanonicalGameObjectBaseOwnerV1::integer(std::uint32_t o)noexcept{auto i=integers_.find(o);return i==integers_.end()?nullptr:&i->second;}
std::uintptr_t* CanonicalGameObjectBaseOwnerV1::pointer(std::uint32_t o)noexcept{if(o==0x2f4)return &lifecycle_.room2f4;auto i=pointers_.find(o);return i==pointers_.end()?nullptr:&i->second;}
std::string* CanonicalGameObjectBaseOwnerV1::string(std::uint32_t o)noexcept{if(o==8)return &template_name8_;auto i=strings_.find(o);return i==strings_.end()?nullptr:&i->second;}
 float* CanonicalGameObjectBaseOwnerV1::vector3(std::uint32_t o)noexcept{switch(o){case 0x120:return scale120_.data();case 0x12c:return absolute_aabb12c();case 0x138:return absolute_aabb12c()+3;case 0x144:return relative_aabb144();case 0x150:return relative_aabb144()+3;case 0x160:return runtime_.subobjects.position;case 0x16c:return runtime_.rotation.rotation;case 0x1a8:return runtime_.controller.destination;default:return nullptr;}}
float* CanonicalGameObjectBaseOwnerV1::scalar(std::uint32_t o)noexcept{if(o==0x178)return &runtime_.rotation.heading_angle;if(o>=0x144&&o<=0x158&&(o-0x144)%4==0)return relative_aabb144()+(o-0x144)/4;if(o>=0x12c&&o<=0x140&&(o-0x12c)%4==0)return absolute_aabb12c()+(o-0x12c)/4;return nullptr;}
void CanonicalGameObjectBaseOwnerV1::update_absolute_aabb()noexcept{for(unsigned i=0;i<6;++i)runtime_.subobjects.absolute_bounds[i]=runtime_.subobjects.local_bounds[i]+runtime_.subobjects.position[i%3];}
bool CanonicalGameObjectBaseOwnerV1::store_byte(std::uint32_t o,std::uint8_t v,std::string& e){return write_bool(this,o,v,e);}
void CanonicalGameObjectBaseOwnerV1::source_delete_v4()noexcept{bytes_[0x82]=2;lifecycle_.disabled81=1;}
bool CanonicalGameObjectBaseOwnerV1::source_online_owner_fc_v4(std::int32_t& out,std::string& e){
 const auto value=*pointer(0xfc);if(value>UINT32_MAX){e="source ownerfc exceeds original 32-bit field";return false;}
 const auto word=static_cast<std::uint32_t>(value);std::memcpy(&out,&word,4);return true;
}
bool CanonicalGameObjectBaseOwnerV1::set_name(void* p,const char* v,std::string& e){if(!v){e="source SetName requires CString";return false;}static_cast<CanonicalGameObjectBaseOwnerV1*>(p)->strings_[0x30]=v;return true;}
bool CanonicalGameObjectBaseOwnerV1::set_archetype(void* p,const char* v,std::string& e){if(!v){e="source archetype requires CString";return false;}static_cast<CanonicalGameObjectBaseOwnerV1*>(p)->strings_[0x48]=v;return true;}
bool CanonicalGameObjectBaseOwnerV1::as_character(void*,std::uintptr_t& v,std::string&){v=0;return true;} // original GameObject virtual IsCharacter0
bool CanonicalGameObjectBaseOwnerV1::read_across_rooms(void* p,std::uint8_t& v,std::string& e){return read_bool(p,0x87,v,e);}
bool CanonicalGameObjectBaseOwnerV1::read_bool(void* p,std::uint32_t o,std::uint8_t& v,std::string& e){auto* at=static_cast<CanonicalGameObjectBaseOwnerV1*>(p)->byte(o);if(!at){e="required actual bool source producer";return false;}v=*at;return true;}
bool CanonicalGameObjectBaseOwnerV1::write_bool(void* p,std::uint32_t o,std::uint8_t v,std::string& e){auto& s=*static_cast<CanonicalGameObjectBaseOwnerV1*>(p);if(o==0x80){s.lifecycle_.visible80=v;s.lifecycle_.visible_written=true;return true;}if(o==0x83||o==0x87){s.bytes_[o]=v;if(o==0x87)s.across_rooms_produced_=true;return true;}auto* at=s.byte(o);if(!at){e="required recovered bool source field";return false;}*at=v;return true;}
bool CanonicalGameObjectBaseOwnerV1::write_int(void* p,std::uint32_t o,std::int32_t v,std::string& e){auto* at=static_cast<CanonicalGameObjectBaseOwnerV1*>(p)->integer(o);if(!at){e="required recovered int source field";return false;}*at=v;return true;}
bool CanonicalGameObjectBaseOwnerV1::write_float(void* p,std::uint32_t o,float v,std::string& e){auto* at=static_cast<CanonicalGameObjectBaseOwnerV1*>(p)->scalar(o);if(!at){e="required recovered float source field";return false;}*at=v;return true;}
bool CanonicalGameObjectBaseOwnerV1::write_string(void* p,std::uint32_t o,const std::string& v,std::string& e){auto* at=static_cast<CanonicalGameObjectBaseOwnerV1*>(p)->string(o);if(!at){e="required recovered CString source field";return false;}*at=v;return true;}
bool CanonicalGameObjectBaseOwnerV1::write_vector3(void* p,std::uint32_t o,const std::array<float,3>& v,std::string& e){auto& s=*static_cast<CanonicalGameObjectBaseOwnerV1*>(p);auto* at=s.vector3(o);if(!at){e="required recovered vector source field";return false;}std::memcpy(at,v.data(),12);if(o==0x16c)s.runtime_.subobjects.rotation=v[2];return true;}
bool CanonicalGameObjectBaseOwnerV1::write_point2(void*,std::uint32_t,const std::array<std::int32_t,2>&,std::string& e){e="GameObject has no registered Point2Di field";return false;}
CanonicalObjectBorrowV1 CanonicalGameObjectBaseOwnerV1::canonical(std::shared_ptr<void> lease){return {identity_,std::move(lease),&handle_,&type_f4_,byte(0x87),&room64_,this,set_name,set_archetype,as_character,&class_name20_,read_across_rooms};}
CanonicalPropertyActorV1 CanonicalGameObjectBaseOwnerV1::properties()noexcept{return {class_name20_,&template_name8_,{this,read_bool,write_bool,write_int,write_float,write_string,write_vector3,write_point2}};}
}
