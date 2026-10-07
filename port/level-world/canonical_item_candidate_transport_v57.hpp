#pragma once
#include "world_item_live_owner_v5.hpp"
namespace dh2::world {
// Fresh generic catalog transport borrows the ONE application's existing Item
// factory. It creates no WorldItemLiveOwner, pool, graph, manager or PropertyMap.
class CanonicalItemCandidateTransportV57 final {
 character::WorldItemLiveOwnerV5* items_{};
 CanonicalObjectManagerV1* manager_{};CanonicalPropertyMapV1* properties_{};
 std::weak_ptr<void> owner_;
 bool coherent(std::shared_ptr<void>&,std::string&)const;
public:
 CanonicalItemCandidateTransportV57(character::WorldItemLiveOwnerV5&,
  CanonicalObjectManagerV1&,CanonicalPropertyMapV1&,std::shared_ptr<void> actual_item_owner);
 bool construct(const CanonicalFactoryEntryV1&,const CanonicalSourceObjectRequestV1&,
  CanonicalClassReceiverV1&,std::string&);
};
}
