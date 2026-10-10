#include "canonical_podecor_body_v49.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
namespace dh2::world {
CanonicalPodDecorBodyV49::CanonicalPodDecorBodyV49(CanonicalGameObjectBaseOwnerV1& b,
 RetainedGameObjectVisualV1& v,physical::NativeWorld& w,RetainedGameObjectDecorServicesV1 s)
 :CanonicalPodDecorBodyV49(b,w,std::move(s)){visual_=&v;}
CanonicalPodDecorBodyV49::CanonicalPodDecorBodyV49(CanonicalGameObjectBaseOwnerV1& b,
 physical::NativeWorld& w,RetainedGameObjectDecorServicesV1 s)
 :base_(b),world_(w),services_(std::move(s)),owner8_(b.identity()){
 transport_.context=this;transport_.test=test;transport_.contact=contact;transport_.velocity=velocity;
}
CanonicalPodDecorBodyV49::~CanonicalPodDecorBodyV49(){
 if(native_.body)world_.destroy(native_.body);
 auto* slot=base_.pointer(0x2dc);if(slot&&*slot==reinterpret_cast<std::uintptr_t>(this))*slot=0;
}
bool CanonicalPodDecorBodyV49::missing(const char* name,std::string& e){
 e=std::string("Required actual PODecor V49 ")+name;error_=e;return false;
}
bool CanonicalPodDecorBodyV49::debug(const char* key,bool& value,std::string& e){
 if(!services_.owner||!services_.debug_switch)return missing("SAME Debug owner",e);
 return services_.debug_switch(key,value,e);
}
bool CanonicalPodDecorBodyV49::construct(std::string& e){
 if(construct_attempted_||native_.body)return missing("fresh source constructor prefix; no retry",e);
 construct_attempted_=true;
 if(!owner8_||!visual_||!visual_->ready()||!visual_->root_identity()||!visual_->marker().found||!world_.backend())
  return missing("actual selected parent/visual colbox/PhysicalWorld",e);
 const auto* position=base_.vector3(0x160);const auto* flat=base_.byte(0x15c);
 if(!position||!flat)return missing("SAME owner placement/flat source slots",e);
 bool no_collisions{};if(!debug("MP_NoCollisions",no_collisions,e))return false;
 physical::DecorBodyInput input{};input.owner=reinterpret_cast<void*>(owner8_);input.new_physical=this;
 input.visual_present=1;input.colbox_found=1;input.collision_group_override=no_collisions;input.previous_flat=*flat;
 std::copy_n(visual_->mesh_box().data(),6,input.mesh_box);std::copy_n(position,3,input.position);
 if(dh2_decor_body_config(&config_,&input))return missing("source polygon definitions",e);
 saved_filter20_v105_.groupIndex=static_cast<int16>(config_.physical.shape.group_index);
 saved_filter20_v105_.categoryBits=static_cast<uint16>(config_.physical.shape.category_bits);
 saved_filter20_v105_.maskBits=static_cast<uint16>(config_.physical.shape.mask_bits);
 native_.radius=config_.physical.radius;native_.body=world_.create_character(config_.physical,&transport_);
 if(!native_.body)return missing("actual Box2D body/shape",e);
 primary18_v105_=native_.body->GetShapeList();secondary1c_v105_=nullptr;
 constructed_=true;return true; // source2dc remains unchanged until outer assignment
}
bool CanonicalPodDecorBodyV49::construct_door(std::string& e){
 if(construct_attempted_||native_.body)return missing("fresh Door PhysicalObject constructor prefix; no retry",e);
 construct_attempted_=true;
 if(!owner8_||visual_||!world_.backend())return missing("Door direct PhysicalObject C2 parent/PhysicalWorld",e);
 bool no_collisions{};if(!debug("MP_NoCollisions",no_collisions,e))return false;
 const auto* box=base_.absolute_aabb12c();const auto* position=base_.vector3(0x160);
 if(!box||!position)return missing("SAME Door absolute12c/138 and position160",e);
 const float width=(box[3]-box[0])*0.01f,height=(box[4]-box[1])*0.01f;
 // Native admission guard before Box2D SetAsBox's positive-extent assertion.
 if(!std::isfinite(width)||!std::isfinite(height)||width<=0||height<=0||
    !std::isfinite(position[0])||!std::isfinite(position[1]))return missing("positive finite Door polygon bounds",e);
 // PhysicalObject C2(world,THIS,true,false,false,false,0,2,ffff,1):
 // static polygon, zero density, solid primary shape, no bullet or pin.
 auto& physical=config_.physical;physical={};physical.enabled=1;
 physical.body.user_data=this;physical.body.position[0]=position[0]*0.01f;physical.body.position[1]=position[1]*0.01f;
 physical.body.allow_sleep=1;physical.body.is_sleeping=1;physical.body.fixed_rotation=1;
 physical.shape.user_data=this;physical.shape.kind=1;physical.shape.friction=1;
 physical.shape.group_index=no_collisions?-666:0;physical.shape.category_bits=2;physical.shape.mask_bits=0xffff;
 physical.shape.vertex_count=4;const float x=width*0.5f,y=height*0.5f;
 const float vertices[]{-x,-y,x,-y,x,y,-x,y};std::copy_n(vertices,8,physical.shape.vertices);
 physical.radius=(width<height?height:width)*0.5f;
 saved_filter20_v105_.groupIndex=static_cast<int16>(physical.shape.group_index);
 saved_filter20_v105_.categoryBits=static_cast<uint16>(physical.shape.category_bits);
 saved_filter20_v105_.maskBits=static_cast<uint16>(physical.shape.mask_bits);
 native_.radius=physical.radius;native_.body=world_.create_character(physical,&transport_);
 if(!native_.body)return missing("actual Door Box2D body/shape",e);
 primary18_v105_=native_.body->GetShapeList();secondary1c_v105_=nullptr;
 constructed_=true;e.clear();return true;
}
bool CanonicalPodDecorBodyV49::source_filter_borrow_v105(physical::NativePhysicalFilterBorrowV1& out,std::string& e){
 const auto* slot=base_.pointer(0x2dc);
 if(!constructed_||!assigned_||!native_.body||!slot||*slot!=reinterpret_cast<std::uintptr_t>(this))return missing("same assigned filter20/shapes18/1c/disabled26",e);
 out={&world_,&native_,&primary18_v105_,&secondary1c_v105_,&saved_filter20_v105_,&disabled26_};return true;
}
bool CanonicalPodDecorBodyV49::assign(bool pin,std::string& e){
 if(!constructed_||!native_.body||assignment_attempted_)return missing("completed constructor/unreplayed assignment",e);
 if(pin)return missing("positive source pin branch; Decor passes false",e);
 assignment_attempted_=true;bool no_physics{};if(!debug("MP_NoPhysics",no_physics,e))return false;
 if(no_physics){world_.destroy(native_.body);return true;} // old source2dc preserved
 auto* slot=base_.pointer(0x2dc);if(!slot)return missing("SAME parent physical2dc slot",e);
 const auto identity=reinterpret_cast<std::uintptr_t>(this);
 if(*slot!=identity){
  if(*slot){if(!services_.destroy_previous)return missing("previous PhysicalObject destructor",e);
   if(!services_.destroy_previous(*slot,e))return false;*slot=0;}
  *slot=identity;
 }
 assigned_=true;
 if(!services_.update_pf)return missing("actual GameObject UpdatePFObject after assignment",e);
 return services_.update_pf(e);
}
bool CanonicalPodDecorBodyV49::release(std::string& e){
 if(native_.body&&!world_.backend())return missing("body teardown before PhysicalWorld destruction",e);
 if(native_.body)world_.destroy(native_.body);
 auto* slot=base_.pointer(0x2dc);if(slot&&*slot==reinterpret_cast<std::uintptr_t>(this))*slot=0;
 assigned_=false;return true;
}
unsigned CanonicalPodDecorBodyV49::test(void* raw,void* peer,const physical::Filter* a,const physical::Filter* b){
 auto& self=*static_cast<CanonicalPodDecorBodyV49*>(raw);
 if(!a||!b)throw std::runtime_error("Required actual PODecor filters");
 const auto* visible=self.base_.byte(0x80);if(!visible)throw std::runtime_error("Required PODecor source visible80");
 if(!*visible)return 0;
 std::uintptr_t owner{};
 if(!self.services_.peer_owner||!self.services_.peer_owner(peer,owner,self.error_))
  throw std::runtime_error(self.error_.empty()?"Required actual PODecor peer owner8":self.error_);
 if(owner){std::uint8_t other_visible{};
  if(!self.services_.peer_visible80||!self.services_.peer_visible80(owner,other_visible,self.error_))
   throw std::runtime_error(self.error_.empty()?"Required actual PODecor peer source80":self.error_);
  if(!other_visible)return 0;
 }
 if(a->group&&a->group==b->group)return a->group>0;
 return (b->category&a->mask)&&(b->mask&a->category);
}
// PODecor begin470074 and inherited persist/end/result are captured bx lr.
void CanonicalPodDecorBodyV49::contact(void*,physical::ContactEvent,void*,const float*,unsigned){}
void CanonicalPodDecorBodyV49::velocity(void* raw,float* out){
 auto& self=*static_cast<CanonicalPodDecorBodyV49*>(raw);physical::NativeBodyObservation value{};
 if(!out||dh2_native_body_observe(&value,&self.native_))throw std::runtime_error("Required actual PODecor velocity");
 for(unsigned i=0;i<2;++i)out[i]=value.linear_velocity[i]*100.f;
}
bool CanonicalPodDecorBodyV49::physical_contact(navigation::PhysicalContact& out,std::string& e){
 const auto* visible=base_.byte(0x80);if(!visible)return missing("actual contact source80",e);
 navigation::PhysicalContact value{};value.present=native_.body!=nullptr;value.disabled=disabled26_;
 value.owner_present=owner8_!=0;value.owner_enabled=*visible!=0;
 if(native_.body&&native_.body->GetShapeList()){const auto filter=native_.body->GetShapeList()->GetFilterData();
  value.primary={filter.groupIndex,filter.categoryBits,filter.maskBits,1};}
 out=value;return true;
}
}
