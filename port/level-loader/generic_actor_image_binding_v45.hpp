#pragma once
#include "generic_actor_draw_submission_v38.hpp"
#include "material_image_identity_v43.hpp"
namespace dh2::loader {
struct ActorImageOwnerBorrowV45 {
 std::shared_ptr<void> receiver_owner;
 std::uintptr_t receiver_identity{};
 const std::uintptr_t* published_visual2d8{};
 const std::string* model_uri290{};
 std::shared_ptr<world::RetainedGameObjectVisualV1> visual;
 MaterialImageServicesV43 images;
};
// Derive source fields from SAME canonical producers, not a parallel actor map.
bool actor_image_owner_v45(const std::shared_ptr<world::CanonicalOpenableGraphV21>&,
 MaterialImageServicesV43,ActorImageOwnerBorrowV45& out,std::string&);
bool actor_image_owner_v45(const std::shared_ptr<character::RetainedCharacterActorV1>&,
 std::shared_ptr<world::RetainedGameObjectVisualV1> actual_visual_lookup,
 MaterialImageServicesV43,ActorImageOwnerBorrowV45& out,std::string&);
struct GenericActorImageServicesV45 {
 std::shared_ptr<void> registry_owner;
 world::CanonicalObjectManagerV1* manager{};
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> roots;
 std::function<bool(const ActorVisualReadV37&,ActorImageOwnerBorrowV45&,std::string&)> borrow_resources;
 // Main's genuine resource provider must reserve/admit the actual image CPU
 // allocation BEFORE read. Its returned lease holds accounting/ownership until
 // all retained copies expire. Source adapter owns no budget or fake allowance.
 std::function<bool(const MaterialImageIdentityV43&,const std::string& canonical_uri,
                    std::shared_ptr<void>& actual_cpu_admission_pin,std::string&)> admit_before_read;
};
struct ActorMaterialImageBindingV45 {
 MaterialImageReadV43 image;
 std::shared_ptr<void> cpu_admission_pin;
};
struct ActorPrimitiveImagesReadV45 {
 const ActorDrawPrimitiveReadV38& draw;
 const std::vector<ActorMaterialImageBindingV45>& images;
};
struct ActorImageSubmissionResultV45 {
 ActorDrawSubmissionResultV38 draws;
 std::size_t sampler_bindings{},source_no_image{},actual_image_reads{};
 bool gpu_texture_residency_claimed{}; // Always false; main backend owns it.
};
// Enters V38/V37's checked synchronous consume itself; no public detached draw
// input. References expire on callback return. Image bytes/pins may be copied
// into main-owned resources independently. The byte shared_ptr also retains
// the genuine CPU admission/filesystem pins, including byte-only copies.
// Main consumer owns shader sampler selection, GPU admission/upload/context and
// pass registration. A false return requires discarding partial queued draws;
// external callback side effects are not rolled back by this source adapter.
// Owning runtime thread only. Do not release/reinitialize receivers in callbacks.
// Ordinary released/stale/foreign owners are rejected. A callback Visual release
// is checked before touching any more scene/material/primitive references.
bool submit_generic_actor_images_v45(GenericActorVisualInputV37&,std::int32_t key,
 const GenericActorImageServicesV45&,
 const std::function<bool(const ActorPrimitiveImagesReadV45&,std::string&)>&,
 ActorImageSubmissionResultV45& out,std::string&);
}
