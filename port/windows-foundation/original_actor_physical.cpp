#include "original_actor_physical.hpp"
#include "../level-world/native_physical_filter_v1.hpp"
#include <algorithm>
#include <cmath>
#include <exception>
#include <stdexcept>
namespace dh::foundation {
OriginalActorPhysical::OriginalActorPhysical(OriginalActorPhysicalBindings b):bindings_(std::move(b)) {world_object_.context=this;world_object_.test=test;world_object_.contact=contact;world_object_.velocity=velocity;}
OriginalActorPhysical::~OriginalActorPhysical(){if(body_.body){std::string e;if(!release(e))std::terminate();}}
bool OriginalActorPhysical::validate(std::string&e)const{
 const bool property_bound=(bindings_.properties!=nullptr)!=(bindings_.readonly_properties!=nullptr);
 if(!bindings_.actor_lease||!bindings_.world_lease||!bindings_.data_lease||!bindings_.identity||!bindings_.position160||!bindings_.destination1a8||!bindings_.attached2e0||!bindings_.visual2d8||!bindings_.world||!property_bound||(bindings_.properties&&dh2_property_validate(bindings_.properties))||!bindings_.ai||bindings_.ai->rows.size()<=8||!bindings_.static84||!bindings_.is_player){e="Required SAME original actor fields/property/AI/world providers";return false;}e.clear();return true;
}
bool OriginalActorPhysical::bind_bounds(const OriginalActorBoundsResult&box,std::string&e){
 if(!validate(e))return false;if(phase_||body_.body){e="Bounds binding requires fresh source physical receiver";return false;}
 for(unsigned k=0;k<6;++k)if(!std::isfinite(box.relative_box[k])||!std::isfinite(box.absolute_box[k])||box.absolute_box[k]!=box.relative_box[k]+bindings_.position160[k%3]){e="Produced source bounds must match SAME actor placement";return false;}
 for(unsigned k=0;k<3;++k)if(box.relative_box[k]>box.relative_box[k+3]){e="Malformed produced source bounds";return false;}
 relative_=box.relative_box;absolute_=box.absolute_box;bounds_bound_=true;e.clear();return true;
}
unsigned OriginalActorPhysical::test(void*p,void*other,const dh2::physical::Filter*own,const dh2::physical::Filter*peer){auto&t=*static_cast<OriginalActorPhysical*>(p);bool allowed=false;if(!own||!peer||!t.bindings_.filter||!t.bindings_.filter(other,*own,*peer,allowed,t.error_))throw std::runtime_error(t.error_.empty()?"Required original source physical collision filter":t.error_);return allowed;}
void OriginalActorPhysical::contact(void*p,dh2::physical::ContactEvent event,void*other,const float*,unsigned persist){if(event==dh2::physical::ContactEvent::result)return;auto&t=*static_cast<OriginalActorPhysical*>(p);if(!t.bindings_.contact||!t.bindings_.contact(event,other,persist,t.error_))throw std::runtime_error(t.error_.empty()?"Required original physical contact dispatcher":t.error_);}
void OriginalActorPhysical::velocity(void*p,float*out){auto&t=*static_cast<OriginalActorPhysical*>(p);dh2::physical::NativeBodyObservation v{};if(!out||dh2_native_body_observe(&v,&t.body_))throw std::runtime_error("Required actual source native velocity");out[0]=v.linear_velocity[0]*100.f;out[1]=v.linear_velocity[1]*100.f;}
bool OriginalActorPhysical::initialize(std::string&e){
 if(!validate(e))return false;
 if(!bounds_bound_||phase_||body_.body||!bindings_.world->backend()){e="Original physical requires produced bounds and fresh loaded world receiver";return false;}
 if(!bindings_.debug_switch||!bindings_.update_pf||!bindings_.filter||!bindings_.contact){e="Required original debug/PF/filter/contact providers";return false;}
 auto id=bindings_.readonly_properties?bindings_.readonly_properties->resolved[1]:bindings_.properties->resolved[1];if(id<0||static_cast<std::size_t>(id)>=bindings_.ai->rows.size())id=8;const auto type=bindings_.ai->rows[id].type;bool player=false;std::uint8_t stat=0;if(!bindings_.is_player(type,player,e)||!bindings_.static84(stat,e))return false;
 dh2::physical::CharacterBodyInput input{};input.owner=reinterpret_cast<void*>(bindings_.identity);input.new_physical=&body_;input.character_type=type;input.is_player=player;input.special_owner_byte=stat;
 input.absolute_bounds[0]=absolute_[0];input.absolute_bounds[1]=absolute_[1];input.absolute_bounds[2]=absolute_[3];input.absolute_bounds[3]=absolute_[4];std::copy_n(bindings_.position160,2,input.position);
 if(dh2_character_body_config(&config_,&input)){e="Original body config rejected source inputs";return false;}phase_=1;
 try {
  if(config_.enabled){bool no_collisions=false;if(!bindings_.debug_switch("MP_NoCollisions",no_collisions,e))return false;input.collision_group_override=no_collisions;if(dh2_character_body_config(&config_,&input)){e="Original debug body config failed";return false;}
   body_={bindings_.world->create_character(config_,&world_object_),config_.radius,config_.pinned};if(!body_.body){e="Original native body allocation failed";return false;}primary_=body_.body->GetShapeList();secondary_=nullptr;saved_filter_=primary_->GetFilterData();filter_disabled_=0;phase_=2;
  }
  bool no_physics=false;if(!bindings_.debug_switch("MP_NoPhysics",no_physics,e))return false;
  if(no_physics){if(body_.body)bindings_.world->destroy(body_.body);primary_=secondary_=nullptr;phase_=4;e.clear();return true;}
  // SetPhysicalObject: SAME initialized receiver assignment precedes PF update.
  physical2dc_=body_.body?reinterpret_cast<std::uintptr_t>(this):0;phase_=3;
  if(!bindings_.update_pf(bindings_.identity,body_,config_,e))return false;
  phase_=4;e.clear();return true;
 }catch(const std::exception&failure){e=failure.what();return false;}
}
bool OriginalActorPhysical::set_position(const std::array<float,3>&p,bool destination,std::string&e){
 if(!bounds_bound_){e="Source actor bounds unbound";return false;}
 dh2::character::CharacterPositionBorrowV7 actor;actor.receiver=bindings_.actor_lease;actor.identity=bindings_.identity;actor.position160=bindings_.position160;actor.relative144=relative_.data();actor.absolute12c=absolute_.data();actor.destination1a8=bindings_.destination1a8;actor.attached2e0=bindings_.attached2e0;actor.physical2dc=&physical2dc_;actor.visual2d8=bindings_.visual2d8;actor.publish_position=bindings_.publish_position;
 auto services=bindings_.position;services.world=bindings_.world_lease;
 services.physical_position=[this](std::uintptr_t id,float x,float y,std::string&error){if(id!=reinterpret_cast<std::uintptr_t>(this)||id!=physical2dc_||!body_.body){error="Required SAME source physical position receiver";return false;}const float xy[]{x,y};if(dh2_native_body_set_position(&body_,xy)<0){error="Original native SetPosition rejected input";return false;}return true;};
 dh2::character::CharacterPositionResultV7 result{};return dh2::character::character_set_position_v7(result,actor,p.data(),destination,services,e);
}
bool OriginalActorPhysical::release(std::string&e){try{if(body_.body)bindings_.world->destroy(body_.body);primary_=secondary_=nullptr;physical2dc_=0;phase_=0;e.clear();return true;}catch(const std::exception&failure){e=failure.what();return false;}}
bool OriginalActorPhysical::set_filter_enabled(bool enabled,std::string& error) {
 if(!body_.body||!physical2dc_){error="Required actual initialized physical filter receiver";return false;}
 const dh2::physical::NativePhysicalFilterBorrowV1 borrow{bindings_.world,&body_,&primary_,&secondary_,&saved_filter_,&filter_disabled_};
 return dh2::physical::native_physical_filter_v1(borrow,enabled,error);
}
bool OriginalActorPhysical::set_source_filter(std::int16_t group,std::uint16_t category,
 std::uint16_t mask,bool apply_secondary,std::string& error) {
 if(!body_.body||physical2dc_!=reinterpret_cast<std::uintptr_t>(this)||
    !bindings_.world||!bindings_.world->backend()||!bindings_.world->cleanup_delivery_idle_v106()){
  error="Required actual initialized PhysicalObject setFilter receiver";return false;
 }
 try {
  const auto apply=[&](b2Shape* shape){
   if(!shape)return;
   if(shape->GetBody()!=body_.body)throw std::runtime_error("PhysicalObject setFilter shape belongs to another body");
   b2FilterData filter;filter.groupIndex=group;filter.categoryBits=category;filter.maskBits=mask;
   shape->SetFilterData(filter);bindings_.world->backend()->Refilter(shape);
  };
  // Original 0x46ece8 mutates and Refilters primary before it even reads the
  // secondary slot. Its fourth argument controls only the latter operation.
  apply(primary_);
  if(apply_secondary)apply(secondary_);
  filter_disabled_=0;error.clear();return true;
 }catch(const std::exception& failure){error=failure.what();return false;}
}
bool OriginalActorPhysical::reset_source_filter(std::string& error) {
 if(!body_.body||physical2dc_!=reinterpret_cast<std::uintptr_t>(this)||
    !bindings_.world||!bindings_.world->backend()||!bindings_.world->cleanup_delivery_idle_v106()){
  error="Required actual initialized PhysicalObject resetFilter receiver";return false;
 }
 try {
  const auto restore=[&](b2Shape* shape){
   if(!shape)return;
   if(shape->GetBody()!=body_.body)throw std::runtime_error("PhysicalObject resetFilter shape belongs to another body");
   shape->SetFilterData(saved_filter_);bindings_.world->backend()->Refilter(shape);
  };
  // Original 0x46ec6c restores primary then secondary, using saved owner words
  // in category/mask/group layout and clearing the disabled byte after both.
  restore(primary_);restore(secondary_);filter_disabled_=0;error.clear();return true;
 }catch(const std::exception& failure){error=failure.what();return false;}
}
bool OriginalActorPhysical::set_pinned(bool pinned,std::string& error) {
 if(!body_.body||!physical2dc_){error="Required actual initialized physical pin receiver";return false;}
 try {
  if((pinned?dh2_native_body_pin(&body_):dh2_native_body_unpin(&body_))!=0){error="Original native Pin/Unpin rejected physical receiver";return false;}
  error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool OriginalActorPhysical::actor_borrow(OriginalTriggerActorBorrow&out,std::string&e)const{if(!validate(e)||!bounds_bound_){if(e.empty())e="Source actor bounds unbound";return false;}auto lease=const_cast<OriginalActorPhysical*>(this)->weak_from_this().lock();if(!lease){e="Source physical owner must be shared for scoped field borrow";return false;}out={lease,bindings_.identity,bindings_.position160,absolute_.data(),&physical2dc_};e.clear();return true;}
bool OriginalActorPhysical::physical_borrow(std::uintptr_t id,OriginalTriggerPhysicalBorrow&out,std::string&e)const{if(id!=reinterpret_cast<std::uintptr_t>(this)||id!=physical2dc_||!body_.body){e="Required SAME initialized original physical receiver";return false;}auto lease=const_cast<OriginalActorPhysical*>(this)->weak_from_this().lock();if(!lease){e="Source physical owner must be shared for scoped body borrow";return false;}out={lease,id,&body_};e.clear();return true;}
bool OriginalActorPhysical::subobjects_borrow(OriginalActorSubobjectsBorrow& out,std::string& error){
 if(!validate(error)||!bounds_bound_){if(error.empty())error="Source subobjects bounds are unbound";return false;}
 auto lease=weak_from_this().lock();if(!lease){error="Source subobjects require a shared physical owner";return false;}
 if(physical2dc_&&(!body_.body||physical2dc_!=reinterpret_cast<std::uintptr_t>(this))){error="Source physical assignment differs from its native receiver";return false;}
 out={std::move(lease),bindings_.identity,bindings_.position160,bindings_.destination1a8,
      relative_.data(),absolute_.data(),bindings_.attached2e0,bindings_.visual2d8,&physical2dc_,
      physical2dc_?&body_:nullptr};error.clear();return true;
}
}

