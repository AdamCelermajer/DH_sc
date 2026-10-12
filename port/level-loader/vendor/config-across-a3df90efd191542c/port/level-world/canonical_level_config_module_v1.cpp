#include "canonical_level_config_module_v1.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace dh2::world {
namespace {
template<class T>T* at(std::map<std::uint32_t,T>& map,std::uint32_t o){auto p=map.find(o);return p==map.end()?nullptr:&p->second;}
template<class T>const T* at(const std::map<std::uint32_t,T>& map,std::uint32_t o){auto p=map.find(o);return p==map.end()?nullptr:&p->second;}
bool contains(std::initializer_list<std::uint32_t> offsets,std::uint32_t o){return std::find(offsets.begin(),offsets.end(),o)!=offsets.end();}
bool required(const char* name,std::string& e){e="Required actual LevelConfig/Module ";e+=name;return false;}
std::int32_t signed_id(std::uint32_t n){std::int32_t out;std::memcpy(&out,&n,4);return out;}
}
CanonicalLevelConfigV1::CanonicalLevelConfigV1(std::shared_ptr<void> pin,LevelConfigServicesV1 s):pin_(std::move(pin)),identity_(reinterpret_cast<std::uintptr_t>(this)),handle_{0,UINT32_MAX,identity_},services_(std::move(s)){
 // Exact ObjectBaseC2 + LevelConfigC1; uninitialized source fields remain
 // absent until a genuine descriptor default/override producer writes them.
 for(auto o:{0x28u,0x29u,0x60u,0x81u,0x84u,0x85u,0x86u,0x88u,0x89u,0xf0u,0xf1u,0xf8u,0x10cu,0x118u,0x119u,0x29cu})bytes_[o]=0;
 bytes_[0x8a]=1;for(auto o:{0xecu,0x108u,0x110u})ints_[o]=-1;
 // Modern safety correction, NOT original constructor parity: the original
 // malloc-backed LevelConfig leaves 87 indeterminate and declares no inherited
 // global property. ObjectManager reads it before comparing names in a room
 // lookup. Give this one field deterministic room-local semantics; preserve
 // the genuine type4, room-1 and ordered Add/default/InitPost producers.
 bytes_[0x87]=0;
 for(auto o:{0x30u,0x48u,0x68u,0x90u,0xb4u,0xd4u,0x120u,0x138u,0x150u,0x168u,0x180u,0x198u,0x1b0u,0x234u,0x24cu,0x264u,0x27cu,0x2b8u,0x2d0u,0x2e8u,0x300u})strings_[o]={};
 for(auto o:{0x1ccu,0x1e0u,0x1ecu,0x1f8u,0x218u,0x224u})vectors_[o]={0,0,0};
}
bool CanonicalLevelConfigV1::set_name(void* p,const char* v,std::string& e){if(!v)return required("ObjectBase SetName CString",e);static_cast<CanonicalLevelConfigV1*>(p)->strings_[0x30]=v;return true;}
bool CanonicalLevelConfigV1::set_archetype(void* p,const char* v,std::string& e){if(!v)return required("ObjectBase archetype CString",e);static_cast<CanonicalLevelConfigV1*>(p)->strings_[0x48]=v;return true;}
bool CanonicalLevelConfigV1::as_character(void*,std::uintptr_t& out,std::string&){out=0;return true;}
bool CanonicalLevelConfigV1::across(void* p,std::uint8_t& out,std::string& e){return read_bool(p,0x87,out,e);}
bool CanonicalLevelConfigV1::read_bool(void* p,std::uint32_t o,std::uint8_t& out,std::string& e){auto* value=at(static_cast<CanonicalLevelConfigV1*>(p)->bytes_,o);if(!value)return required("produced source bool field",e);out=*value;return true;}
bool CanonicalLevelConfigV1::write_bool(void* p,std::uint32_t o,std::uint8_t v,std::string& e){auto& s=*static_cast<CanonicalLevelConfigV1*>(p);if(!contains({0x80,0x83,0x87,0x1c8,0x2a8,0x2b4},o)&&!at(s.bytes_,o))return required("declared bool source offset",e);s.bytes_[o]=v;return true;}
bool CanonicalLevelConfigV1::write_int(void* p,std::uint32_t o,std::int32_t v,std::string& e){if(!contains({0xec,0x108,0x110,0x1d8,0x1dc,0x294,0x298,0x2a0,0x2a4,0x2ac,0x2b0},o))return required("declared int source offset",e);static_cast<CanonicalLevelConfigV1*>(p)->ints_[o]=v;return true;}
bool CanonicalLevelConfigV1::write_float(void* p,std::uint32_t o,float v,std::string& e){if(!contains({0x210,0x214,0x230},o))return required("declared float source offset",e);static_cast<CanonicalLevelConfigV1*>(p)->floats_[o]=v;return true;}
bool CanonicalLevelConfigV1::write_string(void* p,std::uint32_t o,const std::string& v,std::string& e){auto* dest=at(static_cast<CanonicalLevelConfigV1*>(p)->strings_,o);if(!dest)return required("constructed CString source offset",e);*dest=v;return true;}
bool CanonicalLevelConfigV1::write_vector(void* p,std::uint32_t o,const std::array<float,3>& v,std::string& e){auto* dest=at(static_cast<CanonicalLevelConfigV1*>(p)->vectors_,o);if(!dest)return required("declared Point3D source offset",e);*dest=v;return true;}
bool CanonicalLevelConfigV1::write_point2(void*,std::uint32_t,const std::array<std::int32_t,2>&,std::string& e){return required("unsupported Point2D declaration",e);}
bool CanonicalLevelConfigV1::write_list(void* p,std::uint32_t o,const CanonicalPoint3ListV1& v,std::string& e){if(o!=0x204)return required("declared vector<Point3D> source offset",e);static_cast<CanonicalLevelConfigV1*>(p)->dfog_=v;return true;}
CanonicalObjectBorrowV1 CanonicalLevelConfigV1::canonical(std::shared_ptr<void> lease){return {identity_,std::move(lease),&handle_,&type_,nullptr,&room_,this,set_name,set_archetype,as_character,&class_name_,across};}
CanonicalPropertyActorV1 CanonicalLevelConfigV1::properties()noexcept{return {class_name_,&template_,{this,read_bool,write_bool,write_int,write_float,write_string,write_vector,write_point2,write_list}};}
const std::string* CanonicalLevelConfigV1::string(std::uint32_t o)const noexcept{return at(strings_,o);}
const std::array<float,3>* CanonicalLevelConfigV1::vector(std::uint32_t o)const noexcept{return at(vectors_,o);}
const std::int32_t* CanonicalLevelConfigV1::integer(std::uint32_t o)const noexcept{return at(ints_,o);}
const std::uint8_t* CanonicalLevelConfigV1::byte(std::uint32_t o)const noexcept{return at(bytes_,o);}
bool CanonicalLevelConfigV1::init_post(std::string& e){
 if(bytes_.at(0x29c))return true;bytes_[0x29c]=1;
 if(strings_[0x24c].empty())strings_[0x24c]="data/3D/camera/CameraTests.bdae";
 if(strings_[0x27c].empty())strings_[0x27c]="PlayerCamera_Default";
 if(strings_[0x2b8].empty())strings_[0x2b8]="data/3D/Light/default.lightset_xml";
 if(strings_[0x2d0].empty())strings_[0x2d0]="data/3D/Light/default.lightset_xml";
 for(auto& value:vectors_.at(0x1cc))value=value/255.f;
 if(!services_.owner||!services_.debug_switch)return required("shared Debug isTracingLevel",e);
 for(unsigned i=0;i<4;++i){bool ignored=false;if(!services_.debug_switch("isTracingLevel",ignored,e))return false;}
 if(!services_.set_level_config)return required("SAME Level::SetLevelConfig3f150c",e);
 return services_.set_level_config(identity_,e);
}
CanonicalModuleV1::CanonicalModuleV1(std::shared_ptr<void> pin,actor::RuntimeState& runtime,ModuleRuntimeGlobalsV1& globals,GameObjectInitializationServicesV1 init,ModuleInitServicesV1 s):base_(reinterpret_cast<std::uintptr_t>(this),5,std::move(pin),runtime),initialization_(base_,std::move(init)),globals_(globals),services_(std::move(s)){
 base_.lifecycle().static84=1;*base_.byte(0x28)=1;inherited_=base_.properties().fields;id_=signed_id(globals_.next_module_id++);
}
bool CanonicalModuleV1::read_bool(void* p,std::uint32_t o,std::uint8_t& v,std::string& e){auto& s=*static_cast<CanonicalModuleV1*>(p);if(o==0x375){v=s.floor375_;return true;}if(o==0x376){v=s.solid376_;return true;}return s.inherited_.read_bool(s.inherited_.context,o,v,e);}
bool CanonicalModuleV1::write_bool(void* p,std::uint32_t o,std::uint8_t v,std::string& e){auto& s=*static_cast<CanonicalModuleV1*>(p);if(o==0x375){s.floor375_=v;return true;}if(o==0x376){s.solid376_=v;return true;}return s.inherited_.write_bool(s.inherited_.context,o,v,e);}
bool CanonicalModuleV1::write_int(void* p,std::uint32_t o,std::int32_t v,std::string& e){auto& s=*static_cast<CanonicalModuleV1*>(p);return s.inherited_.write_int(s.inherited_.context,o,v,e);}
bool CanonicalModuleV1::write_float(void* p,std::uint32_t o,float v,std::string& e){auto& s=*static_cast<CanonicalModuleV1*>(p);return s.inherited_.write_float(s.inherited_.context,o,v,e);}
bool CanonicalModuleV1::write_string(void* p,std::uint32_t o,const std::string& v,std::string& e){auto& s=*static_cast<CanonicalModuleV1*>(p);switch(o){case 0x378:s.xml_.mgp378=v;return true;case 0x390:s.xml_.mvp390=v;return true;case 0x3a8:s.xml_.alt_mgp3a8=v;return true;case 0x3c0:s.xml_.alt_mvp3c0=v;return true;case 0x3d8:s.xml_.alt_prob3d8=v;return true;default:return s.inherited_.write_string(s.inherited_.context,o,v,e);}}
bool CanonicalModuleV1::write_vector(void* p,std::uint32_t o,const std::array<float,3>& v,std::string& e){auto& s=*static_cast<CanonicalModuleV1*>(p);if(o==0x3f0){s.fog_=v;return true;}return s.inherited_.write_vector3(s.inherited_.context,o,v,e);}
bool CanonicalModuleV1::write_point2(void* p,std::uint32_t o,const std::array<std::int32_t,2>& v,std::string& e){auto& s=*static_cast<CanonicalModuleV1*>(p);return s.inherited_.write_point2(s.inherited_.context,o,v,e);}
CanonicalPropertyActorV1 CanonicalModuleV1::properties()noexcept{auto a=base_.properties();a.fields={this,read_bool,write_bool,write_int,write_float,write_string,write_vector,write_point2};return a;}
bool CanonicalModuleV1::init_post(std::string& e){
 bool eligible=false;if(!initialization_.init_post(eligible,e))return false;
 auto visual=*base_.pointer(0x2d8);if(!visual)return true;
 if(!services_.visual_apply_mesh_box||!services_.visual_physical)return required("Decor same visual ApplyMeshBox/physical28",e);
 if(!services_.visual_apply_mesh_box(visual,e))return false;bool physical=false;if(!services_.visual_physical(visual,physical,e))return false;
 if(physical){std::uintptr_t body=0;if(!services_.construct_podecor||!services_.set_physical)return required("Decor same native PODecor/SetPhysical(false)",e);if(!services_.construct_podecor(body,e)||!services_.set_physical(body,false,e))return false;}
 if(floor375_){if(!services_.load_floor)return required("PFWorld::LoadRoom523c14",e);if(!services_.load_floor(visual,base_.room64(),*base_.string(0x30),solid376_!=0,base_.absolute_aabb12c(),e))return false;floor375_=0;}
 if(!services_.root_world_bounds||!services_.spawn_room_zone)return required("Module transformed root bounds/actual RoomZone spawn",e);
 std::array<float,6> bounds{};if(!services_.root_world_bounds(visual,bounds,e))return false;
 char name[64];std::snprintf(name,sizeof(name),"generated_roomzone_%04d",signed_id(globals_.next_generated_zone++));
 target_providers::Handle16 handle{};std::uint32_t type=0;
 if(!services_.spawn_room_zone(name,handle,type,e))return false;
 if(handle.cached&&type==11){zone_=handle;if(!services_.zone_init)return required("Zone InitWithBoundingBox/Module38c backlink",e);return services_.zone_init(zone_,bounds,base_.identity(),e);}return true;
}
}
