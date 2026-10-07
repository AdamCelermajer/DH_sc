#include "canonical_projectile_physical_v112.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace dh2::world {
CanonicalProjectilePhysicalV112::CanonicalProjectilePhysicalV112(
 CanonicalGameObjectBaseOwnerV1& base,std::shared_ptr<void> receiver,
 physical::NativeWorld& world,CanonicalObjectManagerV1& objects,
 character::LootPhysicalAssociationsV49& associations,ProjectilePhysicalServicesV112 services)
 :base_(base),receiver_(std::move(receiver)),world_(world),objects_(objects),
  associations_(associations),services_(std::move(services)),owner8_(base.identity()){
 transport_={this,test,contact,velocity,{0,0},0,0};
}
CanonicalProjectilePhysicalV112::~CanonicalProjectilePhysicalV112()=default;
bool CanonicalProjectilePhysicalV112::fail(std::string& e,const char* reason){
 if(first_failure_.empty())first_failure_=e.empty()?reason:e;e=first_failure_;return false;
}
bool CanonicalProjectilePhysicalV112::resolve(std::uintptr_t& id,std::uint32_t& type,std::string& e){
 auto handle=base_.shared_handle();const CanonicalObjectBorrowV1* object{};
 if(!objects_.resolve_handle_v4(handle,false,object,{},e))return false;
 id=object?object->identity:0;type=object&&object->type_f4?*object->type_f4:UINT32_MAX;
 if(object&&(!object->lease||!object->type_f4)){e="Required real POProjectile handle/type receiver";return false;}
 e.clear();return true;
}
unsigned CanonicalProjectilePhysicalV112::test(void* raw,void* peer,
 const physical::Filter* own_filter,const physical::Filter* peer_filter){
 auto& self=*static_cast<CanonicalProjectilePhysicalV112*>(raw);std::string e;
 if(!own_filter||!peer_filter||!self.services_.peer_owner||!self.services_.peer_visible80)
  throw std::runtime_error("Required actual POProjectile filter/peer transport");
 const auto visible=self.base_.byte(0x80);
 if(!visible)throw std::runtime_error("Required produced POProjectile parent visible80");
 if(!*visible)return 0;
 std::uintptr_t other{};if(!self.services_.peer_owner(peer,other,e))throw std::runtime_error(e);
 if(other){std::uint8_t enabled{};if(!self.services_.peer_visible80(other,enabled,e))throw std::runtime_error(e);if(!enabled)return 0;}
 const bool accepts=own_filter->group&&own_filter->group==peer_filter->group?
  own_filter->group>0:(own_filter->mask&peer_filter->category)&&(peer_filter->mask&own_filter->category);
 if(!accepts)return 0;
 std::uintptr_t projectile{};std::uint32_t type{};
 if(!self.resolve(projectile,type,e))throw std::runtime_error(e);
 //Original first9 probe then fresh10 probe on mismatch (470594..5c0).
 if(!projectile||type!=9)if(!self.resolve(projectile,type,e))throw std::runtime_error(e);
 if(!projectile||(type!=9&&type!=10)||!peer)return 0;
 const auto position=self.base_.vector3(0x160);
 if(!position||!self.services_.on_collision||!self.services_.on_collision(other,position,e))
  throw std::runtime_error(e.empty()?"Required POProjectile selected virtuald0":e);
 //Source filter delivers collision synchronously but always rejects physical
 //response. It never substitutes a hit during the later world contact phase.
 return 0;
}
void CanonicalProjectilePhysicalV112::contact(void* raw,physical::ContactEvent event,
 void* peer,const float* point,unsigned){
 auto& self=*static_cast<CanonicalProjectilePhysicalV112*>(raw);
 //Persists virtual tail-calls Begins; End/Result inherit literal BX LR.
 if(event!=physical::ContactEvent::add&&event!=physical::ContactEvent::persist)return;
 std::string e;std::uintptr_t projectile{};std::uint32_t type{};
 if(!self.resolve(projectile,type,e))throw std::runtime_error(e);
 if(!projectile||type!=9||!peer)return;
 std::uintptr_t other{};
 if(!point||!self.services_.peer_owner||!self.services_.peer_owner(peer,other,e)||
    !self.services_.on_collision||!self.services_.on_collision(other,point,e))
  throw std::runtime_error(e.empty()?"Required same POProjectile Begin/Persist":e);
}
void CanonicalProjectilePhysicalV112::velocity(void* raw,float* out){
 auto& self=*static_cast<CanonicalProjectilePhysicalV112*>(raw);physical::NativeBodyObservation observed{};
 if(!out||dh2_native_body_observe(&observed,&self.body_))throw std::runtime_error("Required real POProjectile velocity");
 out[0]=observed.linear_velocity[0]*100.f;out[1]=observed.linear_velocity[1]*100.f;
}
bool CanonicalProjectilePhysicalV112::initialize(std::string& e){
 if(construction_started_)return fail(e,"POProjectile C1 cannot replay reached allocation prefix");
 construction_started_=true;
 if(!receiver_||!services_.owner||!services_.debug_switch||!world_.backend()||
    (base_.type_f4()!=9&&base_.type_f4()!=10))return fail(e,"Required actual Projectile/PhysicalWorld C1 owners");
 //Source PhysicalObjectC1 reads real MP_NoCollisions before creating shape.
 bool no_collisions{};if(!services_.debug_switch("MP_NoCollisions",no_collisions,e))return fail(e,"POProjectile Debug delivery");
 physical::CharacterBodyConfig config{};config.enabled=1;config.po_character=0;
 config.body.user_data=this;const auto position=base_.vector3(0x160),bounds=base_.absolute_aabb12c();
 config.body.position[0]=position[0]*0.01f;config.body.position[1]=position[1]*0.01f;
 config.body.allow_sleep=1;config.body.is_sleeping=1;config.body.fixed_rotation=1;config.body.bullet=1;
 config.shape.user_data=this;config.shape.kind=0;config.shape.sensor=1;config.shape.friction=1.f;
 const std::uint32_t density=0x4133d70a;std::memcpy(&config.shape.density,&density,4);
 const float width=(bounds[3]-bounds[0])*0.01f,height=(bounds[4]-bounds[1])*0.01f;
 config.radius=(width<height?height:width)*0.5f;config.shape.radius=config.radius;
 config.shape.group_index=no_collisions?-666:0;config.shape.category_bits=32;config.shape.mask_bits=0x51f;
 saved20_.groupIndex=static_cast<int16>(config.shape.group_index);saved20_.categoryBits=32;saved20_.maskBits=0x51f;
 //The actual PhysicalObject owner8/source receiver exists before CreateShape,
 //whose filter callback can immediately query this SAME registry association.
 if(!associations_.constructed_physical_v112(this,&owner8_,base_,transport_,filter_borrow(),shared_from_this(),e))return fail(e,"POProjectile C1 callback association");
 associated_=true;body_.radius=config.radius;
 body_.body=world_.create_character(config,&transport_);
 if(!body_.body)return fail(e,"POProjectile native body/shape allocation");
 //Sensor shape is original secondary1c, ordinary primary18 stays C1 NULL.
 secondary1c_=body_.body->GetShapeList();primary18_=nullptr;
 constructed_=true;e.clear();return true;
}
bool CanonicalProjectilePhysicalV112::destroy_source(std::string& e){
 if(released_){e.clear();return true;}
 if(!world_.cleanup_delivery_idle_v106()){e="POProjectile D1 cannot destroy actual live physics/contact delivery";return false;}
 if(body_.body&&!world_.backend()){e="POProjectile D1 outlived its actual PhysicalWorld";return false;}
 //DestroyBody may deliver Remove to still-live receiver/peer associations.
 if(body_.body)world_.destroy(body_.body);
 primary18_=secondary1c_=nullptr;if(associated_){associations_.released(this);associated_=false;}
 released_=true;e.clear();return true;
}
}
