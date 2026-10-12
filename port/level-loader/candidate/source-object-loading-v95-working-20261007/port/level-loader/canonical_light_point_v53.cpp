#include "canonical_light_point_v53.hpp"
#include <algorithm>
#include <exception>
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
bool CanonicalLightPointV53::set_name(void* p,const char* v,std::string& e){if(!static_cast<CanonicalLightPointV53*>(p)->writable(e))return false;if(!v)return needs("name CString",e);static_cast<CanonicalLightPointV53*>(p)->strings_[0x30]=v;e.clear();return true;}
bool CanonicalLightPointV53::set_archetype(void* p,const char* v,std::string& e){if(!static_cast<CanonicalLightPointV53*>(p)->writable(e))return false;if(!v)return needs("archetype CString",e);static_cast<CanonicalLightPointV53*>(p)->strings_[0x48]=v;e.clear();return true;}
bool CanonicalLightPointV53::as_character(void*,std::uintptr_t& o,std::string& e){o=0;e.clear();return true;}
bool CanonicalLightPointV53::across(void* p,std::uint8_t& o,std::string& e){return read_bool(p,0x87,o,e);}
bool CanonicalLightPointV53::read_bool(void* p,std::uint32_t o,std::uint8_t& v,std::string& e){auto* x=at(static_cast<CanonicalLightPointV53*>(p)->bytes_,o);if(!x)return needs("produced byte",e);v=*x;e.clear();return true;}
bool CanonicalLightPointV53::write_bool(void* p,std::uint32_t o,std::uint8_t v,std::string& e){if(!static_cast<CanonicalLightPointV53*>(p)->writable(e))return false;auto& m=static_cast<CanonicalLightPointV53*>(p)->bytes_;if(o!=0x164&&!at(m,o))return needs("declared byte offset",e);m[o]=v;e.clear();return true;}
bool CanonicalLightPointV53::write_int(void* p,std::uint32_t o,std::int32_t v,std::string& e){if(!static_cast<CanonicalLightPointV53*>(p)->writable(e))return false;auto* x=at(static_cast<CanonicalLightPointV53*>(p)->ints_,o);if(!x)return needs("declared int offset",e);*x=v;e.clear();return true;}
bool CanonicalLightPointV53::write_float(void* p,std::uint32_t o,float v,std::string& e){if(!static_cast<CanonicalLightPointV53*>(p)->writable(e))return false;if(o!=0x130)return needs("radius source offset",e);static_cast<CanonicalLightPointV53*>(p)->floats_[o]=v;e.clear();return true;}
bool CanonicalLightPointV53::write_string(void* p,std::uint32_t o,const std::string& v,std::string& e){if(!static_cast<CanonicalLightPointV53*>(p)->writable(e))return false;auto* x=at(static_cast<CanonicalLightPointV53*>(p)->strings_,o);if(!x)return needs("constructed CString offset",e);*x=v;e.clear();return true;}
bool CanonicalLightPointV53::write_vector(void* p,std::uint32_t o,const std::array<float,3>& v,std::string& e){if(!static_cast<CanonicalLightPointV53*>(p)->writable(e))return false;auto* x=at(static_cast<CanonicalLightPointV53*>(p)->vectors_,o);if(!x)return needs("declared Point3D offset",e);*x=v;e.clear();return true;}
CanonicalObjectBorrowV1 CanonicalLightPointV53::canonical(std::shared_ptr<void> p){return {identity_,std::move(p),&handle_,&type_,nullptr,&room_,this,set_name,set_archetype,as_character,&class_name_,across};}
CanonicalPropertyActorV1 CanonicalLightPointV53::properties()noexcept{return {class_name_,&template_,{this,read_bool,write_bool,write_int,write_float,write_string,write_vector}};}
const std::string* CanonicalLightPointV53::string(std::uint32_t o)const noexcept{return at(strings_,o);}
const std::array<float,3>* CanonicalLightPointV53::vector(std::uint32_t o)const noexcept{return at(vectors_,o);}
const float* CanonicalLightPointV53::floating(std::uint32_t o)const noexcept{return at(floats_,o);}
const std::uint8_t* CanonicalLightPointV53::byte(std::uint32_t o)const noexcept{return at(bytes_,o);}
std::uint8_t* CanonicalLightPointV53::source_save_byte_v89(std::uint32_t offset,std::string& e){
 if(!writable(e))return nullptr;
 auto* value=at(bytes_,offset);if(!value)e="Required produced actual LightPoint ObjectBase save cell";
 return value;
}
bool CanonicalLightPointV53::source_loading_fields_v95(std::shared_ptr<void> pin,CanonicalObjectLoadingFieldsV95& out,std::string& e){
 if(!writable(e)||!pin){if(e.empty())e="Required live SAME LightPoint field lease";return false;}
 CanonicalObjectLoadingFieldsV95 value;value.receiver=std::move(pin);value.archetype48=string(0x48);
 value.enabled8a=at(bytes_,0x8a);value.minimum_ec=at(ints_,0xec);value.disabled_f1=byte(0xf1);
 value.condition_a8=&condition_a8_v95_;value.tested_ac=&tested_ac_v95_;value.condition_cc=&condition_cc_v95_;value.tested_d0=&tested_d0_v95_;
 //LightBase overrides SetVisible: do not substitute ObjectBase80-only stores.
 //A changed enabled state requires the real LightBase resource setter.
 out=std::move(value);e.clear();return true;
}
bool CanonicalLightPointV53::fail(const std::string& n,std::string& e){if(error_.empty())error_=n.empty()?"Actual LightPoint service failed":n;e=error_;return false;}
bool CanonicalLightPointV53::init_post(std::string& e){if(!writable(e))return false;
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
  if(!node_.identity||!node_.owner||!node_.actual_light_identity)return fail("Required actual retained LightPoint light node",e);
 auto* reference=node_.actor_reference.get();
 if(!reference||reference->actor_identity!=identity_||reference->node_identity!=node_.identity||
    reference->light_identity!=node_.actual_light_identity||!reference->stamp.complete()||
    !reference->runtime_owner||reference->runtime_owner.get()!=services_.owner.get()||
    reference->runtime_owner.owner_before(services_.owner)||services_.owner.owner_before(reference->runtime_owner))
  return fail("Required unique genuine SAME actor120 native reference",e);
 if(!services_.attach_root)return fail("Required actual LightPoint scene-root attachment",e);
 if(!services_.attach_root(node_,e))return fail(e,e);
 if(bytes_.at(0x164)){
  if(!services_.add_automatic)return fail("Required actual SceneManager automatic-light registry",e);
  if(!services_.add_automatic(identity_,node_,e))return fail(e,e);
 }
 bool write_parameters=true;
 if(!strings_.at(0x168).empty()){
  if(!services_.debug_switch)return fail("Required actual LightPoint TestMultiPlayerLight switch",e);
  if(!services_.debug_switch("TestMultiPlayerLight",write_parameters,e))return fail(e,e);
 }
 if(write_parameters){
  if(!services_.borrow_light_parameters)return fail("Required actual LightPoint CLight parameter storage",e);
  LightParameterFieldsV53 parameters;if(!services_.borrow_light_parameters(node_,parameters,e))return fail(e,e);
  if(parameters.actual_light_identity!=node_.actual_light_identity)return fail("Foreign CLight parameter owner",e);
  if(!write_light_parameters(parameters,e))return fail(e,e);
 }
 if(!services_.set_type)return fail("Required actual LightPoint CLight type58 writer",e);
 if(!services_.set_type(node_,0,e))return fail(e,e);
 if(!services_.names||!services_.refresh_attachment)return fail("Required actual LightPoint RefreshAttachment/shared light-set Names",e);
 if(!services_.refresh_attachment(identity_,node_,*services_.names,e))return fail(e,e);
 complete_=true;e.clear();return true;
}
bool CanonicalLightPointV53::sync_data(const LightParameterFieldsV53& f,std::string& e){if(!writable(e))return false;
 if(!strings_.at(0x168).empty()){
  if(!services_.owner||!services_.debug_switch)return needs("DebugSwitch TestMultiPlayerLight",e);
  bool override=false;if(!services_.debug_switch("TestMultiPlayerLight",override,e))return false;
  if(!override){e.clear();return true;} // exact source skip, not driver success
 }
 return write_light_parameters(f,e);
}
bool CanonicalLightPointV53::write_light_parameters(const LightParameterFieldsV53& f,std::string& e){
 if(!f.owner||!f.actual_light_identity||!f.ambient4||!f.diffuse14||!f.specular24||!f.attenuation34||!f.radius40||!f.validate_current)return needs("SAME current retained CLight parameter fields",e);
 if(!f.validate_current(e))return false;
 auto* radius=at(floats_,0x130);if(!radius)return needs("produced radius",e);
 if(*radius>0.01f)*f.radius40=*radius; // strict comparison, source preserves near-zero radius
 *f.attenuation34=vectors_.at(0x134);
 for(auto pair:{std::pair<std::array<float,4>*,std::uint32_t>{f.ambient4,0x140},{f.diffuse14,0x14c},{f.specular24,0x158}}){
  const auto& v=vectors_.at(pair.second);*pair.first={v[0],v[1],v[2],1.f};
 }
 if(!f.validate_current(e))return false;e.clear();return true;
}


namespace {
bool same_light_owner(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b)noexcept {
 return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);
}
}
bool CanonicalLightPointV53::writable(std::string& e)const {
 if(teardown_busy_||teardown_state_!=LightTeardownStateV67::Alive){e="LightPoint source teardown forbids further initialization/property mutation";return false;}
 return true;
}
bool CanonicalLightPointV53::prepare_source_teardown(std::shared_ptr<LightQuiescenceLeaseV67> lease,std::string& e){
 if(teardown_busy_){teardown_reentered_=true;e="Recursive LightPoint source teardown rejected";return false;}
 if(teardown_state_==LightTeardownStateV67::ObjectBaseCompleted){e.clear();return true;}
 if(teardown_state_!=LightTeardownStateV67::Alive&&teardown_state_!=LightTeardownStateV67::Prepared){e="LightPoint native source release prefix already entered";return false;}
 if(teardown_state_==LightTeardownStateV67::Prepared&&teardown_lease_.get()!=lease.get()){
  e="LightPoint source release already prepared with another lease";return false;
 }
 teardown_busy_=true;teardown_reentered_=false;
 struct BusyExit {bool& busy;~BusyExit(){busy=false;}} guard{teardown_busy_};
 if(!lease||!lease->stamp.complete()||!lease->owning_thread_barrier||
    !same_light_owner(services_.owner,lease->runtime_owner)||!lease->validate_current_owning_thread()){
  e="Required SAME current runtime owning-thread light quiescence";return false;
 }
 if(!objectbase_dtor_){
  if(!services_.bind_objectbase_dtor){e="Required actual SAME LightPoint ObjectBase destruction continuation";return false;}
  std::unique_ptr<LightObjectBaseContinuationV67> candidate;
    try {if(!services_.bind_objectbase_dtor(identity_,candidate,e))return false;}
  catch(const std::exception& ex){e=ex.what();return false;}
  objectbase_dtor_=std::move(candidate); // passive capability; no native mutation
 }
 auto* base=objectbase_dtor_.get();
 if(!base||base->actor_identity!=identity_||!(base->stamp==lease->stamp)||
    !same_light_owner(base->runtime_owner,lease->runtime_owner)||!base->validate_exact_actor(*lease)){
  e="Required genuine exact-actor ObjectBase continuation";return false;
 }
 if(strings_.find(0x180)==strings_.end()||strings_.find(0x168)==strings_.end()){
  e="Missing SAME constructed LightPoint source CString fields";return false;
 }
 auto* reference=node_.actor_reference.get();
 if(node_.identity){
  if(!node_.owner||!reference||reference->actor_identity!=identity_||reference->node_identity!=node_.identity||
     reference->light_identity!=node_.actual_light_identity||!(reference->stamp==lease->stamp)||
     !same_light_owner(reference->runtime_owner,lease->runtime_owner)||!reference->validate_exact_actor_reference(*lease)){
   e="Required unique current exact-actor native node120 reference";return false;
  }
 }else if(reference||node_.actual_light_identity){e="Foreign native reference while LightPoint node120 is null";return false;}
 if(teardown_reentered_){e="LightPoint source preflight recursively entered; no mutation admitted";return false;}
 // Main has quiesced raw property/template/room borrows too. No actor fields
 // mutate on rejection above; journal and original registry remain retained.
 teardown_lease_=std::move(lease);teardown_state_=LightTeardownStateV67::Prepared;e.clear();return true;
}
bool CanonicalLightPointV53::execute_source_teardown(std::shared_ptr<LightQuiescenceLeaseV67> lease,std::string& e){
 if(teardown_busy_){teardown_reentered_=true;e="Recursive LightPoint source execution rejected";return false;}
 if(teardown_state_==LightTeardownStateV67::ObjectBaseCompleted){e.clear();return true;}
 if(!prepare_source_teardown(std::move(lease),e))return false;
 teardown_busy_=true;
 struct BusyExit {bool& busy;~BusyExit(){busy=false;}} guard{teardown_busy_};
 auto attached=strings_.find(0x180);auto dae=strings_.find(0x168);
 // Source40bd1c: attachedTo's allocation is released FIRST.
 {std::string empty;empty.swap(attached->second);}
 teardown_state_=LightTeardownStateV67::AttachedTo180Released;
 if(node_.identity){
  // Source40ab34 calls real31d584 and only THEN clears120. The actual
  // intrusive drop may delete the node. Do not inspect/validate it afterwards.
  node_.actor_reference->drop_actual_actor_node_reference();
  teardown_state_=LightTeardownStateV67::NativeActorReferenceDropped;
  node_.identity=0;node_.actual_light_identity=0;
  teardown_state_=LightTeardownStateV67::Node120Cleared;
  node_.actor_reference.reset(); // consumed token destructor does not drop
  node_.owner.reset(); // passive lifetime pin, not the source intrusive drop
 }else teardown_state_=LightTeardownStateV67::Node120Cleared;
 {std::string empty;empty.swap(dae->second);}
 teardown_state_=LightTeardownStateV67::Dae168Released;
  LightObjectBaseFieldsV67 base_fields{identity_,&handle_,&bytes_.at(0x29),&template_,
  &strings_.at(0x30),&strings_.at(0x48),&strings_.at(0x68),&strings_.at(0x90),
  &strings_.at(0xb4),&strings_.at(0xd4)};
 objectbase_dtor_->destroy_actual_objectbase_fields(base_fields);
 teardown_state_=LightTeardownStateV67::ObjectBaseCompleted;e.clear();return true;
 // No invented automatic-list removal: original D0 leaves raw entries intact.
 // Main retains quiescence until actual SceneManager.Clear/no old readers.
}
}


