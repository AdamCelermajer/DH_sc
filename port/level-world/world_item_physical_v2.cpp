#include "world_item_physical_v2.hpp"
#include <algorithm>
#include <stdexcept>
namespace dh2::character {
WorldItemPhysicalV2::WorldItemPhysicalV2(RetainedWorldItemObjectV1& item,physical::NativeWorld& world,WorldItemPhysicalServicesV2 s):item_(item),world_(world),services_(std::move(s)){
 transport_.context=this;transport_.test=test;transport_.contact=contact;transport_.velocity=velocity;
}
WorldItemPhysicalV2::~WorldItemPhysicalV2(){if(native_.body)world_.destroy(native_.body);auto* pointer=item_.base().pointer(0x2dc);if(pointer&&*pointer==reinterpret_cast<std::uintptr_t>(this))*pointer=0;}
bool WorldItemPhysicalV2::missing(const char* name,std::string& e){e=std::string("Required actual POItem ")+name;error_=e;return false;}
bool WorldItemPhysicalV2::debug(const char* name,bool& value,std::string& e){if(!services_.owner||!services_.debug)return missing("shared Debug owner",e);return services_.debug(name,value,e);}
bool WorldItemPhysicalV2::construct(std::string& e){
 if(native_.body||assigned_||!world_.backend())return missing("fresh receiver / same PhysicalWorld",e);
 bool disabled{};if(!debug("MP_NoCollisions",disabled,e))return false;
 const auto* bounds=item_.base().absolute_aabb12c();const auto* position=item_.base().vector3(0x160);
 if(!bounds||!position)return missing("same absolute bounds/position",e);
 physical::ItemBodyInputV2 input;input.physical=this;input.no_collisions=disabled;
 input.absolute_bounds[0]=bounds[0];input.absolute_bounds[1]=bounds[1];input.absolute_bounds[2]=bounds[3];input.absolute_bounds[3]=bounds[4];
 std::copy_n(position,2,input.position);
 if(!physical::item_body_config_v2(config_,input))return missing("source circle constructor definitions",e);
 native_.radius=config_.radius;native_.body=world_.create_character(config_,&transport_);
 if(!native_.body)return missing("real Box2D body/shape allocation",e);secondary_shape_=native_.body->GetShapeList();return true;
}
bool WorldItemPhysicalV2::assign(std::string& e){
 if(!native_.body)return missing("completed PhysicalObjectC2 before assignment",e);
 bool disabled{};if(!debug("MP_NoPhysics",disabled,e))return false;
 if(disabled){world_.destroy(native_.body);secondary_shape_=nullptr;return true;}// Source deletes new and preserves old pointer.
 auto* pointer=item_.base().pointer(0x2dc);if(!pointer)return missing("canonical physical2dc field",e);
 const auto identity=reinterpret_cast<std::uintptr_t>(this);
 if(*pointer!=identity){if(*pointer){if(!services_.destroy_previous)return missing("previous physical destructor",e);if(!services_.destroy_previous(*pointer,e))return false;}*pointer=identity;}
 assigned_=true;
 if(!services_.update_pf)return missing("same GameObject UpdatePFObject",e);return services_.update_pf(e);
}
bool WorldItemPhysicalV2::detach(std::string& e){
 bool disabled{};if(!debug("MP_NoPhysics",disabled,e))return false;if(disabled)return true;
 auto* pointer=item_.base().pointer(0x2dc);if(!pointer)return missing("canonical physical2dc field",e);
 if(*pointer){if(*pointer==reinterpret_cast<std::uintptr_t>(this))world_.destroy(native_.body);
  else{if(!services_.destroy_previous)return missing("previous physical destructor",e);if(!services_.destroy_previous(*pointer,e))return false;}*pointer=0;}
 secondary_shape_=nullptr;assigned_=false;if(!services_.update_pf)return missing("same GameObject UpdatePFObject",e);return services_.update_pf(e);
}
unsigned WorldItemPhysicalV2::test(void* p,void* peer,const physical::Filter* a,const physical::Filter* b){
 auto& t=*static_cast<WorldItemPhysicalV2*>(p);if(!a||!b)throw std::runtime_error("Required actual POItem filters");
 auto* visible=t.item_.base().byte(0x80);if(!visible)throw std::runtime_error("Required actual Item visible80");if(!*visible)return 0;
 std::uintptr_t other{};std::uint8_t other_visible{};
 if(!t.services_.peer_owner||!t.services_.peer_owner(peer,other,t.error_))throw std::runtime_error(t.error_.empty()?"Required actual POItem peer owner":t.error_);
 if(other){if(!t.services_.peer_visible||!t.services_.peer_visible(other,other_visible,t.error_))throw std::runtime_error(t.error_.empty()?"Required actual POItem peer visible80":t.error_);if(!other_visible)return 0;}
 if(a->group&&a->group==b->group)return a->group>0;
 return (b->category&a->mask)&&(b->mask&a->category);
}
bool WorldItemPhysicalV2::collision(physical::ContactEvent event,void* peer,std::string& e){
 if(event==physical::ContactEvent::persist||event==physical::ContactEvent::result)return true;// Actual4701c4/3883d0 bx lr.
 if(event!=physical::ContactEvent::add&&event!=physical::ContactEvent::remove)return missing("original collision operation",e);
 std::uintptr_t self{};std::uint32_t type{};
 if(!services_.resolve)return missing("same Item GetHandle/GetObject(false)",e);
 if(!services_.resolve(item_.base().identity(),self,type,e))return false;
 if(!self||type!=3)return true;
 if(self!=item_.base().identity())return missing("resolved canonical Item identity",e);
 if(!peer)return true;
 std::uintptr_t object{};if(!services_.peer_owner)return missing("same peer PhysicalObject+8",e);
 if(!services_.peer_owner(peer,object,e))return false;if(!object)return true;
 bool interactive{};if(!item_.is_interactive(interactive,e))return false;if(!interactive)return true;
 std::uintptr_t character{};if(!services_.as_character)return missing("actual peer Handle Character cast",e);
 if(!services_.as_character(object,character,e))return false;if(!character)return true;
 std::uintptr_t ooi{};if(!services_.character_ooi)return missing("same Character+14a4 producer",e);
 if(!services_.character_ooi(character,ooi,e))return false;
 return event==physical::ContactEvent::remove?item_.collision_end_v2(character,ooi,e):item_.collision(character,ooi,e);
}
void WorldItemPhysicalV2::contact(void* p,physical::ContactEvent event,void* peer,const float*,unsigned){auto& t=*static_cast<WorldItemPhysicalV2*>(p);if(!t.collision(event,peer,t.error_))throw std::runtime_error(t.error_);}
void WorldItemPhysicalV2::velocity(void* p,float* out){auto& t=*static_cast<WorldItemPhysicalV2*>(p);physical::NativeBodyObservation observed{};if(!out||dh2_native_body_observe(&observed,&t.native_))throw std::runtime_error("Required actual POItem body velocity");out[0]=observed.linear_velocity[0]*100.f;out[1]=observed.linear_velocity[1]*100.f;}
}
