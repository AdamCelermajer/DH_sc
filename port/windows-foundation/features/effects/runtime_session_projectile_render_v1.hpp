#pragma once

#include "runtime_session_projectile_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_scene.hpp"
#include "../../renderer.hpp"
#include "../../../scene-materials/scene.hpp"
#include "../../../engine-resources/resources.hpp"

#include <memory>

namespace dh::foundation::effects {

struct RuntimeSessionProjectileMaterialRequestV1 {
    std::string resource_uri;
    std::uint32_t range_index{};
    const OriginalMaterial* authored_material{};
    const dh2::scene::Material* source_material{};
    const dh2::resources::BresView* source_image{};
};

struct RuntimeSessionProjectileDrawV1 {
    std::uint64_t projectile_id{};
    ActorId owner{invalid_actor_id};
    std::int32_t source_row{-1}, source_model{-1};
    std::array<float, 3> position{};
    float yaw_radians{};
    Mat4 world{};
    const Mesh* mesh{};
    std::shared_ptr<const Mesh> mesh_lease;
    bool source_pass_ready{};
    std::shared_ptr<const void> source_retention;
};

struct RuntimeSessionProjectileUnresolvedV1 {
    std::uint64_t projectile_id{};
    std::string resource_uri, reason;
};

struct RuntimeSessionProjectileRenderFrameV1 {
    std::vector<RuntimeSessionProjectileDrawV1> draws;
    std::vector<RuntimeSessionProjectileUnresolvedV1> unresolved;
    bool renderer_ready{true};
};

struct RuntimeSessionProjectileRenderServicesV1 {
    // Root resolves the exact source shader/pass and uploads the source
    // texture through its current renderer. No alpha/pass fallback is allowed.
    std::function<bool(const RuntimeSessionProjectileMaterialRequestV1&,
                       Material&, std::string&)> material;
    // Keep the shared frame alive until the actual RenderQueue has drained.
    std::function<bool(std::shared_ptr<const RuntimeSessionProjectileRenderFrameV1>,
                       std::string&)> submit;
};

// CPU/source renderer payload for the actual FireWandProjectile model URI.
// Static BDAE transforms are decoded by OriginalScene; per-projectile world
// placement uses the source visual packet's position/yaw. GPU upload and frame
// ordering remain the caller's existing renderer responsibilities.
class RuntimeSessionProjectileRenderV1 final {
public:
    static bool load(AssetCatalog&, RuntimeSessionProjectileRenderServicesV1,
                     std::unique_ptr<RuntimeSessionProjectileRenderV1>&,
                     std::string& error);

    bool prepare(const std::vector<RuntimeSessionProjectileVisualV1>&,
                 std::shared_ptr<const RuntimeSessionProjectileRenderFrameV1>&,
                 std::string& error) const;
    bool submit(const std::vector<RuntimeSessionProjectileVisualV1>&,
                std::string& error) const;
    // The source Projectile PhysicalObject derives its circle from the actual
    // visual object's absolute XY AABB. This returns the exact authored BDAE
    // local geometry bounds so a same-world body can translate them by the
    // current projectile position without inventing a radius.
    bool source_projectile_bounds_v1(const std::string& model_uri,
        std::array<float, 3>& minimum, std::array<float, 3>& maximum,
        std::string& error) const;

private:
    struct SourceV1;
    RuntimeSessionProjectileRenderV1(RuntimeSessionProjectileRenderServicesV1,
                                     std::shared_ptr<SourceV1>);
    RuntimeSessionProjectileRenderServicesV1 services_;
    std::shared_ptr<SourceV1> source_;
};

} // namespace dh::foundation::effects
