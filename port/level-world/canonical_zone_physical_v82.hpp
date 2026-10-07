#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
#include "physical_world.hpp"
#include "native_body.hpp"
#include "native_physical_filter_v1.hpp"
#include <functional>
namespace dh2::world {
struct CanonicalZonePhysicalServicesV82 {
 std::shared_ptr<void> owner; // independent authority; callbacks borrow class weakly
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> update_pf;
 std::function<bool(std::uintptr_t,std::string&)> destroy_previous;
 // Peer lookup is typed native registration, never a cast of callback userdata.
 std::function<bool(void*,std::uintptr_t&,std::string&)> peer_owner;
 std::function<bool(std::uintptr_t,std::uint8_t&,std::string&)> peer_visible80;
 // Actual ObjectBase.GetHandle then ObjectHandle->GameObject. A genuine NULL
 // resolved handle skips the Zone virtual, as in POZone47075c/7a8/7f4/840.
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> resolve_peer_handle;
 // Zone.OnCollision virtualc8 is a void notification; its return register
 // never replaces POZone's already accepted base filter result.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,std::string&)> collision_test_notification;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,physical::ContactEvent,std::uintptr_t,std::string&)> contact;
};
// Source Zone::_InitPhysical39762c allocates this POZone before constructing
// its body. Caller puts it in the native release journal BEFORE construct();
// failed C1/assignment prefixes remain retained there until genuine teardown.
class CanonicalZonePhysicalV82 {
 CanonicalGameObjectBaseOwnerV1& base_;physical::NativeWorld& world_;
 std::weak_ptr<void> parent_;CanonicalZonePhysicalServicesV82 services_;
 const std::uintptr_t owner8_;std::uint8_t disabled26_{};
 physical::NativeBody native_{};physical::WorldObject transport_{};
 physical::CharacterBodyConfig config_{};
 b2FilterData saved_filter20_v105_{};b2Shape *primary18_v105_{},*secondary1c_v105_{};
 bool construction_attempted_{},constructed_{},assignment_attempted_{},assigned_{},released_{};
 std::string error_;
 bool missing(const char*,std::string&);
 bool live(std::string&)const;
 bool debug(const char*,bool&,std::string&);
 bool peer(void*,std::uintptr_t&,std::string&);
 static unsigned test(void*,void*,const physical::Filter*,const physical::Filter*);
 static void contact(void*,physical::ContactEvent,void*,const float*,unsigned);
 static void velocity(void*,float*);
public:
 CanonicalZonePhysicalV82(CanonicalGameObjectBaseOwnerV1&,physical::NativeWorld&,
  std::weak_ptr<void>,CanonicalZonePhysicalServicesV82);
 ~CanonicalZonePhysicalV82();
 CanonicalZonePhysicalV82(const CanonicalZonePhysicalV82&)=delete;
 bool construct(bool actual_trigger381,std::string&);
 bool assign(std::string&); // original SetPhysicalObject(false), no forced pin
 bool release(std::string&);
 bool physical_contact(navigation::PhysicalContact&,std::string&);
 bool borrow_native(std::shared_ptr<void>&,physical::NativeBody*&,std::string&);
 bool source_filter_borrow_v105(physical::NativePhysicalFilterBorrowV1&,std::string&);
 const std::uintptr_t* source_owner8()const noexcept{return &owner8_;}
 physical::WorldObject& transport()noexcept{return transport_;}
 bool assigned()const noexcept{return assigned_;}
};
}
