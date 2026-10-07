#pragma once
#include <physical_avoidance_pair_v108.hpp>
#include <native_physical_filter_v1.hpp>
#include <functional>
#include <exception>
#include <memory>
#include <string>
namespace model_renderer {
// Owned by the synchronous GameObject frame delivery. Borrow resolves the
// SAME candidate physical association; filters are snapshots of actual shapes,
// and receiver remains the selected native PhysicalObject callable projection.
class SourceAvoidanceCollisionV108 {
public:
 struct Loan {
  std::shared_ptr<void> owner;
  dh2::physical::WorldObject* receiver{};
  dh2::navigation::PhysicalContact fields{};
  bool physical_present{};
 };
 using Borrow=std::function<bool(std::uint64_t,Loan&,std::string&)>;
 // Only after a typed association supplied the real receiver. Shape userdata
 // is checked against that receiver, never promoted into dispatch authority.
 static bool from_physical_filter(std::shared_ptr<void> pin,
  const dh2::physical::NativePhysicalFilterBorrowV1& b,
  dh2::physical::WorldObject& receiver,std::uint8_t owner80,Loan& out,std::string& e){
  if(!pin||!b.body||!b.body->body||!b.disabled||!b.primary||!b.secondary){
   e="Required actual avoidance shape/body/filter loan";return false;
  }
  Loan value;value.owner=std::move(pin);value.receiver=&receiver;value.physical_present=true;
  value.fields.present=1;value.fields.disabled=*b.disabled;
  value.fields.owner_present=1;value.fields.owner_enabled=owner80;
  auto copy=[&](b2Shape* shape,dh2::navigation::ContactFilter& filter){
   if(!shape)return true;
   if(shape->GetBody()!=b.body->body||shape->GetUserData()!=&receiver){
    e="Avoidance shape belongs to another typed physical receiver";return false;
   }
   const auto& actual=shape->GetFilterData();
   filter={actual.groupIndex,actual.categoryBits,actual.maskBits,1};return true;
  };
  if(!copy(*b.primary,value.fields.primary)||!copy(*b.secondary,value.fields.secondary))return false;
  out=std::move(value);e.clear();return true;
 }
private:
 Borrow borrow_;
 std::string error_;
 static int dispatch(void* context,std::uint64_t own,std::uint64_t other,std::int32_t* allowed){
  if(!context||!allowed)return 1;
  auto& self=*static_cast<SourceAvoidanceCollisionV108*>(context);
  if(!self.borrow_){self.error_="Required same-world avoidance PhysicalObject borrower";return 1;}
  Loan a,b;
  if(!self.borrow_(own,a,self.error_)||!self.borrow_(other,b,self.error_))return 1;
  if(!a.physical_present||!a.owner){self.error_="Missing actual own avoidance PhysicalObject";return 1;}
  if(b.physical_present&&!b.owner){self.error_="Unpinned actual peer avoidance PhysicalObject";return 1;}
  const dh2::physical::AvoidancePhysicalBorrowV108 av{a.receiver,&a.fields};
  const dh2::physical::AvoidancePhysicalBorrowV108 bv{b.receiver,b.physical_present?&b.fields:nullptr};
  try{
   if(!dh2::physical::selected_avoidance_can_collide_v108(av,bv,*allowed)){
    self.error_="Required selected PhysicalObject onCollisionTest virtual8";return 1;
   }
  }catch(const std::exception& failure){self.error_=failure.what();return 1;}
  return 0;
 }
public:
 explicit SourceAvoidanceCollisionV108(Borrow borrow):borrow_(std::move(borrow)){}
 SourceAvoidanceCollisionV108(const SourceAvoidanceCollisionV108&)=delete;
 void bind(dh2::navigation::AvoidanceScene& scene){scene.collision_context=this;scene.can_collide_v108=&dispatch;}
 const std::string& error()const noexcept{return error_;}
};
}
