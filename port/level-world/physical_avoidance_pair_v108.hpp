#pragma once
#include "physical_world.hpp"
namespace dh2::physical {
// A synchronous loan of the actual PhysicalObject receiver and its current
// shape fields. The borrower pins both owners throughout the virtual call.
struct AvoidancePhysicalBorrowV108 {
 WorldObject* receiver{};
 const navigation::PhysicalContact* fields{};
};
// PhysicalObject::canCollide 46e768 calls ONLY the selected own virtual8.
// This deliberately differs from PhysicalWorld's symmetric contact predicate.
inline int selected_avoidance_can_collide_v108(const AvoidancePhysicalBorrowV108& own,
 const AvoidancePhysicalBorrowV108& other,std::int32_t& allowed){
 allowed=0;
 if(!other.fields)return 1; // original other NULL
 if(!own.fields)return 0;
 const auto& a=*own.fields;const auto& b=*other.fields;
 if(a.disabled||b.disabled)return 1;
 const auto* af=a.primary.present?&a.primary:(a.secondary.present?&a.secondary:nullptr);
 if(!af)return 1;
 const auto* bf=b.primary.present?&b.primary:(b.secondary.present?&b.secondary:nullptr);
 if(!bf)return 1;
 if(!own.receiver||!other.receiver||!own.receiver->test)return 0;
 allowed=own.receiver->test(own.receiver->context,other.receiver->context,af,bf)!=0;
 return 1;
}
}
