#pragma once
#include "canonical_object_manager_v1.hpp"
#include "retained_gameobject_visual_v1.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
#include "retained_character_actor_v1.hpp"
#include "canonical_openable_graph_v21.hpp"
namespace dh2::loader {
// Main lends the actual typed receiver and its existing visual lookup. This
// struct is delivery transport, never a second visual/actor/resource registry.
struct ActorVisualProducerV37 {
 std::shared_ptr<void> receiver_owner;
 std::uintptr_t receiver_identity{};
 const std::uintptr_t* visual2d8{};
 std::shared_ptr<world::RetainedGameObjectVisualV1> visual;
 const character::CharacterAnimationInstance* character_animation{};
 bool character_receiver{};
};
bool actor_visual_producer_v37(const std::shared_ptr<world::CanonicalOpenableGraphV21>&,
                              ActorVisualProducerV37& out,std::string&);
// lookup is main's actual Visual registry result for actor.source_visual().
// NULL published field is reported without pretending activation occurred.
bool actor_visual_producer_v37(const std::shared_ptr<character::RetainedCharacterActorV1>&,
                              std::shared_ptr<world::RetainedGameObjectVisualV1> lookup,
                              ActorVisualProducerV37& out,std::string&);
struct ActorVisualReadV37 {
 const world::CanonicalObjectBorrowV1& receiver;
 const resources::BresView& bres;
 const scene::Scene& scene;
 const std::vector<std::uint32_t>& node_flags;
 const std::vector<std::uint8_t>& source_mesh_render_enabled; // SAME source mesh render field.
 const std::vector<world::RetainedGameObjectVisualV1::SkinnedMesh>& skinned_meshes;
 std::uintptr_t visual_identity{},root_identity{};
 std::uint32_t root_flags{};
 const character::CharacterAnimationInstance* character_animation{};
};
enum class ActorVisualConsumptionV37 {no_visual_published,consumed_visual};
struct ActorVisualConsumeResultV37 {
 ActorVisualConsumptionV37 disposition{ActorVisualConsumptionV37::no_visual_published};
 bool character_receiver{},character_animation_present{};
 std::size_t instances{},skinned_meshes{};
};
struct GenericActorVisualServicesV37 {
 std::shared_ptr<void> registry_owner; // Pins actual manager/provider; no cycle.
 world::CanonicalObjectManagerV1* manager{};
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> roots;
 std::function<bool(const world::CanonicalObjectBorrowV1&,ActorVisualProducerV37&,std::string&)> lookup;
};
// Scoped synchronous read only, on the single runtime owning thread. Consume
// or upload actual mesh/skin data INSIDE the callback. Never retain the raw
// scene/BRES/skin references or release/reinitialize owners during it.
// Retained Visual::release clears bytes even when its shared_ptr survives;
// persistent renderer packets require main's genuine separate resource pin.
class GenericActorVisualInputV37 {
 GenericActorVisualServicesV37 services_;
 bool consuming_{};
 bool validate(std::int32_t,const world::CanonicalObjectBorrowV1&,
               const ActorVisualProducerV37&,std::string&)const;
public:
 explicit GenericActorVisualInputV37(GenericActorVisualServicesV37 s):services_(std::move(s)){}
 bool consume(std::int32_t canonical_key,
              const std::function<bool(const ActorVisualReadV37&,std::string&)>&,
              ActorVisualConsumeResultV37& out,std::string&);
 bool immutable_resource_pin_available()const noexcept{return false;}
 bool gameplay_readiness_claimed()const noexcept{return false;}
};
}
