#pragma once
#include "canonical_openable_container_v1.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "retained_scene_visual_connection_v3.hpp"
#include "retained_gameobject_decor_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "game_object_set_position_v2.hpp"
namespace dh2::world {
struct CanonicalOpenableGraphServicesV4 {
 std::shared_ptr<void> world;
 std::shared_ptr<GameObjectSceneRootRegistryV1> roots;
 physical::NativeWorld* physics{};
 RetainedGameObjectVisualServicesV1 visual;
 GameObjectInitializationServicesV1 initialization;
 GameObjectSetPositionServicesV2 position;
 RetainedGameObjectDecorServicesV1 decor;
 // Original table/conditions/script/audio/loot/interaction endpoints. The
 // graph replaces only services whose actual retained receiver it owns.
 OpenableContainerServicesV1 container;
};
// Owns one actual factory GO_ID7 runtime/base/class receiver and its visual,
// named controller and PODecor. No separate base fields or scene registry.
class CanonicalOpenableGraphV4:public std::enable_shared_from_this<CanonicalOpenableGraphV4> {
 actor::RuntimeState runtime_{};
 CanonicalOpenableGraphServicesV4 services_;
 CanonicalOpenableContainerV1 receiver_;
 std::shared_ptr<RetainedSceneVisualConnectionV3> visual_;
 std::unique_ptr<GameObjectVisualAssetOwnerV1> assets_;
 std::unique_ptr<GameObjectInitializationOwnerV1> initialization_;
 std::vector<std::unique_ptr<RetainedGameObjectDecorV1>> bodies_;
 OpenableContainerServicesV1 container_services();
 GameObjectInitializationServicesV1 initialization_services();
 bool set_visible(bool,std::string&);
 bool physical(std::string&);
 bool destroy_physical(std::uintptr_t,std::string&);
public:
 explicit CanonicalOpenableGraphV4(CanonicalOpenableGraphServicesV4);
 CanonicalOpenableGraphV4(const CanonicalOpenableGraphV4&)=delete;
 ~CanonicalOpenableGraphV4();
 bool ready(std::string&)const;
 CanonicalClassReceiverV1 factory_receiver();
 CanonicalOpenableContainerV1& receiver()noexcept{return receiver_;}
 std::shared_ptr<RetainedGameObjectVisualV1> visual()const{return visual_->attached();}
 bool init_final(std::string& e){return receiver_.receiver().init_final(e);}
 bool set_position(const float*,bool,std::string&);
 bool release(std::string&);
};
}
