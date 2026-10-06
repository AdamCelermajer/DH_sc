#pragma once
#include "retained_gameobject_visual_asset_connection_v1.hpp"
#include <vector>
namespace dh2::world {
// Successor to V1: sole registry for this connection, not an additional registry
// wrapped around V1. Frozen visual/base owners and source destructor remain used.
class RetainedGameObjectVisualAssetConnectionV2 {
 struct Candidate {std::shared_ptr<RetainedGameObjectVisualV1> visual;bool failed;};
 CanonicalGameObjectBaseOwnerV1& base_;RetainedGameObjectVisualServicesV1 source_;
 std::map<std::uintptr_t,Candidate> visuals_;
 std::vector<std::uintptr_t> creation_order_;
 bool destroy(std::uintptr_t,std::string&);
public:
 RetainedGameObjectVisualAssetConnectionV2(CanonicalGameObjectBaseOwnerV1& b,RetainedGameObjectVisualServicesV1 s):base_(b),source_(std::move(s)){}
 GameObjectVisualAssetServicesV1 services(std::shared_ptr<void> lease);
 std::shared_ptr<RetainedGameObjectVisualV1> lookup(std::uintptr_t)const;
 std::shared_ptr<RetainedGameObjectVisualV1> attached()const;
 std::shared_ptr<RetainedGameObjectVisualV1> lookup_root(std::uintptr_t)const;
 // Host candidate boundary: reverse construction order, each actual source
 // VisualD1 release_root -> ForceRegister. Stops at first failed delivery and
 // retains all not-yet-destroyed receivers. Diagnostics are not reset.
 bool discard_failed(std::string&);
 // Caller must first detach source base2d8 via GameObjectVisualAssetOwner.
 bool discard_unattached(std::string&);
 std::size_t retained_count()const{return visuals_.size();}
 std::size_t failed_count()const;
};
}
