#pragma once
#include "loot_root_publishers_v49.hpp"
#include "physical_world.hpp"
#include <functional>
namespace dh2::world {
struct ProjectilePhysicalServicesV112 {
 std::shared_ptr<void> owner; //independent primitive authority
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(void*,std::uintptr_t&,std::string&)> peer_owner;
 std::function<bool(std::uintptr_t,std::uint8_t&,std::string&)> peer_visible80;
 std::function<bool(std::uintptr_t,const float*,std::string&)> on_collision;
};
//Actual POProjectile over source PhysicalObject C1 arguments used by SetInfo.
//The class receiver and C1 flags remain source-family owned; this object owns
//only its actual physics/shape/filter transport and callback association.
class CanonicalProjectilePhysicalV112 final:
 public std::enable_shared_from_this<CanonicalProjectilePhysicalV112> {
 CanonicalGameObjectBaseOwnerV1& base_;
 std::shared_ptr<void> receiver_;
 physical::NativeWorld& world_;
 CanonicalObjectManagerV1& objects_;
 character::LootPhysicalAssociationsV49& associations_;
 ProjectilePhysicalServicesV112 services_;
 std::uintptr_t owner8_{};
 physical::NativeBody body_{};
 physical::WorldObject transport_{};
 b2Shape* primary18_{};b2Shape* secondary1c_{};
 b2FilterData saved20_{};std::uint8_t disabled26_{};
 bool construction_started_{},constructed_{},associated_{},released_{};
 std::string first_failure_;
 bool resolve(std::uintptr_t&,std::uint32_t&,std::string&);
 bool fail(std::string&,const char*);
 static unsigned test(void*,void*,const physical::Filter*,const physical::Filter*);
 static void contact(void*,physical::ContactEvent,void*,const float*,unsigned);
 static void velocity(void*,float*);
public:
 CanonicalProjectilePhysicalV112(CanonicalGameObjectBaseOwnerV1&,
  std::shared_ptr<void> actual_receiver,physical::NativeWorld&,
  CanonicalObjectManagerV1&,character::LootPhysicalAssociationsV49&,
  ProjectilePhysicalServicesV112);
 ~CanonicalProjectilePhysicalV112(); //host release never impersonates source D0
 bool initialize(std::string&);
 bool destroy_source(std::string&);
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 const std::uintptr_t* source_owner8()const noexcept{return &owner8_;}
 physical::NativeBody& native()noexcept{return body_;}
 physical::WorldObject& world_object()noexcept{return transport_;}
 physical::NativePhysicalFilterBorrowV1 filter_borrow()noexcept{
  return {&world_,&body_,&primary18_,&secondary1c_,&saved20_,&disabled26_};
 }
 bool constructed()const noexcept{return constructed_;}
 bool released()const noexcept{return released_;}
};
}
