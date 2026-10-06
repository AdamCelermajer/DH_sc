#pragma once
#include "retained_gameobject_visual_asset_connection_v1.hpp"
#include <vector>
#include "gameobject_visual_field_borrow_v5.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
namespace dh2::world {
// Character derives GameObject; this connection borrows its SAME field owner
// and visual2d8. It never allocates a generic base or a second scene manager.
// Generic source Visual construction only; CharAnimator replacement remains
// a subsequent source lifecycle provider, not an implicit second CPU scene.
class RetainedCharacterVisualConnectionV5 {
 struct Candidate {std::shared_ptr<RetainedGameObjectVisualV1> visual;bool failed;};
 GameObjectVisualFieldBorrowV5 fields_;std::uintptr_t& visual2d8_;RetainedGameObjectVisualServicesV1 source_;
 std::shared_ptr<GameObjectSceneRootRegistryV1> manager_;
 std::map<std::uintptr_t,Candidate> visuals_;
 std::vector<std::uintptr_t> creation_order_;
 bool destroy(std::uintptr_t,std::string&);
public:
 RetainedCharacterVisualConnectionV5(GameObjectVisualFieldBorrowV5, std::uintptr_t& same_visual2d8, RetainedGameObjectVisualServicesV1, std::shared_ptr<GameObjectSceneRootRegistryV1>);
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
