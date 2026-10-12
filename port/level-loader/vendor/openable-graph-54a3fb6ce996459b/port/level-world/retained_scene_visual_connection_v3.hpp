#pragma once
#include "retained_gameobject_visual_asset_connection_v2.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
namespace dh2::world {
// Concrete scene-root attachment, visibility and animator destruction over
// the SAME V2 visual registry. Source asset reading/PF stay borrowed world
// providers; scene registration/release require no fixture callbacks.
class RetainedSceneVisualConnectionV3 {
 std::shared_ptr<GameObjectSceneRootRegistryV1> manager_;
 std::shared_ptr<RetainedGameObjectVisualAssetConnectionV2> connection_;
public:
 RetainedSceneVisualConnectionV3(CanonicalGameObjectBaseOwnerV1&,RetainedGameObjectVisualServicesV1,
                               std::shared_ptr<GameObjectSceneRootRegistryV1>);
 GameObjectVisualAssetServicesV1 services(std::shared_ptr<void> real_bridge_lease){return connection_->services(std::move(real_bridge_lease));}
 std::shared_ptr<RetainedGameObjectVisualV1> attached()const{return connection_->attached();}
 bool discard_failed(std::string& e){return connection_->discard_failed(e);}
 bool discard_unattached(std::string& e){return connection_->discard_unattached(e);}
 std::size_t retained_count()const{return connection_->retained_count();}
};
}
