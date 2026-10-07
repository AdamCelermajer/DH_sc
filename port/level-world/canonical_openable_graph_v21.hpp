#pragma once
#include "canonical_openable_container_v1.hpp"
#include "openable_container_interaction_v2.hpp"
#include "canonical_class_receiver_bindings_v1.hpp"
#include "retained_scene_visual_connection_v3.hpp"
#include "retained_gameobject_decor_v1.hpp"
#include "game_object_initialization_owner_v1.hpp"
#include "game_object_set_position_v2.hpp"
#include "retained_generic_animator_v21.hpp"
namespace dh2::world {
struct CanonicalOpenableGraphServicesV21 {
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
 OpenableContainerInteractionServicesV2 interaction;
};
// Owns one actual factory GO_ID7 runtime/base/class receiver and its visual,
// named controller and PODecor. No separate base fields or scene registry.
class CanonicalOpenableGraphV21:public std::enable_shared_from_this<CanonicalOpenableGraphV21> {
 actor::RuntimeState runtime_{};
 CanonicalOpenableGraphServicesV21 services_;
 CanonicalOpenableContainerV1 receiver_;
 std::shared_ptr<RetainedSceneVisualConnectionV3> visual_;
 std::unique_ptr<GameObjectVisualAssetOwnerV1> assets_;
 std::unique_ptr<GameObjectInitializationOwnerV1> initialization_;
 std::vector<std::unique_ptr<RetainedGameObjectDecorV1>> bodies_;
 std::shared_ptr<RetainedGenericAnimatorV21> animator_;
 std::weak_ptr<RetainedGameObjectVisualV1> animator_visual_;
 bool animator(std::shared_ptr<RetainedGenericAnimatorV21>&,std::string&);
 OpenableContainerServicesV1 container_services();
 GameObjectInitializationServicesV1 initialization_services();
 bool set_visible(bool,std::string&);
 bool physical(std::string&);
 bool destroy_physical(std::uintptr_t,std::string&);
public:
 explicit CanonicalOpenableGraphV21(CanonicalOpenableGraphServicesV21);
 CanonicalOpenableGraphV21(const CanonicalOpenableGraphV21&)=delete;
 ~CanonicalOpenableGraphV21();
 bool ready(std::string&)const;
 CanonicalClassReceiverV1 factory_receiver();
 CanonicalOpenableContainerV1& receiver()noexcept{return receiver_;}
 std::shared_ptr<RetainedGameObjectVisualV1> visual()const{return visual_->attached();}
 bool frame(std::uint32_t,std::string&);
 bool init_final(std::string& e){return receiver_.receiver().init_final(e);}
 bool interact(std::uintptr_t actor,std::string& e){return openable_container_interact_v2(receiver_.receiver(),receiver_.fields(),services_.interaction,actor,e);}
 bool set_position(const float*,bool,std::string&);
 bool physical_peer_v90(void*,std::uintptr_t&,std::shared_ptr<void>&,bool&,std::string&);
 bool borrow_native_body_v69(std::uintptr_t,std::shared_ptr<void>&,
  physical::NativeBody*&,std::string&);
 bool source_filter_borrow_v105(std::uintptr_t,physical::NativePhysicalFilterBorrowV1&,std::string&);
 bool destroy_visual_source_v92(std::uintptr_t,std::string&);
 bool destroy_physical_source_v92(std::uintptr_t,std::string&);
 bool discard_unpublished_source_v92(std::uint32_t,std::string&);
 bool require_resources_released_v92(std::string&)const;
 bool release(std::string&);
};
}
