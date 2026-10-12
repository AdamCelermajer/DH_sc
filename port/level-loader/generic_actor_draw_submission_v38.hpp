#pragma once
#include "generic_actor_visual_input_v37.hpp"
namespace dh2::loader {
enum class ActorPositionSpaceV38 {raw_attribute_local,retained_skin_world};
enum class ActorMaterialBindingV38 {original_assigned_slot};
struct ActorDrawPrimitiveReadV38 {
 const ActorVisualReadV37& actor;
 const scene::Instance& instance;
 std::uint32_t instance_index{},primitive_index{};
 assets::Mesh mesh;
 assets::Primitive primitive;
 ActorPositionSpaceV38 position_space{ActorPositionSpaceV38::raw_attribute_local};
 const world::RetainedGameObjectVisualV1::SkinnedMesh* skin{};
 // Cached world already includes actual owner placement/rotation/scale. Use
 // once for raw vertices; skin.palette/positions already contain that world.
 const std::array<float,16>& cached_world;
 ActorMaterialBindingV38 material_binding{ActorMaterialBindingV38::original_assigned_slot};
 const scene::Material* bound_material{};
 // Original geometry/controller producer assigns each authored binding to
 // its corresponding mesh material slot. V39 resolves that SAME Scene-owned
 // material; primitive.material remains raw source metadata, not a selector.
};
struct ActorDrawSubmissionResultV38 {
 ActorVisualConsumptionV37 visual{ActorVisualConsumptionV37::no_visual_published};
 std::size_t visible_instances{},hidden_instances{},primitives{},static_primitives{},skinned_primitives{},unresolved_materials{};
};
// Streams transport records inside V37's checked consume scope. No GPU work,
// render-pass registration, primitive conversion, actor pose update or texture
// loading is invented. Helper exclusion consumes actual constructor visibility
// and render fields, without another source-name filter. Consumer owns those real
// source services and reports missing ones. All references expire on return.
bool submit_generic_actor_draw_v38(GenericActorVisualInputV37&,std::int32_t key,
 const std::function<bool(const ActorDrawPrimitiveReadV38&,std::string&)>&,
 ActorDrawSubmissionResultV38& out,std::string&);
}
