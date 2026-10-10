#pragma once
#include "canonical_decor_v15.hpp"
#include "loot_root_publishers_v49.hpp"
namespace dh2::world {
// Caller retains this beside the actual Decor/AnimatedDecor class record.
// Borrows its EXISTING visual/registry/World and supplies the positive source
// physical leaves; floor-map and animation services remain the actual owners.
class CanonicalDecorPhysicalConnectionV49 {
 CanonicalGameObjectBaseOwnerV1& base_;physical::NativeWorld& physics_;
 character::LootPhysicalAssociationsV49& associations_;
 std::weak_ptr<void> parent_lease_;
 std::function<std::shared_ptr<RetainedGameObjectVisualV1>(std::uintptr_t)> visual_;
 RetainedGameObjectDecorServicesV1 source_;
 std::map<std::uintptr_t,std::unique_ptr<CanonicalPodDecorBodyV49>> bodies_;
 std::vector<std::uintptr_t> creation_order_;
 bool borrow_visual(std::uintptr_t,std::shared_ptr<RetainedGameObjectVisualV1>&,std::string&);
 bool construct(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t&,std::string&);
 bool assign(std::uintptr_t,bool,std::string&);
 bool destroy(std::uintptr_t,std::string&);
public:
 CanonicalDecorPhysicalConnectionV49(CanonicalGameObjectBaseOwnerV1&,physical::NativeWorld&,
  character::LootPhysicalAssociationsV49&,std::weak_ptr<void> actual_parent_lease,
  std::function<std::shared_ptr<RetainedGameObjectVisualV1>(std::uintptr_t)> actual_visual_borrow,
  RetainedGameObjectDecorServicesV1 actual_source);
 void bind(DecorServicesV15&);
 bool create_door_physical(std::string&);
 // Scoped borrow of the body already assigned to SAME physical2dc.
 // Caller retains both returned receiver pin and this connection authority.
 bool borrow_native_body(std::uintptr_t,std::shared_ptr<void>&,physical::NativeBody*&,std::string&);
 bool source_filter_borrow_v105(std::uintptr_t,physical::NativePhysicalFilterBorrowV1&,std::string&);
 bool destroy_owned_source_v92(std::uintptr_t id,std::string& e){return destroy(id,e);}
 bool release(std::string&);
 std::size_t retained_bodies()const noexcept{return bodies_.size();}
};
}

