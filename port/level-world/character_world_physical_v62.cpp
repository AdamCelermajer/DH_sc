#include "character_world_physical_v62.hpp"
#include <stdexcept>
#include <exception>
namespace dh2::character {
bool CharacterWorldPhysicalV62::borrow_avoidance_contact_v108(std::uint8_t enabled,navigation::PhysicalContact& out,std::string& e)const{
 if(!native_.body||!published_v7_||!object_.identity){e="Required SAME published POCharacter/body for avoidance";return false;}
 navigation::PhysicalContact source{};source.present=1;source.disabled=filter_disabled_;source.owner_present=1;source.owner_enabled=enabled;
 auto filter=[](b2Shape* shape,navigation::ContactFilter& out){if(!shape)return;const auto& f=shape->GetFilterData();out={f.groupIndex,f.categoryBits,f.maskBits,1};};
 filter(primary_,source.primary);filter(secondary_,source.secondary);out=source;e.clear();return true;
}
CharacterWorldPhysicalV62::CharacterWorldPhysicalV62(physical::NativeWorld& w,ScriptCharacterObject& o,data::PropertyView& p,CharacterPhysicalCollisionBorrowV62 c,WorldNpcPhysicalServicesV1 s,std::function<bool(std::uintptr_t,const physical::NativeBody*,bool,std::string&)> assignment):world_(w),collision_(std::move(c)),object_(o),properties_(p),services_(s),source_assignment_(std::move(assignment)){world_object_.context=this;world_object_.test=test;world_object_.contact=contact;world_object_.velocity=velocity;}
CharacterWorldPhysicalV62::~CharacterWorldPhysicalV62(){
 // A callback failure cannot silently leave a dangling WorldObject context.
 // Explicit release exposes its error to the caller before this destructor.
 if(native_.body&&!release())std::terminate();
}
[[noreturn]] void CharacterWorldPhysicalV62::unavailable(const char* message){
 if(error_.empty())error_=message;throw std::runtime_error(error_);
}
unsigned CharacterWorldPhysicalV62::test(void* p,void* other,const physical::Filter* own_filter,const physical::Filter* other_filter){
 auto& t=*static_cast<CharacterWorldPhysicalV62*>(p);t.error_.clear();
 if(!own_filter||!other_filter)t.unavailable("Required actual native shape filters");
 bool allowed{};if(!t.collision_.permits_filter(other_filter->category,allowed)){t.error_=t.collision_.error();t.unavailable("Required POCharacter source filter prefix");}if(!allowed)return 0;
 std::uintptr_t peer{};if(!t.services_.peer_owner||!t.services_.peer_owner(t.services_.context,other,peer,t.error_))t.unavailable("Required actual physical peer owner");
 std::uint8_t enabled{};
 if(!t.services_.enabled80||!t.services_.enabled80(t.services_.context,t.object_.identity,enabled,t.error_))t.unavailable("Required actual ObjectBase+80");if(!enabled)return 0;
 if(peer){if(!t.services_.enabled80(t.services_.context,peer,enabled,t.error_))t.unavailable("Required actual peer ObjectBase+80");if(!enabled)return 0;}
 if(own_filter->group&&own_filter->group==other_filter->group)return own_filter->group>0;
 return (other_filter->category&own_filter->mask)&&(other_filter->mask&own_filter->category);
}
void CharacterWorldPhysicalV62::contact(void* p,physical::ContactEvent event,void* other,const float*,unsigned persist){
 auto& t=*static_cast<CharacterWorldPhysicalV62*>(p);t.error_.clear();
 if(event==physical::ContactEvent::result)return; // Whole POCharacter Result 46fb14: bx lr.
 std::uintptr_t peer{};if(!t.services_.peer_owner||!t.services_.peer_owner(t.services_.context,other,peer,t.error_))t.unavailable("Required actual collision peer owner");
 const auto code=event==physical::ContactEvent::add?WorldNpcPhysicalEventV1::begin:
 event==physical::ContactEvent::persist?WorldNpcPhysicalEventV1::persist:WorldNpcPhysicalEventV1::end;
 if(!t.collision_.physical_event(code,peer,persist)){t.error_=t.collision_.error();t.unavailable("Required actual NPC collision continuation");}
}
void CharacterWorldPhysicalV62::velocity(void* p,float* xy){
 auto& t=*static_cast<CharacterWorldPhysicalV62*>(p);physical::NativeBodyObservation observation{};
 if(!xy||dh2_native_body_observe(&observation,&t.native_))t.unavailable("Required actual physical velocity");
 xy[0]=observation.linear_velocity[0]*100.f;xy[1]=observation.linear_velocity[1]*100.f;
}
bool CharacterWorldPhysicalV62::initialize_source(physical::NpcBodyRequest request,const ProjectionV62& supplied){
 const ProjectionV62* projection=&supplied;const bool source_debug=true;
 error_.clear();
 if(phase_||native_.body||!world_.backend()||request.previous_physical||!object_.properties||
 request.properties!=&properties_||properties_.base!=object_.properties->base.data()||
 properties_.resolved!=object_.properties->resolved.data()||!object_.life||!collision_.receiver){
  error_="NPC physical initialization requires fresh body and same World/script/property/life owner";return false;
 }
 request.owner=reinterpret_cast<void*>(object_.identity);request.new_physical=&native_;
 if(source_debug){request.collision_group_override=0;request.disable_physical=0;}
 auto project=[&](){if(projection){if(!*projection){error_="Required same live Character body projection";return false;}return (*projection)(request,projection_,error_);}error_="Required actual live Character body projection";return false;};
 if(!project())return false;phase_=1;
 try{
  if(projection_.body.enabled){
   if(source_debug){bool override_group{};if(!services_.debug_switch||!services_.debug_switch(services_.context,"MP_NoCollisions",override_group,error_)){if(error_.empty())error_="Required actual source MP_NoCollisions Debug producer";return false;}request.collision_group_override=override_group;if(!project())return false;}
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
  if(native_.body){
   if(!source_assignment_&&!services_.publish_physical_v7){error_="Required SAME Character physical2dc source assignment";return false;}
   const bool assigned=source_assignment_?source_assignment_(reinterpret_cast<std::uintptr_t>(this),&native_,true,error_):services_.publish_physical_v7(services_.context,object_.identity,reinterpret_cast<std::uintptr_t>(this),&native_,true,error_);
   if(!assigned)return false;published_v7_=true;
  }
  if(!services_.update_pf||services_.update_pf(services_.context,object_.identity,&native_,&projection_,error_)){
   if(error_.empty())error_="Required source Character::UpdatePFObject tail";return false;
  }
  phase_=4;return true;
 }catch(const std::exception& e){error_=e.what();return false;}
}
bool CharacterWorldPhysicalV62::release(){
 error_.clear();try{if(native_.body)world_.destroy(native_.body);primary_=secondary_=nullptr;
  if(published_v7_){
   if(!source_assignment_&&!services_.publish_physical_v7){error_="Required same Character physical2dc source detachment";return false;}
   const bool detached=source_assignment_?source_assignment_(reinterpret_cast<std::uintptr_t>(this),&native_,false,error_):services_.publish_physical_v7(services_.context,object_.identity,reinterpret_cast<std::uintptr_t>(this),&native_,false,error_);
   if(!detached)return false;published_v7_=false;
  }
  phase_=0;return true;}
 catch(const std::exception& e){error_=e.what();return false;}
}
bool CharacterWorldPhysicalV62::enable_filter(){
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
bool CharacterWorldPhysicalV62::disable_filter(){
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
bool CharacterWorldPhysicalV62::source_death_filter_v84(std::int16_t group,std::uint16_t category,std::uint16_t mask){
 error_.clear();if(!native_.body){return true;}
 try{if(!primary_||!world_.backend()||primary_->GetBody()!=native_.body){error_="Required SAME primary Character death filter";return false;}
  b2FilterData filter;filter.groupIndex=group;filter.categoryBits=category;filter.maskBits=mask;
  primary_->SetFilterData(filter);world_.backend()->Refilter(primary_);return true;
 }catch(const std::exception& e){error_=e.what();return false;}
}
bool CharacterWorldPhysicalV62::source_reset_filter_v84(){
 return source_death_filter_v84(saved_filter_.groupIndex,saved_filter_.categoryBits,saved_filter_.maskBits);
}
}

