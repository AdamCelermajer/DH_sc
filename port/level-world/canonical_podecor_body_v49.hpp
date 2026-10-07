#pragma once
#include "retained_gameobject_decor_v1.hpp"
namespace dh2::world {
// Separated actual PODecor constructor and GameObject SetPhysicalObject phases.
// Same base/visual/world; no second actor, PF, pose or source registry.
class CanonicalPodDecorBodyV49 {
 CanonicalGameObjectBaseOwnerV1& base_;RetainedGameObjectVisualV1& visual_;
 physical::NativeWorld& world_;RetainedGameObjectDecorServicesV1 services_;
 const std::uintptr_t owner8_;std::uint8_t disabled26_{};
 physical::NativeBody native_{};physical::WorldObject transport_{};
 physical::DecorBodyConfig config_{};
 b2FilterData saved_filter20_v105_{};b2Shape *primary18_v105_{},*secondary1c_v105_{};
 bool construct_attempted_{},constructed_{},assignment_attempted_{},assigned_{};
 std::string error_;
 static unsigned test(void*,void*,const physical::Filter*,const physical::Filter*);
 static void contact(void*,physical::ContactEvent,void*,const float*,unsigned);
 static void velocity(void*,float*);
 bool missing(const char*,std::string&);
 bool debug(const char*,bool&,std::string&);
public:
 CanonicalPodDecorBodyV49(CanonicalGameObjectBaseOwnerV1&,RetainedGameObjectVisualV1&,
  physical::NativeWorld&,RetainedGameObjectDecorServicesV1);
 ~CanonicalPodDecorBodyV49();
 CanonicalPodDecorBodyV49(const CanonicalPodDecorBodyV49&)=delete;
 bool construct(std::string&);
 bool assign(bool source_pin,std::string&);
 bool release(std::string&);
 CanonicalGameObjectBaseOwnerV1& source_base()noexcept{return base_;}
 const std::uintptr_t* source_owner8()const noexcept{return &owner8_;}
 physical::WorldObject& transport()noexcept{return transport_;}
 physical::NativeBody& native()noexcept{return native_;}
 bool physical_contact(navigation::PhysicalContact&,std::string&);
 bool source_filter_borrow_v105(physical::NativePhysicalFilterBorrowV1&,std::string&);
 bool assigned()const noexcept{return assigned_;}
};
}
