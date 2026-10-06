#pragma once
#include "retained_level_module_graph_v1.hpp"
#include "gameobject_scene_root_registry_v1.hpp"

namespace dh2::loader {
struct GenericRendererObjectV36 {
    std::int32_t registry_key{};
    world::CanonicalObjectBorrowV1 receiver; // SAME fields and actual receiver lease.
};
struct GenericRendererSourceV36 {
    ObjectEntryV1 authored; // Immutable original XML including scripts/conditions/children.
    std::uint32_t module_occurrence{UINT32_MAX};
    std::int32_t module_id{-1},registry_key{};
    std::array<float,3> module_offset{};
    world::CanonicalFactoryStageV1 construction_stage{world::CanonicalFactoryStageV1::empty};
};
// A renderer input from the actual retained graph. Resource pins make already
// captured mesh payloads safe after release; they do not keep a root registered.
// Capture again each frame after pose/visibility changes. Validation checks
// owner/membership identity; it is not a pose-change detector. Validate before
// live submission/navigation/object access. This is SOURCE
// geometry + registry transport, never a GSLevel readiness/activation decision.
class GenericRendererInputV36 {
    friend bool capture_generic_renderer_input_v36(
        const std::shared_ptr<RetainedLevelModuleGraphV1>&,
        const std::shared_ptr<world::GameObjectSceneRootRegistryV1>&,
        GenericRendererInputV36&,std::string&);
    std::weak_ptr<RetainedLevelModuleGraphV1> graph_;
    std::weak_ptr<world::GameObjectSceneRootRegistryV1> roots_;
    std::uintptr_t level_identity_{};
    std::uint32_t registry_next_key_{},registry_count_{};
    LevelSourceRequestV1 request_;
    RetainedModulePreparationStatusV1 status_;
    std::shared_ptr<floors::World> floors_;
    std::vector<ModuleDrawFrameV1> modules_;
    std::vector<GenericRendererObjectV36> objects_;
    std::vector<GenericRendererSourceV36> authored_;
public:
    explicit operator bool()const noexcept{return level_identity_!=0;}
    const auto& source_request()const noexcept{return request_;}
    const auto& captured_source_status()const noexcept{return status_;}
    std::uintptr_t level_identity()const noexcept{return level_identity_;}
    const auto& module_frames()const noexcept{return modules_;}
    const auto& registry_objects()const noexcept{return objects_;}
    const auto& authored_sources()const noexcept{return authored_;}
    // This adapter deliberately does not synthesize actor visuals, their
    // activation or animation. Main must lend its SAME visual/skin producers.
    bool actor_visual_producer_bound()const noexcept{return false;}
    bool gameplay_readiness_claimed()const noexcept{return false;}
    bool validate_live(const std::shared_ptr<RetainedLevelModuleGraphV1>&,
                       std::string&)const;
    bool borrow_navigation(const std::shared_ptr<RetainedLevelModuleGraphV1>&,
                           std::shared_ptr<floors::World>& out,std::string&)const;
    bool borrow_object(const std::shared_ptr<RetainedLevelModuleGraphV1>&,
                       std::int32_t key,world::CanonicalObjectBorrowV1& out,std::string&)const;
};
// Capture on the single runtime-owning thread after actual floor/Module
// preparation. Failure preserves out. Every actual registry object and every
// reached original source attempt is retained, including unfinished prefixes.
bool capture_generic_renderer_input_v36(
    const std::shared_ptr<RetainedLevelModuleGraphV1>&,
    const std::shared_ptr<world::GameObjectSceneRootRegistryV1>&,
    GenericRendererInputV36& out,std::string&);
}
