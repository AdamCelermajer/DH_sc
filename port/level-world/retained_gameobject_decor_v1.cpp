#include "retained_gameobject_decor_v1.hpp"
#include <algorithm>
#include <stdexcept>
namespace dh2::world {
RetainedGameObjectDecorV1::RetainedGameObjectDecorV1(CanonicalGameObjectBaseOwnerV1& b,
 RetainedGameObjectVisualV1& v,physical::NativeWorld& w,RetainedGameObjectDecorServicesV1 s)
 :base_(b),visual_(v),world_(w),services_(std::move(s)){
 world_object_.context=this;world_object_.test=test;world_object_.contact=contact;world_object_.velocity=velocity;
}
RetainedGameObjectDecorV1::~RetainedGameObjectDecorV1(){if(native_.body){world_.destroy(native_.body);auto* p=base_.pointer(0x2dc);if(p&&*p==reinterpret_cast<std::uintptr_t>(this))*p=0;}}
bool RetainedGameObjectDecorV1::missing(const char* method,std::string& e)const{e=std::string("Required actual PODecor ")+method;return false;}
bool RetainedGameObjectDecorV1::debug(const char* key,bool& v,std::string& e){if(!services_.owner||!services_.debug_switch)return missing("shared Debug owner",e);return services_.debug_switch(key,v,e);}
unsigned RetainedGameObjectDecorV1::test(void* p,void* peer,const physical::Filter* a,const physical::Filter* b){auto& s=*static_cast<RetainedGameObjectDecorV1*>(p);std::string e;unsigned result=0;
 if(!a||!b)throw std::runtime_error("Required actual PODecor shape filters");
 auto fields=s.base_.properties().fields;std::uint8_t visible=0;
 if(!fields.read_bool||!fields.read_bool(fields.context,0x80,visible,e))throw std::runtime_error(e.empty()?"Required SAME canonical byte80 producer":e);
 if(!visible)return 0;
 std::uintptr_t other=0;
 if(!s.services_.peer_owner||!s.services_.peer_owner(peer,other,e))throw std::runtime_error(e.empty()?"Required actual PhysicalObject peer owner":e);
 if(other){if(!s.services_.peer_visible80||!s.services_.peer_visible80(other,visible,e))throw std::runtime_error(e.empty()?"Required canonical peer byte80 producer":e);if(!visible)return 0;}
 if(a->group&&a->group==b->group)result=a->group>0;
 else result=(b->category&a->mask)&&(b->mask&a->category);
 return result;
}
// Actual PODecorBegin470074 and inherited PhysicalObject Persist3883c8,
// End3883cc/Result3883d0 are independently captured literal bx lr methods.
void RetainedGameObjectDecorV1::contact(void*,physical::ContactEvent,void*,const float*,unsigned){}
void RetainedGameObjectDecorV1::velocity(void* p,float* out){auto& s=*static_cast<RetainedGameObjectDecorV1*>(p);physical::NativeBodyObservation o{};
 if(dh2_native_body_observe(&o,&s.native_))throw std::runtime_error("Required live PODecor body velocity");for(unsigned i=0;i<2;++i)out[i]=o.linear_velocity[i]*100.f;
}
bool RetainedGameObjectDecorV1::initialize(std::string& e){
 if(native_.body||assigned_)return missing("fresh constructor receiver",e);
 if(!visual_.ready())return missing("completed SAME visual constructor",e);
 if(!visual_.root_identity()||!visual_.marker().found)return true; // original no colbox -> no PODecor
 if(!world_.backend())return missing("loaded SAME PhysicalWorld",e);
 auto* position=base_.vector3(0x160);auto* flat=base_.byte(0x15c);auto* pointer=base_.pointer(0x2dc);
 if(!position||!flat||!pointer)return missing("SAME parent body/bounds storage",e);
 bool no_collisions=false;if(!debug("MP_NoCollisions",no_collisions,e))return false;
 physical::DecorBodyInput input{};input.owner=reinterpret_cast<void*>(base_.identity());input.new_physical=this;
 input.previous_physical=reinterpret_cast<void*>(*pointer);input.visual_present=1;input.colbox_found=1;
 input.collision_group_override=no_collisions;input.previous_flat=*flat;
 std::copy_n(visual_.mesh_box().data(),6,input.mesh_box);std::copy_n(position,3,input.position);
 if(dh2_decor_body_config(&config_,&input))return missing("original polygon definitions",e);
 saved_filter20_v105_.groupIndex=static_cast<int16>(config_.physical.shape.group_index);
 saved_filter20_v105_.categoryBits=static_cast<uint16>(config_.physical.shape.category_bits);
 saved_filter20_v105_.maskBits=static_cast<uint16>(config_.physical.shape.mask_bits);
 native_.radius=config_.physical.radius;
 native_.body=world_.create_character(config_.physical,&world_object_);
 if(!native_.body)return missing("actual Box2D body/shape allocation",e);
 primary18_v105_=native_.body->GetShapeList();secondary1c_v105_=nullptr;
 bool no_physics=false;if(!debug("MP_NoPhysics",no_physics,e))return false;
 if(no_physics){world_.destroy(native_.body);return true;} // preserves previous field exactly
 if(*pointer&&*pointer!=reinterpret_cast<std::uintptr_t>(this)){
  if(!services_.destroy_previous)return missing("previous PhysicalObject destructor",e);
  if(!services_.destroy_previous(*pointer,e))return false;*pointer=0;
 }
 *pointer=reinterpret_cast<std::uintptr_t>(this);assigned_=true;
 if(!services_.update_pf)return missing("SAME GameObject UpdatePFObject",e);
 return services_.update_pf(e); // source false pin argument: no Pin call
}
bool RetainedGameObjectDecorV1::source_filter_borrow_v105(physical::NativePhysicalFilterBorrowV1& out,std::string& e){
 const auto* slot=base_.pointer(0x2dc);
 if(!assigned_||!native_.body||!slot||*slot!=reinterpret_cast<std::uintptr_t>(this))return missing("same assigned filter20/shapes18/1c/disabled26",e);
 out={&world_,&native_,&primary18_v105_,&secondary1c_v105_,&saved_filter20_v105_,&disabled26_v105_};return true;
}
bool RetainedGameObjectDecorV1::detach(std::string& e){
 bool disabled=false;if(!debug("MP_NoPhysics",disabled,e))return false;if(disabled)return true;
 auto* p=base_.pointer(0x2dc);if(!p)return missing("SAME physical assignment",e);
 if(*p){if(*p==reinterpret_cast<std::uintptr_t>(this)){world_.destroy(native_.body);assigned_=false;}
  else {if(!services_.destroy_previous)return missing("previous PhysicalObject destructor",e);if(!services_.destroy_previous(*p,e))return false;}
  *p=0;
 }
 if(!services_.update_pf)return missing("SAME GameObject UpdatePFObject",e);return services_.update_pf(e);
}
bool RetainedGameObjectDecorV1::release(std::string& e){
 if(native_.body&&!world_.backend())return missing("body lifetime preceding World teardown",e);
 if(native_.body)world_.destroy(native_.body);auto* p=base_.pointer(0x2dc);
 if(p&&*p==reinterpret_cast<std::uintptr_t>(this))*p=0;assigned_=false;return true;
}
}
