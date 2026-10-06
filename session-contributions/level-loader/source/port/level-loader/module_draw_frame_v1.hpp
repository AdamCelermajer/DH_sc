#pragma once
#include "retained_module_visual_v3.hpp"

namespace dh2::loader {
struct ModuleDrawMeshV1 {
    std::uint32_t source_instance{},geometry{};
    std::array<float,16> cached_world{};
    std::vector<scene::Material> materials;
};
// Immutable render submission from one actual retained Module visual. Resource
// ownership survives synchronous visual release; it does not pin the scene root
// or a second world. A new submission must be captured after scene changes.
struct ModuleDrawFrameV1 {
    std::int32_t module_id=-1;
    std::uintptr_t visual_identity{},root_identity{};
    std::shared_ptr<void> resource;
    resources::BresView bres{};
    std::vector<ModuleDrawMeshV1> meshes;
};
// Reads SAME-root cached matrices, visibility and attachment membership. Never
// recalculates optimized transforms or supplies conditions/animation/lighting.
// Caller supplies the actual Module receiver's module_id. Failure preserves out.
bool capture_module_draw_frame_v1(const std::shared_ptr<world::RetainedModuleVisualV3>&,
    std::int32_t module_id,ModuleDrawFrameV1& out,std::string& error);
}
