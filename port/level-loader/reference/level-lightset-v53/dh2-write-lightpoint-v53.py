from pathlib import Path
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/level-lightset-v53'
(b/'canonical_light_point_v53.hpp').write_text(r'''#pragma once
#include <canonical_property_map_v1.hpp>
#include <light_set_name_owner_v3.hpp>
#include <memory>
namespace dh2::world {
// A borrowed ACTUAL renderer light node. Main supplies its retained node owner,
// native identity and storage writer. A source miss is node.identity==0.
struct LightNodeBorrowV53 {std::shared_ptr<void> owner;std::uintptr_t identity{};};
struct LightPointInitServicesV53 {
 std::shared_ptr<void> owner;
 // Original LoadScene(dae,"",false,false), choose first actual light node.
 std::function<bool(const std::string&,const char*,bool,bool,LightNodeBorrowV53&,std::string&)> first_scene_light;
 std::function<bool(bool,LightNodeBorrowV53&,std::string&)> construct_light_node;
 std::function<bool(const LightNodeBorrowV53&,std::string&)> attach_root;
 std::function<bool(std::uintptr_t,const LightNodeBorrowV53&,std::string&)> add_automatic;
 // Source SyncData's actual CLight storage/driver writer; not GPU completion.
 std::function<bool(std::uintptr_t,const LightNodeBorrowV53&,std::string&)> sync_data;
 std::function<bool(const LightNodeBorrowV53&,std::uint16_t,std::string&)> set_type;
 // Must borrow the existing manager Names owner, not allocate a parallel one.
 std::shared_ptr<LightSetNameOwnerV3> names;
 std::function<bool(std::uintptr_t,const LightNodeBorrowV53&,LightSetNameOwnerV3&,std::string&)> refresh_attachment;
};
// Genuine ObjectBase-derived factory34115c / LightPointC1_40bdd8, GO_ID19.
// Never inherits GameObject geometry/Visual/Character fields.
class CanonicalLightPointV53 final {
 std::shared_ptr<void> pin_;std::uintptr_t identity_;
 target_providers::Handle16 handle_;std::uint32_t type_{19};std::int32_t room_{-1};const char* class_name_{};
 std::string template_;std::map<std::uint32_t,std::uint8_t> bytes_;
 std::map<std::uint32_t,std::int32_t> ints_;std::map<std::uint32_t,float> floats_;
 std::map<std::uint32_t,std::string> strings_;std::map<std::uint32_t,std::array<float,3>> vectors_;
 LightPointInitServicesV53 services_;LightNodeBorrowV53 node_;bool attempted_{},complete_{};std::string error_;
 static bool set_name(void*,const char*,std::string&);static bool set_archetype(void*,const char*,std::string&);
 static bool as_character(void*,std::uintptr_t&,std::string&);static bool across(void*,std::uint8_t&,std::string&);
 static bool read_bool(void*,std::uint32_t,std::uint8_t&,std::string&);
 static bool write_bool(void*,std::uint32_t,std::uint8_t,std::string&);static bool write_int(void*,std::uint32_t,std::int32_t,std::string&);
 static bool write_float(void*,std::uint32_t,float,std::string&);static bool write_string(void*,std::uint32_t,const std::string&,std::string&);
 static bool write_vector(void*,std::uint32_t,const std::array<float,3>&,std::string&);
 bool fail(const std::string&,std::string&);
public:
 CanonicalLightPointV53(std::shared_ptr<void>,LightPointInitServicesV53);
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void>);
 CanonicalPropertyActorV1 properties()noexcept;
 bool init_post(std::string&);
 const std::string* string(std::uint32_t)const noexcept;
 const std::array<float,3>* vector(std::uint32_t)const noexcept;
 const float* floating(std::uint32_t)const noexcept;
 const std::uint8_t* byte(std::uint32_t)const noexcept;
 std::uintptr_t identity()const noexcept{return identity_;}
 const LightNodeBorrowV53& actual_node()const noexcept{return node_;}
 bool init_complete()const noexcept{return complete_;}
 static constexpr bool is_game_object()noexcept{return false;}
};
}
''')
(b/'canonical_light_point_v53.cpp').write_text(r'''#include "canonical_light_point_v53.hpp"
#include <algorithm>
namespace dh2::world {
namespace {
template<class T>T* at(std::map<std::uint32_t,T>& m,std::uint32_t o){auto i=m.find(o);return i==m.end()?nullptr:&i->second;}
template<class T>const T* at(const std::map<std::uint32_t,T>& m,std::uint32_t o){auto i=m.find(o);return i==m.end()?nullptr:&i->second;}
bool needs(const char* n,std::string& e){e="Required actual LightPoint ";e+=n;return false;}
}
CanonicalLightPointV53::CanonicalLightPointV53(std::shared_ptr<void> p,LightPointInitServicesV53 s):pin_(std::move(p)),identity_(reinterpret_cast<std::uintptr_t>(this)),handle_{0,UINT32_MAX,identity_},services_(std::move(s)){
 for(auto o:{0x28u,0x29u,0x60u,0x81u,0x84u,0x86u,0x88u,0x89u,0xf0u,0xf1u,0xf8u,0x10cu,0x118u,0x119u})bytes_[o]=0;
 bytes_[0x85]=1;bytes_[0x8a]=1;
 // Modern deterministic room-local safety, as other ObjectBase receivers:
 // original C1 leaves87 indeterminate; not constructor byte parity for87.
 bytes_[0x87]=0;
 for(auto o:{0xecu,0x108u,0x110u})ints_[o]=-1;
 for(auto o:{0x30u,0x48u,0x68u,0x90u,0xb4u,0xd4u,0x168u,0x180u})strings_[o]={};
 for(auto o:{0x124u,0x134u,0x140u,0x14cu,0x158u,0x1a4u})vectors_[o]={0,0,0};
 // radius130 and automatic164 were NOT initialized by C1. Descriptor defaults
 // produce them; missing property initialization is an explicit failure.
}
bool CanonicalLightPointV53::set_name(void* p,const char* v,std::string& e){if(!v)return needs("name CString",e);static_cast<CanonicalLightPointV53*>(p)->strings_[0x30]=v;e.clear();return true;}
bool CanonicalLightPointV53::set_archetype(void* p,const char* v,std::string& e){if(!v)return needs("archetype CString",e);static_cast<CanonicalLightPointV53*>(p)->strings_[0x48]=v;e.clear();return true;}
bool CanonicalLightPointV53::as_character(void*,std::uintptr_t& o,std::string& e){o=0;e.clear();return true;}
bool CanonicalLightPointV53::across(void* p,std::uint8_t& o,std::string& e){return read_bool(p,0x87,o,e);}
bool CanonicalLightPointV53::read_bool(void* p,std::uint32_t o,std::uint8_t& v,std::string& e){auto* x=at(static_cast<CanonicalLightPointV53*>(p)->bytes_,o);if(!x)return needs("produced byte",e);v=*x;e.clear();return true;}
bool CanonicalLightPointV53::write_bool(void* p,std::uint32_t o,std::uint8_t v,std::string& e){auto& m=static_cast<CanonicalLightPointV53*>(p)->bytes_;if(o!=0x164&&!at(m,o))return needs("declared byte offset",e);m[o]=v;e.clear();return true;}
bool CanonicalLightPointV53::write_int(void* p,std::uint32_t o,std::int32_t v,std::string& e){auto* x=at(static_cast<CanonicalLightPointV53*>(p)->ints_,o);if(!x)return needs("declared int offset",e);*x=v;e.clear();return true;}
bool CanonicalLightPointV53::write_float(void* p,std::uint32_t o,float v,std::string& e){if(o!=0x130)return needs("radius source offset",e);static_cast<CanonicalLightPointV53*>(p)->floats_[o]=v;e.clear();return true;}
bool CanonicalLightPointV53::write_string(void* p,std::uint32_t o,const std::string& v,std::string& e){auto* x=at(static_cast<CanonicalLightPointV53*>(p)->strings_,o);if(!x)return needs("constructed CString offset",e);*x=v;e.clear();return true;}
bool CanonicalLightPointV53::write_vector(void* p,std::uint32_t o,const std::array<float,3>& v,std::string& e){auto* x=at(static_cast<CanonicalLightPointV53*>(p)->vectors_,o);if(!x)return needs("declared Point3D offset",e);*x=v;e.clear();return true;}
CanonicalObjectBorrowV1 CanonicalLightPointV53::canonical(std::shared_ptr<void> p){return {identity_,std::move(p),&handle_,&type_,nullptr,&room_,this,set_name,set_archetype,as_character,&class_name_,across};}
CanonicalPropertyActorV1 CanonicalLightPointV53::properties()noexcept{return {class_name_,&template_,{this,read_bool,write_bool,write_int,write_float,write_string,write_vector}};}
const std::string* CanonicalLightPointV53::string(std::uint32_t o)const noexcept{return at(strings_,o);}
const std::array<float,3>* CanonicalLightPointV53::vector(std::uint32_t o)const noexcept{return at(vectors_,o);}
const float* CanonicalLightPointV53::floating(std::uint32_t o)const noexcept{return at(floats_,o);}
const std::uint8_t* CanonicalLightPointV53::byte(std::uint32_t o)const noexcept{return at(bytes_,o);}
bool CanonicalLightPointV53::fail(const std::string& n,std::string& e){if(error_.empty())error_=n;e=error_;return false;}
bool CanonicalLightPointV53::init_post(std::string& e){
 if(complete_){e.clear();return true;}if(attempted_)return fail("LightPoint source prefix already attempted; release candidate",e);
 if(!at(floats_,0x130)||!at(bytes_,0x164))return fail("Required actual LightPoint descriptor defaults/overrides",e);
 attempted_=true;bytes_[0x1b0]=0;bytes_[0x1b1]=0;
 for(auto o:{0x140u,0x14cu,0x158u})for(auto& v:vectors_.at(o))v=v/255.f;
 vectors_.at(0x134)[1]=vectors_.at(0x134)[1]/1000.f;vectors_.at(0x134)[2]=vectors_.at(0x134)[2]/1000000.f;
 if(!services_.owner)return fail("Required actual LightPoint renderer owner",e);
 if(!strings_.at(0x168).empty()){
  if(!services_.first_scene_light)return fail("Required actual LightPoint SceneManager.LoadScene",e);
  if(!services_.first_scene_light(strings_.at(0x168),"",false,false,node_,e))return fail(e,e);
 }
 if(!node_.identity){
  if(!services_.construct_light_node)return fail("Required actual LightPoint CLightSceneNode(true)",e);
  if(!services_.construct_light_node(true,node_,e))return fail(e,e);
 }
 if(!node_.identity||!node_.owner)return fail("Required actual retained LightPoint light node",e);
 if(!services_.attach_root)return fail("Required actual LightPoint scene-root attachment",e);
 if(!services_.attach_root(node_,e))return fail(e,e);
 if(bytes_.at(0x164)){
  if(!services_.add_automatic)return fail("Required actual SceneManager automatic-light registry",e);
  if(!services_.add_automatic(identity_,node_,e))return fail(e,e);
 }
 if(!services_.sync_data)return fail("Required actual LightPoint SyncData CLight writer",e);
 if(!services_.sync_data(identity_,node_,e))return fail(e,e);
 if(!services_.set_type)return fail("Required actual LightPoint CLight type58 writer",e);
 if(!services_.set_type(node_,0,e))return fail(e,e);
 if(!services_.names||!services_.refresh_attachment)return fail("Required actual LightPoint RefreshAttachment/shared light-set Names",e);
 if(!services_.refresh_attachment(identity_,node_,*services_.names,e))return fail(e,e);
 complete_=true;e.clear();return true;
}
}
''')
(r/'canonical_light_point_declarations_v53.inc.proposed').write_text('''// Insert within actual CanonicalPropertyMapV1 init_properties declaration body.
// Also add LightPoint to recovered class whitelist; require ZERO producer.
if(kind=="LightPoint") {
 add("position",0x124,*source_.position_rotation_default);
 add("radius",0x130,float{2.0f});
 add("attenuation",0x134,std::array<float,3>{{0,0,0}});
 add("ambientColor",0x140,std::array<float,3>{{0,0,0}});
 add("diffuseColor",0x14c,std::array<float,3>{{0,0,0}});
 add("specularColor",0x158,std::array<float,3>{{0,0,0}});
 add("automatic",0x164,std::uint8_t{0});
 add("dae",0x168,std::string{});
 add("attachedTo",0x180,std::string{});
 add("attachedOffset",0x1a4,*source_.position_rotation_default);
}
''')
print('Wrote unique GO19 constructor/property/scalar-init candidate and declaration fragment')
