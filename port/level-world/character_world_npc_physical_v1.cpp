#include "character_world_npc_physical_v1.hpp"
#include <stdexcept>
#include <exception>
namespace dh2::character {
bool character_npc_physical_debug_v1(DebugSwitches& owner,const DebugFileServices24& files,
 const char* key,bool& result,std::string& error){
 if(!key||(std::string(key)!="MP_NoCollisions"&&std::string(key)!="MP_NoPhysics")){error="Required exact source physical Debug key";return false;}
 if(dh2_character_debug_load(&owner,&files)!=1){error="Required shared physical Debug load";return false;}
 const std::string temporary(key);std::uint32_t value{};
 if(dh2_character_debug_get(&value,&owner,temporary.c_str(),&files)!=1){error="Required shared physical Debug query";return false;}
 result=value!=0;return true;
}
CharacterWorldNpcPhysicalV1::CharacterWorldNpcPhysicalV1(physical::NativeWorld& w,
 physical::CharacterNpcBodyModel& m,CharacterWorldNpcCollisionV1& c,ScriptCharacterObject& o,
 CharacterScriptSession& s,WorldNpcPhysicalServicesV1 services):world_(w),model_(m),collisions_(c),object_(o),session_(s),services_(services){
 world_object_.context=this;world_object_.test=test;world_object_.contact=contact;world_object_.velocity=velocity;
}
CharacterWorldNpcPhysicalV1::~CharacterWorldNpcPhysicalV1(){
 // A callback failure cannot silently leave a dangling WorldObject context.
 // Explicit release exposes its error to the caller before this destructor.
 if(native_.body&&!release())std::terminate();
}
[[noreturn]] void CharacterWorldNpcPhysicalV1::unavailable(const char* message){
 if(error_.empty())error_=message;throw std::runtime_error(error_);
}
unsigned CharacterWorldNpcPhysicalV1::test(void* p,void* other,const physical::Filter* own_filter,const physical::Filter* other_filter){
 auto& t=*static_cast<CharacterWorldNpcPhysicalV1*>(p);t.error_.clear();
 if(!own_filter||!other_filter)t.unavailable("Required actual native shape filters");
 bool allowed{};if(!t.collisions_.permits_filter(other_filter->category,allowed)){t.error_=t.collisions_.error();t.unavailable("Required POCharacter source filter prefix");}if(!allowed)return 0;
 std::uintptr_t peer{};if(!t.services_.peer_owner||!t.services_.peer_owner(t.services_.context,other,peer,t.error_))t.unavailable("Required actual physical peer owner");
 std::uint8_t enabled{};
 if(!t.services_.enabled80||!t.services_.enabled80(t.services_.context,t.object_.identity,enabled,t.error_))t.unavailable("Required actual ObjectBase+80");if(!enabled)return 0;
 if(peer){if(!t.services_.enabled80(t.services_.context,peer,enabled,t.error_))t.unavailable("Required actual peer ObjectBase+80");if(!enabled)return 0;}
 if(own_filter->group&&own_filter->group==other_filter->group)return own_filter->group>0;
 return (other_filter->category&own_filter->mask)&&(other_filter->mask&own_filter->category);
}
void CharacterWorldNpcPhysicalV1::contact(void* p,physical::ContactEvent event,void* other,const float*,unsigned persist){
 auto& t=*static_cast<CharacterWorldNpcPhysicalV1*>(p);t.error_.clear();
 if(event==physical::ContactEvent::result)return; // Whole POCharacter Result 46fb14: bx lr.
 std::uintptr_t peer{};if(!t.services_.peer_owner||!t.services_.peer_owner(t.services_.context,other,peer,t.error_))t.unavailable("Required actual collision peer owner");
 const auto code=event==physical::ContactEvent::add?WorldNpcPhysicalEventV1::begin:
 event==physical::ContactEvent::persist?WorldNpcPhysicalEventV1::persist:WorldNpcPhysicalEventV1::end;
 if(!t.collisions_.physical_event(code,peer,persist)){t.error_=t.collisions_.error();t.unavailable("Required actual NPC collision continuation");}
}
void CharacterWorldNpcPhysicalV1::velocity(void* p,float* xy){
 auto& t=*static_cast<CharacterWorldNpcPhysicalV1*>(p);physical::NativeBodyObservation observation{};
 if(!xy||dh2_native_body_observe(&observation,&t.native_))t.unavailable("Required actual physical velocity");
 xy[0]=observation.linear_velocity[0]*100.f;xy[1]=observation.linear_velocity[1]*100.f;
}
bool CharacterWorldNpcPhysicalV1::initialize(physical::NpcBodyRequest request){
 return initialize_impl(request,false);
}
bool CharacterWorldNpcPhysicalV1::initialize_source(physical::NpcBodyRequest request){
 return initialize_impl(request,true);
}
bool CharacterWorldNpcPhysicalV1::initialize_impl(physical::NpcBodyRequest request,bool source_debug){
 error_.clear();
 if(phase_||native_.body||!world_.backend()||request.previous_physical||
 request.properties!=&session_.property_view()||session_.properties().get()!=object_.properties.get()||
 session_.combat_state().get()!=object_.life.get()){
  error_="NPC physical initialization requires fresh body and same World/script/property/life owner";return false;
 }
 request.owner=reinterpret_cast<void*>(object_.identity);request.new_physical=&native_;
 if(source_debug){request.collision_group_override=0;request.disable_physical=0;}
 if(!model_.project(request,projection_,error_))return false;phase_=1;
 try{
  if(projection_.body.enabled){
   if(source_debug){bool override_group{};if(!services_.debug_switch||!services_.debug_switch(services_.context,"MP_NoCollisions",override_group,error_)){if(error_.empty())error_="Required actual source MP_NoCollisions Debug producer";return false;}request.collision_group_override=override_group;if(!model_.project(request,projection_,error_))return false;}
   saved_filter_.groupIndex=static_cast<int16>(projection_.body.shape.group_index);
   saved_filter_.categoryBits=static_cast<uint16>(projection_.body.shape.category_bits);
   saved_filter_.maskBits=static_cast<uint16>(projection_.body.shape.mask_bits);
   native_={world_.create_character(projection_.body,&world_object_),projection_.body.radius,projection_.body.pinned};
   if(!native_.body){error_="Genuine NPC body allocation failed";return false;}
   primary_=native_.body->GetShapeList();phase_=2;
  }
  if(source_debug){bool disabled{};if(!services_.debug_switch||!services_.debug_switch(services_.context,"MP_NoPhysics",disabled,error_)){if(error_.empty())error_="Required actual source MP_NoPhysics Debug producer after allocation";return false;}request.disable_physical=disabled;}
  if(request.disable_physical){world_.destroy(native_.body);primary_=secondary_=nullptr;phase_=4;return true;}
  // The native body above is the sole physical backing assigned to this owner.
  phase_=3;
  if(!services_.update_pf||services_.update_pf(services_.context,object_.identity,&native_,&projection_,error_)){
   if(error_.empty())error_="Required source Character::UpdatePFObject tail";return false;
  }
  phase_=4;return true;
 }catch(const std::exception& e){error_=e.what();return false;}
}
bool CharacterWorldNpcPhysicalV1::release(){
 error_.clear();try{if(native_.body)world_.destroy(native_.body);primary_=secondary_=nullptr;phase_=0;return true;}
 catch(const std::exception& e){error_=e.what();return false;}
}
bool CharacterWorldNpcPhysicalV1::enable_filter(){
 error_.clear();try{
  if(filter_disabled_){
   for(auto* shape:{primary_,secondary_})if(shape){
    if(!world_.backend()||shape->GetBody()!=native_.body){error_="Required actual source shape/world filter owner";return false;}
    shape->SetFilterData(saved_filter_);world_.backend()->Refilter(shape);
   }
  }
  filter_disabled_=0;return true;
 }catch(const std::exception& e){error_=e.what();return false;}
}
bool CharacterWorldNpcPhysicalV1::disable_filter(){
 error_.clear();try{
  if(!filter_disabled_){
   b2FilterData zero;zero.groupIndex=0;zero.categoryBits=0;zero.maskBits=0;
   for(auto* shape:{primary_,secondary_})if(shape){
    if(!world_.backend()||shape->GetBody()!=native_.body){error_="Required actual source shape/world filter owner";return false;}
    shape->SetFilterData(zero);world_.backend()->Refilter(shape);
   }
  }
  filter_disabled_=1;return true;
 }catch(const std::exception& e){error_=e.what();return false;}
}
}
