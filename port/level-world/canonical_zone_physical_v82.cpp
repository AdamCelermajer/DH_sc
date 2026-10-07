#include "canonical_zone_physical_v82.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
namespace dh2::world {
CanonicalZonePhysicalV82::CanonicalZonePhysicalV82(CanonicalGameObjectBaseOwnerV1& base,
 physical::NativeWorld& world,std::weak_ptr<void> parent,CanonicalZonePhysicalServicesV82 services)
 :base_(base),world_(world),parent_(std::move(parent)),services_(std::move(services)),owner8_(base.identity()){
 transport_.context=this;transport_.test=test;transport_.contact=contact;transport_.velocity=velocity;
}
CanonicalZonePhysicalV82::~CanonicalZonePhysicalV82(){
 // Qualified source release is required before PhysicalWorld teardown. If the
 // backend already destroyed every body, the host facet is merely retired.
 if(native_.body&&world_.backend())world_.destroy(native_.body);
 native_.body=nullptr;
 if(!parent_.expired()){auto* slot=base_.pointer(0x2dc);if(slot&&*slot==reinterpret_cast<std::uintptr_t>(this))*slot=0;}
}
bool CanonicalZonePhysicalV82::missing(const char* operation,std::string& e){
 error_=std::string("Required actual POZone resource: ")+operation;e=error_;return false;
}
bool CanonicalZonePhysicalV82::live(std::string& e)const{
 if(parent_.expired()||!services_.owner){e="Released SAME POZone parent/service authority";return false;}return true;
}
bool CanonicalZonePhysicalV82::debug(const char* name,bool& value,std::string& e){
 if(!live(e)||!services_.debug_switch)return missing("source Debug load/switch",e);
 return services_.debug_switch(name,value,e);
}
bool CanonicalZonePhysicalV82::construct(bool trigger,std::string& e){
 if(construction_attempted_||released_)return missing("fresh constructor; failed prefix cannot replay",e);
 construction_attempted_=true;
 if(!live(e)||!world_.backend()||!owner8_)return missing("SAME App physical world and parent",e);
 bool no_collisions{};if(!debug("MP_NoCollisions",no_collisions,e))return false;
 const auto* box=base_.absolute_aabb12c();const auto* position=base_.vector3(0x160);
 if(!box||!position)return missing("actual absolute12c/138 and position160",e);
 const float width=(box[3]-box[0])*0.01f,height=(box[4]-box[1])*0.01f;
 // Explicit native admission guard before the old Box2D polygon assertion.
 if(!std::isfinite(width)||!std::isfinite(height)||width<=0||height<=0||
    !std::isfinite(position[0])||!std::isfinite(position[1]))return missing("positive finite source sensor bounds",e);
 // PhysicalObjectC2(46f2f0): flat=false => polygon; static=true gives density0;
 // sensor=true, bullet=false, group=-5, category800, mask trigger?4:51e.
 config_={};config_.enabled=1;
 config_.body.user_data=this;config_.body.position[0]=position[0]*0.01f;config_.body.position[1]=position[1]*0.01f;
 config_.body.allow_sleep=1;config_.body.is_sleeping=1;config_.body.fixed_rotation=1;
 config_.shape.user_data=this;config_.shape.kind=1;config_.shape.sensor=1;config_.shape.friction=1;
 config_.shape.group_index=no_collisions?-666:-5;config_.shape.category_bits=0x800;config_.shape.mask_bits=trigger?4u:0x51eu;
 config_.shape.vertex_count=4;const float x=width*0.5f,y=height*0.5f;
 const float vertices[]{-x,-y,x,-y,x,y,-x,y};std::copy_n(vertices,8,config_.shape.vertices);
 // Source comparison at46f4d8 is __aeabi_fcmplt: the selected value is max.
 config_.radius=(width<height?height:width)*0.5f;
 saved_filter20_v105_.groupIndex=static_cast<int16>(config_.shape.group_index);
 saved_filter20_v105_.categoryBits=static_cast<uint16>(config_.shape.category_bits);
 saved_filter20_v105_.maskBits=static_cast<uint16>(config_.shape.mask_bits);
 native_.radius=config_.radius;native_.body=world_.create_character(config_,&transport_);
 if(!native_.body)return missing("genuine Box2D body/shape construction",e);
 primary18_v105_=native_.body->GetShapeList();secondary1c_v105_=nullptr;
 constructed_=true;e.clear();return true;
}
bool CanonicalZonePhysicalV82::source_filter_borrow_v105(physical::NativePhysicalFilterBorrowV1& out,std::string& e){
 if(!live(e)||!constructed_||!assigned_||released_||!native_.body||!base_.pointer(0x2dc)||*base_.pointer(0x2dc)!=reinterpret_cast<std::uintptr_t>(this))return missing("actual assigned filter20/shape18/1c/disabled26",e);
 out={&world_,&native_,&primary18_v105_,&secondary1c_v105_,&saved_filter20_v105_,&disabled26_};return true;
}
bool CanonicalZonePhysicalV82::assign(std::string& e){
 if(!constructed_||!native_.body||assignment_attempted_||released_)return missing("completed fresh body before assignment",e);
 assignment_attempted_=true;bool no_physics{};if(!debug("MP_NoPhysics",no_physics,e))return false;
 if(no_physics){world_.destroy(native_.body);released_=true;e.clear();return true;}
 auto* slot=base_.pointer(0x2dc);if(!slot)return missing("SAME physical2dc assignment slot",e);
 const auto identity=reinterpret_cast<std::uintptr_t>(this);
 if(*slot!=identity){
  if(*slot){if(!services_.destroy_previous)return missing("previous PhysicalObject D0",e);
   if(!services_.destroy_previous(*slot,e))return false;*slot=0;}
  *slot=identity;
 }
 assigned_=true;
 if(!services_.update_pf)return missing("actual UpdatePFObject after assignment",e);
 return services_.update_pf(base_,e);
}
bool CanonicalZonePhysicalV82::peer(void* address,std::uintptr_t& resolved,std::string& e){
 resolved=0;std::uintptr_t owner{};
 if(!live(e)||!services_.peer_owner||!services_.peer_owner(address,owner,e))return missing("typed physical peer owner8",e);
 if(!owner)return missing("nonnull peer ObjectBase before GetHandle",e);
 if(!services_.resolve_peer_handle)return missing("same-world GetHandle/GameObject resolution",e);
 return services_.resolve_peer_handle(owner,resolved,e);
}
unsigned CanonicalZonePhysicalV82::test(void* raw,void* peer_address,const physical::Filter* a,const physical::Filter* b){
 auto& self=*static_cast<CanonicalZonePhysicalV82*>(raw);
 if(!a||!b||!self.live(self.error_))throw std::runtime_error(self.error_.empty()?"Required POZone filter delivery":self.error_);
 const auto* visible=self.base_.byte(0x80);if(!visible)throw std::runtime_error("Required source POZone owner80");
 if(!*visible)return 0;
 std::uintptr_t owner{};
 if(!self.services_.peer_owner||!self.services_.peer_owner(peer_address,owner,self.error_))throw std::runtime_error(self.error_);
 if(owner){std::uint8_t value{};
  if(!self.services_.peer_visible80||!self.services_.peer_visible80(owner,value,self.error_))throw std::runtime_error(self.error_);
  if(!value)return 0;
 }
 const bool accepted=a->group&&a->group==b->group?a->group>0:(b->category&a->mask)&&(b->mask&a->category);
 if(!accepted)return 0;
 std::uintptr_t object{};if(!self.peer(peer_address,object,self.error_))throw std::runtime_error(self.error_);
 if(!object)return 1; // original preserves base-test result on NULL handle
 if(!self.services_.collision_test_notification)throw std::runtime_error("Required actual Zone void virtualc8");
 if(!self.services_.collision_test_notification(self.base_,object,self.error_))throw std::runtime_error(self.error_);
 return 1;
}
void CanonicalZonePhysicalV82::contact(void* raw,physical::ContactEvent event,void* peer_address,const float*,unsigned){
 auto& self=*static_cast<CanonicalZonePhysicalV82*>(raw);std::uintptr_t object{};
 if(!self.peer(peer_address,object,self.error_))throw std::runtime_error(self.error_);
 if(!object)return;
 if(!self.services_.contact||!self.services_.contact(self.base_,event,object,self.error_))
  throw std::runtime_error(self.error_.empty()?"Required actual Zone contact virtual":self.error_);
}
void CanonicalZonePhysicalV82::velocity(void* raw,float* out){
 auto& self=*static_cast<CanonicalZonePhysicalV82*>(raw);physical::NativeBodyObservation value{};
 if(!out||dh2_native_body_observe(&value,&self.native_))throw std::runtime_error("Required actual POZone velocity");
 out[0]=value.linear_velocity[0]*100.f;out[1]=value.linear_velocity[1]*100.f;
}
bool CanonicalZonePhysicalV82::physical_contact(navigation::PhysicalContact& out,std::string& e){
 if(!live(e))return false;const auto* visible=base_.byte(0x80);if(!visible)return missing("source owner80",e);
 out={};out.present=native_.body!=nullptr;out.disabled=disabled26_;out.owner_present=1;out.owner_enabled=*visible!=0;
 if(native_.body&&native_.body->GetShapeList()){const auto filter=native_.body->GetShapeList()->GetFilterData();
  out.secondary={filter.groupIndex,filter.categoryBits,filter.maskBits,1};}
 e.clear();return true;
}
bool CanonicalZonePhysicalV82::borrow_native(std::shared_ptr<void>& pin,physical::NativeBody*& out,std::string& e){
 pin=parent_.lock();out=nullptr;const auto* slot=pin?base_.pointer(0x2dc):nullptr;
 if(!pin||!assigned_||released_||!native_.body||!slot||*slot!=reinterpret_cast<std::uintptr_t>(this))return missing("SAME assigned Zone physical facet",e);
 out=&native_;e.clear();return true;
}
bool CanonicalZonePhysicalV82::release(std::string& e){
 if(native_.body&&!world_.backend())return missing("body release before PhysicalWorld teardown",e);
 if(native_.body)world_.destroy(native_.body);
 if(!parent_.expired()){auto* slot=base_.pointer(0x2dc);if(slot&&*slot==reinterpret_cast<std::uintptr_t>(this))*slot=0;}
 assigned_=false;released_=true;e.clear();return true;
}
}
