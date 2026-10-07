#pragma once
#include "retained_gameobject_visual_v1.hpp"
#include "game_object_visual_asset_owner_v1.hpp"
#include <map>
namespace dh2::world {
// Concrete opaque-boundary replacement. Every visual handle resolves to one
// owned typed native receiver; the same base+2d8 is still the sole attachment.
class RetainedGameObjectVisualAssetConnectionV1 {
 CanonicalGameObjectBaseOwnerV1& base_;RetainedGameObjectVisualServicesV1 source_;
 std::map<std::uintptr_t,std::shared_ptr<RetainedGameObjectVisualV1>> visuals_;
 bool missing(const char*,std::string&)const;
public:
 RetainedGameObjectVisualAssetConnectionV1(CanonicalGameObjectBaseOwnerV1& b,RetainedGameObjectVisualServicesV1 s):base_(b),source_(std::move(s)){}
 GameObjectVisualAssetServicesV1 services(std::shared_ptr<void> real_connection_lease);
 std::shared_ptr<RetainedGameObjectVisualV1> lookup(std::uintptr_t)const;
 std::shared_ptr<RetainedGameObjectVisualV1> attached()const;
};
}
